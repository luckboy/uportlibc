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
#ifndef _SYS_WAIT_H
#define _SYS_WAIT_H

#include <sys/resource.h>
#include <uportsys/sys.h>
#include <signal.h>

#define WEXITED                 __UPORTSYS_WEXITED
#define WCONTINUED              __UPORTSYS_WCONTINUED
#define WNOHANG                 __UPORTSYS_WNOHANG
#define WNOWAIT                 __UPORTSYS_WNOWAIT
#define WSTOPPED                __UPORTSYS_WSTOPPED
#define WUNTRACED               __UPORTSYS_WUNTRACED

#define WEXITSTATUS(status)     __UPORTSYS_WEXITSTATUS(status)
#define WIFCONTINUED(status)    __UPORTSYS_WIFCONTINUED(status)
#define WIFEXITED(status)       __UPORTSYS_WIFEXITED(status)
#define WIFSIGNALED(status)     __UPORTSYS_WIFSIGNALED(status)
#define WIFSTOPPED(status)      __UPORTSYS_WIFSTOPPED(status)
#define WSTOPSIG(status)        __UPORTSYS_WSTOPSIG(status)
#define WTERMSIG(status)        __UPORTSYS_WTERMSIG(status)

#ifdef __cplusplus
extern "C" {
#endif

typedef enum __uportsys_idtype_t
{
  P_ALL = __UPORTSYS_P_ALL,
  P_PID = __UPORTSYS_P_PID,
  P_PGID = __UPORTSYS_P_PGID
} idtype_t;

pid_t wait(int *status);
pid_t wait3(int *status, int options, struct rusage *usage);
int waitid(idtype_t id_type, id_t id, siginfo_t *info, int options);
pid_t waitpid(pid_t pid, int *status, int options);

#ifdef __cplusplus
}
#endif

#endif
