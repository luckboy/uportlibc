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
#ifndef _SYS_TYPES_H
#define _SYS_TYPES_H

#include <uportsys/sys.h>

#ifdef __cplusplus
extern "C" {
#endif

#ifndef _BLKCNT_T
#define _BLKCNT_T
typedef __uportsys_blkcnt_t blkcnt_t;
#endif

#ifndef _BLKSIZE_T
#define _BLKSIZE_T
typedef __uportsys_blksize_t blksize_t;
#endif

#ifndef _CLOCK_T
#define _CLOCK_T
typedef __uportsys_clock_t clock_t;
#endif

#ifndef _DEV_T
#define _DEV_T
typedef __uportsys_dev_t dev_t;
#endif

#ifndef _FSBLKCNT_T
#define _FSBLKCNT_T
typedef __uportsys_fsblkcnt_t fsblkcnt_t;
#endif

#ifndef _FSFILCNT_T
#define _FSFILCNT_T
typedef __uportsys_fsfilcnt_t fsfilcnt_t;
#endif

#ifndef _GID_T
#define _GID_T
typedef __uportsys_gid_t gid_t;
#endif

#ifndef _INO_T
#define _INO_T
typedef __uportsys_ino_t ino_t;
#endif

#ifndef _MODE_T
#define _MODE_T
typedef __uportsys_mode_t mode_t;
#endif

#ifndef _NLINK_T
#define _NLINK_T
typedef __uportsys_nlink_t nlink_t;
#endif

#ifndef _OFF_T
#define _OFF_T
typedef __uportsys_off_t off_t;
#endif

#ifndef _PID_T
#define _PID_T
typedef __uportsys_pid_t pid_t;
#endif

#ifndef _SIZE_T
#define _SIZE_T
typedef __SIZE_TYPE__ size_t;
#endif

#ifndef _SSIZE_T
#define _SSIZE_T
typedef __uportsys_ssize_t ssize_t;
#endif

#ifndef _SUSECONDS_T
#define _SUSECONDS_T
typedef __uportsys_suseconds_t suseconds_t;
#endif

#ifndef _TIME_T
#define _TIME_T
typedef __uportsys_time_t time_t;
#endif

#ifndef _UID_T
#define _UID_T
typedef __uportsys_uid_t uid_t;
#endif

#ifndef _USECONDS_T
#define _USECONDS_T
typedef __uportsys_useconds_t useconds_t;
#endif

#ifdef __cplusplus
}
#endif

#endif
