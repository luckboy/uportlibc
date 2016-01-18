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
#ifndef _SYS_RESOURCE_H
#define _SYS_RESOURCE_H

#include <uportsys/sys.h>

#define PRIO_PROCESS            __UPORTSYS_PRIO_PROCESS
#define PRIO_PGRP               __UPORTSYS_PRIO_PGRP
#define PRIO_USER               __UPORTSYS_PRIO_USER

#define RLIM_INFINITY           __UPORTSYS_RLIM_INFINITY
#define RLIM_SAVED_MAX          __UPORTSYS_RLIM_SAVED_MAX
#define RLIM_SAVED_CUR          __UPORTSYS_RLIM_SAVED_CUR

#define RUSAGE_SELF             __UPORTSYS_RUSAGE_SELF
#define RUSAGE_CHILDREN         __UPORTSYS_RUSAGE_CHILDREN

#define RLIMIT_AS               __UPORTSYS_RLIMIT_AS
#define RLIMIT_CORE             __UPORTSYS_RLIMIT_CORE
#define RLIMIT_CPU              __UPORTSYS_RLIMIT_CPU
#define RLIMIT_DATA             __UPORTSYS_RLIMIT_DATA
#define RLIMIT_FSIZE            __UPORTSYS_RLIMIT_FSIZE
#define RLIMIT_NOFILE           __UPORTSYS_RLIMIT_NOFILE
#define RLIMIT_STACK            __UPORTSYS_RLIMIT_STACK

#ifdef __cplusplus
extern "C" {
#endif

#ifndef _ID_T
#define _ID_T
typedef __uportsys_id_t id_t;
#endif

typedef __uportsys_rlim_t rlim_t;

#ifndef _STRUCT_TIMEVAL
#define _STRUCT_TIMEVAL
__UPORTSYS_STRUCT_TIMEVAL(timeval);
#endif

__UPORTSYS_STRUCT_RLIMIT(rlimit);
__UPORTSYS_STRUCT_RUSAGE(rusage, timeval);

int getpriority(int which, id_t who);
int getrlimit(int resource, struct rlimit *rlim);
int getrusage(int who, struct rusage *usage);
int setpriority(int which, id_t who, int prio);
int setrlimit(int resource, const struct rlimit *rlim);

#ifdef __cplusplus
}
#endif

#endif
