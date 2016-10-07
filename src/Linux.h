/*
 * Linux.h
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

#ifndef LINUX_H
#define LINUX_H

#include <ctype.h>
#include <stdlib.h>
#include <setjmp.h>
#include <pthread.h>

typedef int *GHandle;
typedef int GProcess;
typedef pthread_mutex_t GMutex;
typedef pthread_cond_t GCondVar;

#define __forceinline   inline
#define __align(x)      __attribute__((aligned(x)))

#define PATH_MAX        4096
#define SIZE_T          "zu"

#define INFO            ((GThreadInfo *)0x300000)
#define SETTINGS        ((GSettings *)0x301000)
#define SHARED          ((GSharedInfo *)0x302000)
#define DATA            ((GGlobalData *)0x8000000)
#define PAWNHASH        ((GPawnEntry *)0x10000000)
#define PVHASH          ((GPVEntry *)0x20000000)
#define HASH            ((GEntry *)0x40000000)

#define IOSIZE          4096

#endif
