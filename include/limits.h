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
#ifndef _LIMITS_H
#define _LIMITS_H

#include <uportlibc/float_limits.h>
#include <uportsys/sys.h>

/* Runtime macros. */

#define CHAR_BIT                __CHAR_BIT__
#define WORD_BIT                (__SIZEOF_INT__ * 8)
#define LONG_BIT                (__SIZEOF_LONG__ * 8)

#define SCHAR_MAX               __SCHAR_MAX__
#define SCHAR_MIN               (-SCHAR_MAX - 1)
#define SHRT_MAX                __SHRT_MAX__
#define SHRT_MIN                (-SHRT_MAX - 1)
#define INT_MAX                 __INT_MAX__
#define INT_MIN                 (-INT_MAX - 1)
#define LONG_MAX                __LONG_MAX__
#define LONG_MIN                (-LONG_MAX - 1L)
#define LONG_LONG_MAX           __LONG_LONG_MAX__
#define LONG_LONG_MIN           (-LONG_LONG_MAX - 1LL)

#define UCHAR_MAX               (SCHAR_MAX * 2U + 1U)
#define USHRT_MAX               (SHRT_MAX * 2U + 1U)
#define UINT_MAX                (INT_MAX * 2U + 1U)
#define ULONG_MAX               (LONG_MAX * 2UL + 1UL)
#define ULONG_LONG_MAX          (LONG_LONG_MAX * 2ULL + 1ULL)

#ifdef __CHAR_UNSIGNED__
#define CHAR_MAX                UCHAR_MAX
#define CHAR_MIN                0U
#else
#define CHAR_MAX                SCHAR_MAX
#define CHAR_MIN                SCHAR_MIN
#endif

#define MB_LEN_MAX              4
#define SSIZE_MAX               INT_MAX

/* System macros. */

#define ARG_MAX                 __UPORTSYS_ARG_MAX
#define CHILD_MAX               __UPORTSYS_CHILD_MAX
#define FILESIZEBITS            __UPORTSYS_FILESIZEBITS
#define IOV_MAX                 __UPORTSYS_IOV_MAX
#define LINK_MAX                __UPORTSYS_LINK_MAX
#define MAX_CANON               __UPORTSYS_MAX_CANON
#define MAX_INPUT               __UPORTSYS_MAX_INPUT
#define NAME_MAX                __UPORTSYS_NAME_MAX
#define NGROUPS_MAX             __UPORTSYS_NGROUPS_MAX
#define OPEN_MAX                __UPORTSYS_OPEN_MAX
#define PAGE_SIZE               __UPORTSYS_PAGE_SIZE
#define PATH_MAX                __UPORTSYS_PATH_MAX
#define PIPE_BUF                __UPORTSYS_PIPE_BUF
#define SIGQUEUE_MAX            __UPORTSYS_SIGQUEUE_MAX

/* Other macros. */

#define ATEXIT_MAX              INT_MAX
#define RTSIG_MAX               (__UPORTSYS_SIGRTMAX - __UPORTSYS_SIGRTMIN + 1)
#define SEM_NSEMS_MAX           INT_MAX
#define SEM_VALUE_MAX           INT_MAX
#define STREAM_MAX              INT_MAX
#define TTY_NAME_MAX            (NAME_MAX + 1)
#define TZNAME_MAX              6

#endif
