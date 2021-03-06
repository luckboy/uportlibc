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

int sigaddset(sigset_t *set, int sig_num)
{
  if(sig_num < 1 || sig_num > SIGRTMAX) {
    errno = EINVAL;
    return -1;
  }
  __UPORTSYS__sigaddset(set, sig_num);
  return 0;
}

int sigdelset(sigset_t *set, int sig_num)
{
  if(sig_num < 1 || sig_num > SIGRTMAX) {
    errno = EINVAL;
    return -1;
  }
  __UPORTSYS__sigdelset(set, sig_num);
  return 0;
}

int sigemptyset(sigset_t *set)
{ __UPORTSYS__sigemptyset(set); return 0; }

int sigfillset(sigset_t *set)
{ __UPORTSYS__setfillset(set); return 0; }

int sigismember(sigset_t *set, int sig_num)
{
  if(sig_num < 1 || sig_num > SIGRTMAX) {
    errno = EINVAL;
    return -1;
  }
  return __UPORTSYS__setismember(set, sig_num);  
}
