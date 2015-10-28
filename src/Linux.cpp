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
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include <xmmintrin.h>
#include <popcntintrin.h>

#include <sys/mman.h>
#include <poll.h>
#include <sys/prctl.h>
#include <unistd.h>

#define builtin_cpuid(f, ax, bx, cx, dx)    \
    __asm__ __volatile__ ("cpuid" : "=a" (ax), "=b" (bx), "=c" (cx), \
        "=d" (dx) : "a" (f))

#define builtin_popcnt_u64(x)   _mm_popcnt_u64((x))

/*
 * Replacements for the Windows Interlocked* functions.
 */
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

/*
 * Initialize the hash table.
 */
void init_hash()
{
    static sint64 old_size = 0;
    sint64 size = hash_size * sizeof(GEntry);
    if (parent && Hash != NULL)
        munmap(Hash, old_size);
    old_size = size;
    if (parent)
    {
        // Unlike Windows Gull there is no need to create a file for this
        // mapping.  Instead, the child process will simply inherit the
        // mapping after the fork().
        int prot = PROT_READ | PROT_WRITE,
            flags = MAP_SHARED | MAP_ANONYMOUS;
        Hash = (GEntry *)mmap(NULL, size, prot, flags, -1, 0);
        assert(Hash != MAP_FAILED);
    }
    hash_mask = hash_size - 4;
}

/*
 * Initialize shared memory regions.
 */
void init_shared()
{
    // This is similar to init_hash().
    static sint64 old_size = 0;
    sint64 size = SharedPVHashOffset + pv_hash_size * sizeof(GPVEntry);
    if (parent && Smpi != NULL)
        munmap(Smpi, old_size);
    old_size = size;
    if (parent)
    {
        int prot = PROT_READ | PROT_WRITE,
            flags = MAP_SHARED | MAP_ANONYMOUS;
        Smpi = (GSMPI *)mmap(NULL, size, prot, flags, -1, 0);
        assert(Smpi != MAP_FAILED);
    }
    Material = (GMaterial*)(((char*)Smpi) + SharedMaterialOffset);
    MagicAttacks = (uint64*)(((char*)Smpi) + SharedMagicOffset);
    PVHash = (GPVEntry*)(((char*)Smpi) + SharedPVHashOffset);
}

/*
 * Test if input is available.
 */
int input()
{
    struct pollfd fds;
    int ret;
    fds.fd = 0;     // stdin
    fds.events = POLLIN;
    return poll(&fds, 1, 0);
}

/*
 * Create a child process and return its ID.
 */
int main(int argc, char **argv);        // Forward decl.
GProcess CreateChildProcess(int child_id)
{
    GProcess id = fork();
    if (id != 0)
        return id;

    close(0);       // No need for stdin, stdout, stderr
    close(1);
    close(2);

    prctl(PR_SET_PDEATHSIG, SIGHUP);    // Ensure that the child dies with
                                        // the parent. (necessary?)

    // Gull expects the child process to enter through main().
    // So we simply call main() here:
    char child_id_buf[32];
    int ret = snprintf(child_id_buf, sizeof(child_id_buf)-1, "%d", child_id);
    assert(ret > 0 && ret < sizeof(child_id_buf)-1);
    char *argv[] =
    {
        "/proc/self/exe",
        "child",
        "0",
        child_id_buf,
        NULL
    };
    ret = main(4, argv);
    exit(ret);
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
sint64 get_time()
{
    struct timespec ts;
    unsigned tick = 0;
    clock_gettime(CLOCK_MONOTONIC_RAW, &ts);
    tick  = ts.tv_nsec / 1000000;
    tick += ts.tv_sec * 1000;
    return tick;
}

