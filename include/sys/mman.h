/*
 * Copyright (c) 2016 Łukasz Szpakowski
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */
#ifndef _SYS_MMAN_H
#define _SYS_MMAN_H

#include <uportsys/sys.h>

#define MAP_SHARED              __UPORTSYS_MAP_SHARED
#define MAP_PRIVATE             __UPORTSYS_MAP_PRIVATE
#define MAP_FIXED               __UPORTSYS_MAP_FIXED

#define MAP_FAILURE             __UPORTSYS_MAP_FAILURE

#define MS_ASYNC                __UPORTSYS_MS_ASYNC
#define MS_INVALIDATE           __UPORTSYS_MS_INVALIDATE
#define MS_SYNC                 __UPORTSYS_MS_SYNC

#define MCL_CURRENT             __UPORTSYS_MCL_CURRENT
#define MCL_FUTURE              __UPORTSYS_MCL_FUTURE

#define PROT_READ               __UPORTSYS_PROT_READ
#define PROT_WRITE              __UPORTSYS_PROT_WRITE
#define PROT_EXEC               __UPORTSYS_PROT_EXEC
#define PROT_NONE               __UPORTSYS_PROT_NONE

#ifdef __cplusplus
extern "C" {
#endif

#ifndef _OFF_T
#define _OFF_T
typedef __uportsys_off_t off_t;
#endif

#ifndef _SIZE_T
#define _SIZE_T
typedef __SIZE_TYPE__ size_t;
#endif

int mlock(const void *addr, size_t len);
int mlockall(int flags);
void *mmap(void *addr, size_t len, int prot, int flags, int fd, off_t offset);
int mprotect(void *addr, size_t len, int prot);
int msync(void *addr, size_t len, int flags);
int munlock(const void *addr, size_t len);
int munlockall(void);
int munmap(void *addr, size_t len);

#ifdef __cplusplus
}
#endif

#endif
