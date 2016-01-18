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
#include <sys/resource.h>
#include <uportsys/sys.h>
#include <errno.h>

int getpriority(int which, id_t who)
{ return __uportsys_getpriority(which, who, &errno); }

int getrlimit(int resource, struct rlimit *rlim)
{ return __uportsys_getrlimit(resource, (struct __uportsys_rlimit *) rlim, &errno); }

int getrusage(int who, struct rusage *usage)
{ return __uportsys_getrusage(who, (struct __uportsys_rusage *) usage, &errno); }

int setpriority(int which, id_t who, int prio)
{ return __uportsys_setpriority(which, who, prio, &errno); }

int setrlimit(int resource, const struct rlimit *rlim)
{ return __uportsys_setrlimit(resource, (const struct __uportsys_rlimit *) rlim, &errno); }
