/*
 * Windows.cpp
 * Original Gull code is in the "public domain".
 * New code: Copyright (c) 2015 the copyright holders
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

#include <setjmp.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <windows.h>

#include <xmmintrin.h>
#include <popcntintrin.h>
#include <x86intrin.h>

#define builtin_cpuid(f, ax, bx, cx, dx)    \
    __asm__ __volatile__ ("cpuid" : "=a" (ax), "=b" (bx), "=c" (cx), \
        "=d" (dx) : "a" (f))

#define builtin_popcnt_u64(x)   _mm_popcnt_u64((x))

#define builtin_sync_fetch_and_add(x, y)                \
    __sync_fetch_and_add((x), (y))
#define builtin_sync_fetch_and_or(x, y)                 \
    __sync_fetch_and_or((x), (y))
#define builtin_sync_fetch_and_and(x, y)                 \
    __sync_fetch_and_and((x), (y))
#define builtin_sync_bit_test_and_reset(x, y)           \
    ((__sync_fetch_and_and((x), ~(1 << (y))) & (1 << (y))) != 0)
#define builtin_sync_lock_test_and_set(x, y)            \
    __sync_lock_test_and_set((x), (y))
#define builtin_sync_val_compare_and_swap(x, y, z)      \
    __sync_val_compare_and_swap((x), (z), (y))

void init_hash() { 
#ifdef TUNER
    return;
#endif
    char name[256];
    sint64 size = (hash_size * sizeof(GEntry));
    int min_page_size;
    HINSTANCE hDll;
    sprintf(name, "GULL_HASH_%d", WinParId);
    int initialized = 0;
    if (parent && HASH != NULL) {
        initialized = 1;
        UnmapViewOfFile(Hash);
        CloseHandle(HASH);
    }
    if (parent) {
        if (!LargePages) goto no_lp;
#ifndef LARGE_PAGES
        goto no_lp;
#endif 
        typedef int(*GETLARGEPAGEMINIMUM)(void);
        GETLARGEPAGEMINIMUM pGetLargePageMinimum;
        hDll = LoadLibrary(TEXT("kernel32.dll"));
        if (hDll == NULL) goto no_lp;
        pGetLargePageMinimum = (GETLARGEPAGEMINIMUM)GetProcAddress(hDll, "GetLargePageMinimum"); 
        if (pGetLargePageMinimum == NULL) goto no_lp;
        min_page_size = (*pGetLargePageMinimum)();
        if (size < min_page_size) size = min_page_size;
        if (!initialized) {
            TOKEN_PRIVILEGES tp;
            HANDLE hToken;
            OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hToken);
            LookupPrivilegeValue(NULL, "SeLockMemoryPrivilege", &tp.Privileges[0].Luid);
            tp.PrivilegeCount = 1;
            tp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
            AdjustTokenPrivileges(hToken, FALSE, &tp, 0, (PTOKEN_PRIVILEGES)NULL, 0);
        }
        HASH = NULL;
        HASH = CreateFileMapping(INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE | SEC_COMMIT | SEC_LARGE_PAGES, size >> 32, size & 0xFFFFFFFF, name);
        if (HASH != NULL) {
            fprintf(stdout, "Large page hash\n");
            goto hash_allocated;
        }
    no_lp:
        HASH = CreateFileMapping(INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, size >> 32, size & 0xFFFFFFFF, name);
    } else HASH = OpenFileMapping(FILE_MAP_ALL_ACCESS, 0, name);
hash_allocated:
    Hash = (GEntry*)MapViewOfFile(HASH, FILE_MAP_ALL_ACCESS, 0, 0, size);
    if (parent) memset(Hash, 0, size);
    hash_mask = hash_size - 4;
}

void init_shared() {
#ifdef TUNER
    return;
#endif
    char name[256];
    sint64 size = SharedPVHashOffset + pv_hash_size * sizeof(GPVEntry);
    sprintf(name, "GULL_SHARED_%d", WinParId);
    if (parent && SHARED != NULL) {
        UnmapViewOfFile(Smpi);
        CloseHandle(SHARED);
    }
    if (parent) SHARED = CreateFileMapping(INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, 0, size, name);
    else SHARED = OpenFileMapping(FILE_MAP_ALL_ACCESS, 0, name);
    Smpi = (GSMPI*)MapViewOfFile(SHARED, FILE_MAP_ALL_ACCESS, 0, 0, size);
    if (parent) memset(Smpi, 0, size);
    Material = (GMaterial*)(((char*)Smpi) + SharedMaterialOffset);
    MagicAttacks = (uint64*)(((char*)Smpi) + SharedMagicOffset);
    PVHash = (GPVEntry*)(((char*)Smpi) + SharedPVHashOffset);
    if (parent) memset(PVHash, 0, pv_hash_size * sizeof(GPVEntry));
}

int input() {
    if (child) return 0;
    DWORD p;
    if (F(Input)) return 0;
    if (F(Console)) {
        if (PeekNamedPipe(StreamHandle,NULL,0,NULL,&p,NULL)) return (p > 0);
        else return 1;
    } else return 0;
}         

HANDLE CreateChildProcess(int child_id) {
    char name[1024];
    TCHAR szCmdline[1024];
    PROCESS_INFORMATION piProcInfo;
    STARTUPINFO siStartInfo;
    BOOL bSuccess = FALSE;
      
    ZeroMemory(&piProcInfo, sizeof(PROCESS_INFORMATION));
    ZeroMemory(&siStartInfo, sizeof(STARTUPINFO));
    ZeroMemory(szCmdline, 1024 * sizeof(TCHAR));
    ZeroMemory(name, 1024);
  
    siStartInfo.cb = sizeof(STARTUPINFO);
    siStartInfo.dwFlags |= STARTF_USESTDHANDLES;
  
    GetModuleFileName(NULL, name, 1024);
    sprintf(szCmdline, " child %d %d", WinParId, child_id);
      
    bSuccess = CreateProcess(TEXT(name), TEXT(szCmdline), NULL, NULL, FALSE, CREATE_NO_WINDOW, NULL, NULL, &siStartInfo, &piProcInfo);
  
    if (bSuccess) {
        CloseHandle(piProcInfo.hThread);
        return piProcInfo.hProcess;
    } else {
        fprintf(stdout, "Error %d\n", GetLastError());
        return NULL; 
    }
}

static void msleep(unsigned ms)
{
    Sleep(ms);
}

sint64 get_time() {
#ifdef CPU_TIMING
#ifndef TIMING
    if (CpuTiming) {
#endif
        uint64 ctime;
        QueryProcessCycleTime(GetCurrentProcess(), &ctime);
#ifdef TIMING
        return ctime / (CyclesPerMSec / 1000);
#endif
        return (ctime / CyclesPerMSec);
#ifndef TIMING
    } 
#endif    
#endif    
    return GetTickCount();
}

