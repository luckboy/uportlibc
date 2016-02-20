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
#ifndef _TIME_H
#define _TIME_H

#include <uportsys/sys.h>

#ifdef ___UPORTSYS_CLK_TCK
#define CLK_TCK                 (__uportlibc_clk_tck())
#else
#define CLK_TCK                 __UPORTSYS_CLK_TCK
#endif

#define CLOCKS_PER_SEC          1000000

#ifndef __cplusplus
#define NULL                    ((void *) 0)
#else
#define NULL                    0
#endif

#ifdef __cplusplus
extern "C" {
#endif

#ifndef _CLOCK_T
#define _CLOCK_T
typedef __uportsys_clock_t clock_t;
#endif

#ifndef _SIZE_T
#define _SIZE_T
typedef __SIZE_TYPE__ size_t;
#endif

#ifndef _TIME_T
#define _TIME_T
typedef __uportsys_time_t time_t;
#endif

#ifndef _STRUCT_TIMESPEC
#define _STRUCT_TIMESPEC
__UPORTSYS_STRUCT_TIMESPEC(timespec);
#endif

struct tm
{
  int tm_sec;
  int tm_min;
  int tm_hour;
  int tm_mday;
  int tm_mon;
  int tm_year;
  int tm_wday;
  int tm_yday;
  int tm_isdst;
};

extern int daylight;
extern long timezone;
extern char *tzname[2];

/* System functions. */

int nanosleep(const struct timespec *req, const struct timespec *rem);
#ifdef ___UPORTSYS_CLK_TCK
clock_t __uportlibc_clk_tck(void);
#endif

/* Other functions. */

char *asctime(const struct tm *tm);
char *asctime_r(const struct tm *tm, char *buf);
clock_t clock(void);
char *ctime(const time_t *time_ptr);
char *ctime_r(const time_t *time_ptr, char *buf);
double difftime(const time_t *time_ptr1, const time_t *time_ptr2);
struct tm *getdate(const char *str);
struct tm *gmtime(const time_t *time_ptr);
struct tm *gmtime_r(const time_t *time_ptr, struct tm *result);
struct tm *localtime(const time_t *time_ptr);
struct tm *localtime_r(const time_t *time_ptr, struct tm *result);
size_t strftime(const char *str, size_t max_size, const char *format, const struct tm *tm);
char *strptime(char *buf, const char *format, struct tm *tm);
void tzset(void);

#ifdef __cplusplus
}
#endif

#endif
