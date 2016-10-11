/*
 * Linux.cpp
 * Copyright (c) 2015 the copyright holders
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
#include <errno.h>
#include <math.h>
#include <setjmp.h>
#include <signal.h>
#include <stdarg.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include <xmmintrin.h>
#include <popcntintrin.h>
#include <x86intrin.h>

#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/wait.h>

#define UINT64_MAX  0xFFFFFFFFFFFFFFFFull       // XXX
#define UINT32_MAX  0xFFFFFFFF
#define UINT8_MAX   0xFF

#ifdef MACOSX   // MacOSX:
#include <mach/mach_time.h>
#define MAP_ANONYMOUS   MAP_ANON
#else           // Linux:
#include <sys/prctl.h>
#endif 

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
    int r = snprintf(name, len, "/LazyGull_%u_%s_%d", id, basename, idx);
    if (r < 0 || r >= len)
        error("failed to create object name: %s", strerror(errno));
}

/*
 * Init an object.
 */
void *init_object(const char *object, size_t size, void *addr,
    bool create, bool readonly, bool map, const void *value)
{
    int fd = -1;
    int flags = 0;
    if (object != NULL)
    {
        if (create)
            fd = shm_open(object, O_RDWR | O_CREAT | O_CLOEXEC,
                S_IRUSR | S_IWUSR);
        else
            fd = shm_open(object, O_RDWR | O_CLOEXEC, 0);
        if (fd < 0)
            error("failed to open object %s: %s", object, strerror(errno));
        if (create && ftruncate(fd, SIZE(size)) != 0)
            error("failed to truncate object %s: %s", object, strerror(errno));
        flags |= MAP_SHARED;
    }
    else
        flags |= MAP_PRIVATE | MAP_ANONYMOUS;
    if (map)
    {
        int prot = PROT_READ | (readonly && value == NULL? 0: PROT_WRITE);
        flags |= (addr == NULL? 0: MAP_FIXED);
        void *ptr = mmap(addr, SIZE(size), prot, flags, fd, 0);
        if (ptr == MAP_FAILED || (addr != NULL && ptr != addr))
            error("failed to map object %s: %s", object, strerror(errno));
        if (value != NULL)
        {
            memcpy(ptr, value, size);
            if (readonly && mprotect(ptr, SIZE(size), PROT_READ) != 0)
                error("failed to protect object %s: %s", object,
                    strerror(errno));
        }
        if (fd > 0)
            close(fd);
        return ptr;
    }
    if (fd > 0)
        close(fd);
    return NULL;
}

/*
 * Remove object.
 */
void remove_object(const char *object)
{
    if (shm_unlink(object) != 0)
        error("failed to unlink object %s: %s", object, strerror(errno));
}

/*
 * Delete an object.
 */
void delete_object(void *addr, size_t size)
{
    if (munmap(addr, SIZE(size)) != 0)
        error("failed to unmap object: %s", strerror(errno));
}

/*
 * Create a child process.
 */
void create_child(const char *hashName, const char *pvHashName,
    const char *pawnHashName, const char *dataName, const char *settingsName,
    const char *sharedName, const char *infoName, const char *tbPath)
{
    pid_t pid = fork();
    if (pid < 0)
        error("failed to fork: %s", strerror(errno));
    if (pid != 0)
        return;
#ifndef MACOSX
    prctl(PR_SET_PDEATHSIG, SIGHUP);
#endif
    char exe[PATH_MAX];
    ssize_t len = readlink("/proc/self/exe", exe, sizeof(exe)-1);
    if (len < 0)
        error("failed to read link: %s", strerror(errno));
    exe[len] = '\0';
    execl(exe, "Gull", "child", hashName, pvHashName, pawnHashName,
        dataName, settingsName, sharedName, infoName, tbPath, NULL);
    error("failed to exec: %s", strerror(errno));
}

/*
 * Get the process ID.
 */
unsigned get_pid(void)
{
    return (unsigned)getpid();
}

/*
 * Get the number of CPUs.
 */
unsigned get_num_cpus(void)
{
    return (unsigned)sysconf(_SC_NPROCESSORS_ONLN);
}

/*
 * Nuke a chold process.
 */
static void nuke_child(unsigned pid)
{
    kill((pid_t)pid, SIGKILL);
    waitpid(pid, NULL, 0);
}

