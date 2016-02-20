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
#include <uportsys/sys.h>
#include <errno.h>
#include <signal.h>
#include <stddef.h>

int kill(pid_t pid, int sig_num)
{ return __uportsys_kill(pid, sig_num, &errno); }

int sigaction(int sig_num, const struct sigaction *act, struct sigaction *old_act)
{ return __uportsys_sigaction(sig_num, (const struct __uportsys_sigaction *) act, (struct __uportsys_sigaction *) old_act, &errno); }

int sigaltstack(const stack_t *stack, stack_t *old_stack)
{ return __uportsys_sigaltstack(stack, old_stack, &errno); }

int sigpending(sigset_t *set)
{ return __uportsys_sigpending(set, &errno); }

int sigprocmask(int how, sigset_t *set, sigset_t *old_set)
{ return __uportsys_sigprocmask(how, set, old_set, &errno); }

int sigsuspend(const sigset_t *set)
{ return __uportsys_sigsuspend(set, &errno); }

int sigtimedwait(const sigset_t *set, siginfo_t *info, const struct timespec *timeout)
{ return __uportsys_sigtimedwait(set, (__uportsys_siginfo_t *) info, (const struct __uportsys_timespec *) timeout, &errno); }

int sigwaitinfo(const sigset_t *set, siginfo_t *info)
{ return sigtimedwait(set, info, NULL); }
