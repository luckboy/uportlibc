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
#include <sys/mman.h>
#include <uportsys/sys.h>
#include <errno.h>

#ifdef ___UPORTSYS_MXXX_MEMORY_MANAGEMENT

int mlock(const void *addr, size_t len)
{ return __uportsys_mlock(addr, len, &errno); }

int mlockall(int flags)
{ return __uportsys_mlockall(flags, &errno); }

void *mmap(void *addr, size_t len, int prot, int flags, int fd, off_t offset)
{ return __uportsys_mmap(addr, len, prot, flags, fd, offset, &errno); }

int mprotect(void *addr, size_t len, int prot)
{ return __uportsys_mprotect(addr, len, prot, &errno); }

int msync(void *addr, size_t len, int flags)
{ return __uportsys_msync(addr, len, flags, &errno); }

int munlock(const void *addr, size_t len)
{ return __uportsys_munlock(addr, len, &errno); }

int munlockall(void)
{ return __uportsys_munlockall(&errno); }

int munmap(void *addr, size_t len)
{ return __uportsys_munmap(addr, len, &errno); }

#else

int mlock(const void *addr, size_t len)
{ errno = ENOSYS; return -1; }

int mlockall(int flags)
{ errno = ENOSYS; return -1; }

void *mmap(void *addr, size_t len, int prot, int flags, int fd, off_t offset)
{ errno = ENOSYS; return MAP_FAILURE; }

int mprotect(void *addr, size_t len, int prot)
{ errno = ENOSYS; return -1; }

int msync(void *addr, size_t len, int flags)
{ errno = ENOSYS; return -1; }

int munlock(const void *addr, size_t len)
{ errno = ENOSYS; return -1; }

int munlockall(void)
{ errno = ENOSYS; return -1; }

int munmap(void *addr, size_t len)
{ errno = ENOSYS; return -1; }

#endif