/*
 * Sleep for `ms' milliseconds.
 */
static void msleep(unsigned ms)
{
    usleep(1000 * ms);
}

/*
 * Get the time in milliseconds.
 */
int64_t get_time()
{
#ifndef MACOSX
    // Linux:
    struct timespec ts;
    unsigned tick = 0;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    tick  = ts.tv_nsec / 1000000;
    tick += ts.tv_sec * 1000;
    return tick;
#else
    // MacOSX:
    return (int64_t)mach_absolute_time() / 1000000;
#endif
}

/*
 * Threads.
 */
#include <pthread.h>
#include <sys/time.h>

static void mutex_init(GMutex *mutex)
{
    pthread_mutexattr_t attrs;
    pthread_mutexattr_init(&attrs);
    pthread_mutexattr_setpshared(&attrs, PTHREAD_PROCESS_SHARED);
    pthread_mutex_init(mutex, &attrs);
}

static void cond_init(GCondVar *condVar)
{
    pthread_condattr_t attrs;
    pthread_condattr_init(&attrs);
    pthread_condattr_setpshared(&attrs, PTHREAD_PROCESS_SHARED);
    pthread_condattr_setclock(&attrs, CLOCK_MONOTONIC);
    pthread_cond_init(condVar, &attrs);
}

#define mutex_unlock    pthread_mutex_unlock

static void mutex_lock(GMutex *mutex)
{
    pthread_mutex_lock(mutex);
}

static bool mutex_lock(GMutex *mutex, uint64_t timeout)
{
    struct timespec ts;
    if (clock_gettime(CLOCK_REALTIME, &ts) != 0)
    {
        // Backup strat.
        mutex_lock(mutex);
        return false;
    }
    ts.tv_nsec += timeout * 1000000;
    int r = pthread_mutex_timedlock(mutex, &ts);
    return (r == ETIMEDOUT);
}

static void mutex_free(GMutex *mutex)
{
    // NOP 
}

#define cond_signal     pthread_cond_signal
#define cond_broadcast  pthread_cond_broadcast
#define cond_wait       pthread_cond_wait

static void cond_free(GCondVar *mutex)
{
    // NOP
}

/*
 * Input.
 */
static bool get_line(char *line, unsigned linelen, uint64_t timeout)
{
    static char buf[4 * IOSIZE];
    static unsigned ptr = 0, end = 0;
    unsigned i = 0;

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

        struct timeval tv;
        tv.tv_sec  = timeout / 1000;
        tv.tv_usec = (timeout % 1000) * 1000;
        fd_set fds;
        FD_ZERO(&fds);
        FD_SET(STDIN_FILENO, &fds);
        ssize_t res = select(STDIN_FILENO+1, &fds, NULL, NULL, &tv);
        if (res < 0)
            error("failed to wait for input: %s", strerror(errno));
        if (res == 0)
            return true;   // Timeout

        do
        {
            res = read(STDIN_FILENO, buf, sizeof(buf));
        }
        while (res < 0 && errno == EINTR);

        if (res == 0)
        {
            line[0] = EOF;
            return false;
        }
        if (res < 0)
            error("failed to read input: %s", strerror(errno));

        ptr = 0;
        end = res;
    }
}

/*
 * Output.
 */
static void put_line(char *line, unsigned linelen)
{
    unsigned ptr = 0; 
    while (ptr < linelen)
    {
        int res = write(STDOUT_FILENO, line + ptr, linelen - ptr);
        if (res < 0 && errno == EINTR)
            continue;
        if (res < 0)
            error("failed to write output: %s", strerror(errno));
        ptr += (unsigned)res;
    }
}

/*
 * O/S specific init.
 */
static void init_os(void)
{
    int fds[2];
    if (pipe2(fds, O_CLOEXEC) != 0)
        return;
    pid_t pid = getpid();
    if (fork() == 0)
    {
#ifndef MACOSX
        prctl(PR_SET_PDEATHSIG, SIGHUP);
#endif
        close(fds[1]);
        char c;
        int r = read(fds[0], &c, sizeof(c));
        kill(-pid, SIGKILL);
        error("failed to kill children: %s", strerror(errno));
    }
    close(fds[0]);
}

