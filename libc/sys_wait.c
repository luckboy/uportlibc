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
#include <sys/wait.h>
#include <uportsys/sys.h>
#include <errno.h>

pid_t wait3(int *status, int options, struct rusage *usage)
{ return __uportsys_wait3(status, options, (struct __uportsys_rusage *) usage, &errno); }

int waitid(idtype_t id_type, id_t id, siginfo_t *info, int options)
{ return __uportsys_waitid(id_type, id, (__uportsys_siginfo_t *) info, options, &errno); }

pid_t waitpid(pid_t pid, int *status, int options)
{ return __uportsys_waitpid(pid, status, options, &errno); }
