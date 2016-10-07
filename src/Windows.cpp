/*
 * Windows.cpp
 * Original Gull code is in the "public domain".
 * New code: Copyright (c) 2016 the copyright holders
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 */

#include <assert.h>
#include <setjmp.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <windows.h>

#include <xmmintrin.h>
#include <popcntintrin.h>
#include <x86intrin.h>

#define UINT64_MAX  0xFFFFFFFFFFFFFFFFull       // XXX
#define UINT32_MAX  0xFFFFFFFF

#define builtin_cpuid(f, ax, bx, cx, dx)    \
    __asm__ __volatile__ ("cpuid" : "=a" (ax), "=b" (bx), "=c" (cx), \
        "=d" (dx) : "a" (f))

#define PAGE_SIZE       4096
#define SIZE(size)      ((((size)-1) / PAGE_SIZE) * PAGE_SIZE + PAGE_SIZE)

/*
 * Log a message.
 */
static void log(const char *format, ...)
{
    FILE *stream = fopen("lazygull.log", "a");
    if (stream == NULL)
        return;
    va_list ap;
    va_start(ap, format);
    vfprintf(stream, format, ap);
    va_end(ap);
    fclose(stream);
}

/*
 * Print an error message.
 */
#define error(format, ...)                                              \
    do {                                                                \
        log("error: " format "\n", ##__VA_ARGS__);                      \
        fprintf(stderr, "error: " format "\n", ##__VA_ARGS__);          \
        abort();                                                        \
    } while (false)

/*
 * Init an object name.
 */
void init_object_name(char *name, size_t len, const char *basename,
    unsigned id, int idx)
{
    int r = snprintf(name, len, "Local\\LazyGull_%u_%s_%d", id, basename, idx);
    if (r < 0 || r >= len)
        error("failed to create object name (%d)", GetLastError());
}

/*
 * Init an object.
 */
typedef struct
{
    char name[256];
    HANDLE handle;
} GHandleInfo;

static GHandleInfo handleInfo[16] = {0};

void *init_object(const char *object, size_t size, void *addr,
    bool create, bool readonly, bool map, const void *value)
{
    size_t size2 = SIZE(size);
    HANDLE handle = INVALID_HANDLE_VALUE;
    if (object != NULL)
    {
        if (create)
        {
            handle = CreateFileMapping(INVALID_HANDLE_VALUE, NULL, 
                PAGE_READWRITE, (DWORD)(size2 >> 32),
                (DWORD)(size2 & 0xFFFFFFFF), object);
            for (unsigned i = 0;
                    i < sizeof(handleInfo) / sizeof(handleInfo[0]); i++)
            {
                if (handleInfo[i].name[0] == '\0')
                {
                    strncpy(handleInfo[i].name, object,
                        sizeof(handleInfo[i].name)-1);
                    handleInfo[i].handle = handle;
                    break;
                }
            }
        }
        else
            handle = OpenFileMapping(FILE_MAP_ALL_ACCESS, FALSE, object);
    }
    else
        handle = CreateFileMapping(INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE,
            (DWORD)(size2 >> 32), (DWORD)(size2 & 0xFFFFFFFF), NULL);
    if ((handle == INVALID_HANDLE_VALUE || handle == NULL) &&
            (create || GetLastError() != ERROR_ALREADY_EXISTS))
        error("failed to open file mapping \"%s\" (%d)", object,
            GetLastError());
    void *ptr = NULL;
    if (map)
    {
        DWORD access = FILE_MAP_READ |
            (readonly && value == NULL? 0: FILE_MAP_WRITE);
        ptr = MapViewOfFileEx(handle, access, 0, 0, size2, addr);
        if (ptr == NULL)
            error("failed to map file mapping \"%s\" (%d)", object,
                GetLastError());
        if (value != NULL)
        {
            memcpy(ptr, value, size);
            DWORD old_prot;
            if (readonly &&
                    !VirtualProtect(ptr, size2, PAGE_READONLY, &old_prot))
                error("failed to protect object \"%s\" (%d)", object,
                    GetLastError());
        }
    }
    if (!create || object == NULL)
        CloseHandle(handle);
    return ptr;
}

/*
 * Remove object.
 */
void remove_object(const char *object)
{
    for (unsigned i = 0; i < i < sizeof(handleInfo) / sizeof(handleInfo[0]);
            i++)
    {
        if (strcmp(handleInfo[i].name, object) == 0)
        {
            handleInfo[i].name[0] = '\0';
            CloseHandle(handleInfo[i].handle);
            handleInfo[i].handle = NULL;
            return;
        }
    }
    error("failed to remove object \"%s\"", object);
}

/*
 * Delete an object.
 */
void delete_object(void *addr, size_t size)
{
    if (!UnmapViewOfFile(addr))
        error("failed to unmap object (%d)", GetLastError());
}

/*
 * Create a child process.
 */
void create_child(const char *hashName, const char *pvHashName,
    const char *pawnHashName, const char *dataName, const char *settingsName,
    const char *sharedName, const char *infoName, const char *tbPath)
{
    char name[PATH_MAX];
    char command[10 * PATH_MAX];
    PROCESS_INFORMATION procInfo;
    STARTUPINFO startInfo;
      
    memset(&procInfo, 0, sizeof(procInfo));
    memset(&startInfo, 0, sizeof(startInfo));
  
    startInfo.cb = sizeof(STARTUPINFO);
    startInfo.dwFlags |= STARTF_USESTDHANDLES;
    startInfo.hStdError = GetStdHandle(STD_ERROR_HANDLE); 
    startInfo.hStdOutput = GetStdHandle(STD_OUTPUT_HANDLE); 
    startInfo.hStdInput = INVALID_HANDLE_VALUE;
  
    if (GetModuleFileName(NULL, name, sizeof(name)-1) >= sizeof(name)-1)
        error("failed to get module name");
    int len = snprintf(command, sizeof(command)-1,
        "\"%s\" child \"%s\" \"%s\" \"%s\" \"%s\" \"%s\" \"%s\" \"%s\" \"%s\"",
        name, hashName, pvHashName, pawnHashName, dataName, settingsName,
        sharedName, infoName, tbPath);
    if (len < 0 || len >= sizeof(command)-1)
        error("failed to create command line for child"); 

    BOOL success = CreateProcess(NULL, command, NULL, NULL, TRUE,
        0, NULL, NULL, &startInfo, &procInfo);
    if (!success)
        error("failed to create child process (%d)", GetLastError());
    CloseHandle(procInfo.hThread);
}

/*
 * Get the process ID.
 */
unsigned get_pid(void)
{
    return (unsigned)GetCurrentProcessId();
}

/*
 * Get the number of CPUs.
 */
unsigned get_num_cpus(void)
{
    SYSTEM_INFO sysinfo;
    GetSystemInfo(&sysinfo);
    return (unsigned)sysinfo.dwNumberOfProcessors;
}

/*
 * Nuke a chold process.
 */
static void nuke_child(unsigned pid)
{
    HANDLE handle = OpenProcess(PROCESS_ALL_ACCESS, FALSE, (DWORD)pid);
    if (handle == NULL)
        return;
    TerminateProcess(handle, EXIT_SUCCESS);
    if (WaitForSingleObject(handle, INFINITE) != WAIT_OBJECT_0)
        error("failed to terminate child (%d)", GetLastError());
    CloseHandle(handle);
}

/*
 * Sleep for `ms' milliseconds.
 */
static void msleep(unsigned ms)
{
    Sleep(ms);
}

/*
 * Get the time in milliseconds.
 */
int64_t get_time(void)
{
    return GetTickCount64();
}

/*
 * Threads.
 */
typedef HANDLE GMutex;
typedef HANDLE GEvent;

static void mutex_init(GMutex *mutex)
{
    SECURITY_ATTRIBUTES attr;
    memset(&attr, 0, sizeof(attr));
    attr.nLength = sizeof(attr);
    attr.bInheritHandle = TRUE;
    *mutex = CreateMutex(&attr, FALSE, NULL);
    if (*mutex == NULL)
        error("failed to create mutex (%d)", GetLastError());
}

static void mutex_lock(GMutex *mutex)
{
    if (WaitForSingleObject(*mutex, INFINITE) != WAIT_OBJECT_0)
        error("failed to lock mutex (%d)", GetLastError());
}

static bool mutex_lock(GMutex *mutex, uint64_t timeout)
{
    switch (WaitForSingleObject(*mutex, (DWORD)timeout))
    {
        case WAIT_OBJECT_0:
            return false;
        case WAIT_TIMEOUT:
            return true;
        default:
            error("failed to lock mutex (%d)", GetLastError());
    }
}

static void mutex_unlock(GMutex *mutex)
{
    if (!ReleaseMutex(*mutex))
        error("failed to unlock mutex (%d)", GetLastError());
}

static void mutex_free(GMutex *mutex)
{
    CloseHandle(*mutex);
}

static void event_init(GEvent *event)
{
    SECURITY_ATTRIBUTES attr;
    memset(&attr, 0, sizeof(attr));
    attr.nLength = sizeof(attr);
    attr.bInheritHandle = TRUE;
    *event = CreateEvent(&attr, FALSE, FALSE, NULL);
    if (*event == NULL)
        error("failed to create event (%d)", GetLastError());
}

static void event_signal(GEvent *event)
{
    SetEvent(*event);
}

static void event_wait(GEvent *event, GMutex *mutex)
{
    if (SignalObjectAndWait(*mutex, *event, INFINITE, FALSE) !=
            WAIT_OBJECT_0)
        error("failed to wait for event (%d)", GetLastError());
}

static void event_free(GEvent *event)
{
    CloseHandle(*event);
}

/*
 * Input
 */
static DWORD forward(LPVOID param)
{
    char buf[4 * IOSIZE];
    HANDLE in  = GetStdHandle(STD_INPUT_HANDLE);
    HANDLE out = (HANDLE)param;
    while (true)
    {
        DWORD len;
        if (!ReadFile(in, buf, sizeof(buf), &len, NULL))
            error("failed to read input (%d)", GetLastError());
        if (len == 0)
        {
            CloseHandle(out);
            return 0;
        }

        DWORD ptr = 0;
        while (ptr < len)
        {
            DWORD writelen;
            if (!WriteFile(out, buf + ptr, len - ptr, &writelen, NULL))
                error("failed to forward input (%d)", GetLastError());
            ptr += writelen;
        }

        FlushFileBuffers(out);
    }
}

static bool get_line(char *line, unsigned linelen, uint64_t timeout)
{
    static char buf[4 * IOSIZE];
    static unsigned ptr = 0, end = 0;
    unsigned i = 0;

    static HANDLE handle = INVALID_HANDLE_VALUE;
    static GEvent event = NULL;
    static bool init = false;
    if (!init)
    {
        handle = GetStdHandle(STD_INPUT_HANDLE);
        if (GetFileType(handle) == FILE_TYPE_PIPE)
        {
            char name[256];
            int res = snprintf(name, sizeof(name)-1,
                "\\\\.\\pipe\\LazyGull_%u_pipe", get_pid());
            if (res < 0 || res >= sizeof(name)-1)
                error("failed to create pipe name");
            HANDLE out = CreateNamedPipe(name,
                PIPE_ACCESS_DUPLEX | FILE_FLAG_OVERLAPPED,
                PIPE_TYPE_BYTE | PIPE_READMODE_BYTE | PIPE_WAIT, 2,
                4 * IOSIZE, 4 * IOSIZE, 0, NULL);
            if (out == INVALID_HANDLE_VALUE)
                error("failed to create named pipe #1 (%d)", GetLastError());
            handle = CreateFile(name, GENERIC_READ, 0, NULL,
                OPEN_EXISTING, FILE_FLAG_OVERLAPPED, NULL);
            if (handle == INVALID_HANDLE_VALUE)
                error("failed to create named pipe #2 (%d)", GetLastError());
            HANDLE thread = CreateThread(NULL, 0, forward, (LPVOID)out, 0,
                NULL);
            if (thread == NULL)
                error("failed to create thread (%d)", GetLastError());
        }
        event_init(&event);
        init = true;
    }
 
    while (true)
    {
        bool space = false;
        while (ptr < end)
        {
            if (i >= linelen)
                error("input buffer overflow");
            char c = buf[ptr++];
            switch (c)
            {
                case ' ': case '\r': case '\t': case '\n':
                    if (!space)
                    {
                        line[i++] = ' ';
                        space = true;
                    }
                    if (c == '\n')
                    {
                        line[i-1] = '\0';
                        return false;
                    }
                    continue;
                default:
                    space = false;
                    line[i++] = c;
                    continue;
            }
        }

        OVERLAPPED overlapped;
        memset(&overlapped, 0, sizeof(overlapped));
        overlapped.hEvent = event;
        DWORD len;
        if (!ReadFile(handle, buf, sizeof(buf), &len, &overlapped))
        {
            if (GetLastError() != ERROR_IO_PENDING)
                error("failed to read input (%d)", GetLastError());
            bool timedout = false;

            switch (WaitForSingleObject(event, timeout))
            {
                case WAIT_TIMEOUT:
                    if (!CancelIo(handle))
                        error("failed to cancel input (%d)", GetLastError());
                    timedout = true;
                    break;
                case WAIT_OBJECT_0:
                    break;
                default:
                    error("failed to wait for input (%d)", GetLastError());
            }
            if (!GetOverlappedResult(handle, &overlapped, &len, FALSE))
            {
                if (timedout && GetLastError() == ERROR_OPERATION_ABORTED)
                    return true;
                error("failed to get input result (%d)", GetLastError());
            }
        }

        if (len == 0)
        {
            line[0] = EOF;
            return false;
        }

        ptr = 0;
        end = len;
    }
}

/*
 * Output
 */
static void put_line(char *line, unsigned linelen)
{
    unsigned ptr = 0;
    HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
    while (ptr < linelen)
    {
        DWORD len;
        if (!WriteFile(handle, line + ptr, linelen - ptr, &len, NULL))
            error("failed to write output (%d)", GetLastError());
        ptr += (unsigned)len;
    }
    FlushFileBuffers(handle);
}

/*
 * O/S specific init.
 */
static void init_os(void)
{
    HANDLE handle = GetStdHandle(STD_INPUT_HANDLE);
    DWORD mode;
    if (GetConsoleMode(handle, &mode))
    {
        mode &= ~ENABLE_MOUSE_INPUT;
        mode &= ~ENABLE_WINDOW_INPUT;
        mode |= ENABLE_LINE_INPUT;
        mode |= ENABLE_ECHO_INPUT;
        SetConsoleMode(handle, mode);
        FlushConsoleInputBuffer(handle);
    }

    HANDLE job = CreateJobObject(NULL, NULL);
    if (job == NULL)
        error("failed to create job object (%d)", GetLastError());
    JOBOBJECT_EXTENDED_LIMIT_INFORMATION info;
    memset(&info, 0, sizeof(info));
    info.BasicLimitInformation.LimitFlags = JOB_OBJECT_LIMIT_KILL_ON_JOB_CLOSE;
    SetInformationJobObject(job, JobObjectExtendedLimitInformation, &info,
                sizeof(info));
    AssignProcessToJobObject(job, GetCurrentProcess());     // Allowed to fail
}

