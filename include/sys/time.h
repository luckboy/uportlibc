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
#ifndef _SYS_TIME_H
#define _SYS_TIME_H

#include <uportsys/sys.h>

#define FD_SETSIZE              __UPORTSYS_FD_SETSIZE

#define ITIMER_REAL             __UPORTSYS_ITIMER_REAL
#define ITIMER_VIRTUAL          __UPORTSYS_ITIMER_VIRTUAL
#define ITIMER_PROF             __UPORTSYS_ITIMER_PROF

#ifdef __cplusplus
extern "C" {
#endif

typedef __uportsys_fd_set fd_set;

#ifndef _SUSEOCONDS_T
#define _SUSEOCONDS_T
typedef __uportsys_suseconds_t suseconds_t;
#endif

#ifndef _TIME_T
#define _TIME_T
typedef __uportsys_time_t time_t;
#endif

#ifndef _STRUCT_TIMEVAL
#define _STRUCT_TIMEVAL
__UPORTSYS_STRUCT_TIMEVAL(timeval);
#endif

__UPORTSYS_STRUCT_ITIMERVAL(itimerval, timeval);

#ifdef __UPORTSYS_STRUCT_TIMEZONE
__UPORTSYS_STRUCT_TIMEZONE(timezone);
#endif

void FD_CLR(int fd, fd_set *set);
int FD_ISSET(int fd, fd_set *set);
void FD_SET(int fd, fd_set *set);
void FD_ZERO(fd_set *set);

int getitimer(int which, struct itimerval *value);
#ifdef __UPORTSYS_STRUCT_TIMEZONE
int gettimeofday(struct timeval *tv, struct timezone *tz);
#else
int gettimeofday(struct timeval *tv, void *tz);
#endif
int setitimer(int which, const struct itimerval *value, struct itimerval *old_value);
int select(int nfds, fd_set *rfds, fd_set *wfds, fd_set *efds, struct timeval *timeout);
int utimes(const char *path_name, const struct timeval *times);

#ifdef __cplusplus
}
#endif

#endif
