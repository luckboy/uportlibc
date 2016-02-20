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
#ifndef _FCNTL_H
#define _FCNTL_H

#include <uportsys/sys.h>

#define F_DUP                   __UPORTSYS_F_DUP
#define F_GETFD                 __UPORTSYS_F_GETFD
#define F_SETFD                 __UPORTSYS_F_SETFD
#define F_GETFL                 __UPORTSYS_F_GETFL
#define F_SETFL                 __UPORTSYS_F_SETFL
#define F_GETLK                 __UPORTSYS_F_GETLK
#define F_SETLK                 __UPORTSYS_F_SETLK
#define F_SETLKW                __UPORTSYS_F_SETLKW

#define FD_CLOEXEC              __UPORTSYS_FD_CLOEXEC

#define F_RDLCK                 __UPORTSYS_F_RDLCK
#define F_UNLCK                 __UPORTSYS_F_UNLCK
#define F_WRLCK                 __UPORTSYS_F_WRLCK

#define O_ACCMODE               __UPORTSYS_O_ACCMODE
#define O_RDONLY                __UPORTSYS_O_RDONLY
#define O_WRONLY                __UPORTSYS_O_WRONLY
#define O_RDWR                  __UPORTSYS_O_RDWR

#define O_CREAT                 __UPORTSYS_O_CREAT
#define O_EXCL                  __UPORTSYS_O_EXCL
#define O_NOCTTY                __UPORTSYS_O_NOCTTY
#define O_TRUNC                 __UPORTSYS_O_TRUNC
#define O_APPEND                __UPORTSYS_O_APPEND
#define O_DSYNC                 __UPORTSYS_O_DSYNC
#define O_NONBLOCK              __UPORTSYS_O_NONBLOCK
#define O_RSYNC                 __UPORTSYS_O_RSYNC
#define O_SYNC                  __UPORTSYS_O_SYNC

#ifdef __cplusplus
extern "C" {
#endif

#ifndef _MODE_T
#define _MODE_T
typedef __uportsys_mode_t mode_t;
#endif

#ifndef _OFF_T
#define _OFF_T
typedef __uportsys_off_t off_t;
#endif

#ifndef _PID_T
#define _PID_T
typedef __uportsys_pid_t pid_t;
#endif

__UPORTSYS_STRUCT_FLOCK(flock);

int creat(const char *file_name, mode_t mode);
int fcntl(int fd, int cmd, ...);
int open(const char *file_name, int flags, ...);

#ifdef __cplusplus
}
#endif

#endif
