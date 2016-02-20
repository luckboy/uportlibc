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
#include <sys/time.h>
#include <uportsys/sys.h>
#include <errno.h>
#include <time.h>

int getitimer(int which, struct itimerval *value)
{ return __uportsys_getitimer(which, (struct __uportsys_itimerval *) value, &errno); }

#ifdef __UPORTSYS_STRUCT_TIMEZONE
int gettimeofday(struct timeval *tv, struct timezone *tz)
{ return __uportsys_gettimeofday((struct __uportsys_timeval *) tv, (struct __uportsys_timezone *) tz, &errno); }
#else
int gettimeofday(struct timeval *tv, void *tz)
{ return __uportsys_gettimeofday((struct __uportsys_timeval *) tv, tz, &errno); }
#endif

int setitimer(int which, const struct itimerval *value, struct itimerval *old_value)
{ return __uportsys_setitimer(which, (const struct __uportsys_itimerval *) value, (struct __uportsys_itimerval *) old_value, &errno); }

int select(int nfds, fd_set *rfds, fd_set *wfds, fd_set *efds, struct timeval *timeout)
{ return __uportsys_select(nfds, rfds, wfds, efds, (struct __uportsys_timeval *) timeout, &errno); }

int utimes(const char *path_name, const struct timeval *times)
{ return __uportsys_utimes(path_name, (const struct __uportsys_timeval *) times, &errno); }

int nanosleep(const struct timespec *req, const struct timespec *rem)
{ return __uportsys_nanosleep((const struct __uportsys_timespec *) req, (struct __uportsys_timespec *) rem, &errno); } 

#ifdef ___UPORTSYS_CLK_TCK
clock_t __uportlibc_clk_tck(void)
{
  int tmp_err_num;
  return __uportsys_clk_tck(&tmp_err_num);
}
#endif
