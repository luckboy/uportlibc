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
#ifndef _POLL_H
#define _POLL_H

#include <uportsys/sys.h>

#define POLLIN                  __UPORTSYS_POLLIN
#define POLLPRI                 __UPORTSYS_POLLPRI
#define POLLRDBAND              __UPORTSYS_POLLRDBAND
#define POLLRDNORM              __UPORTSYS_POLLRDNORM
#define POLLOUT                 __UPORTSYS_POLLOUT
#define POLLWRBAND              __UPORTSYS_POLLWRBAND
#define POLLWRNORM              __UPORTSYS_POLLWRNORM
#define POLLERR                 __UPORTSYS_POLLERR
#define POLLHUP                 __UPORTSYS_POLLHUP
#define POLLNVAL                __UPORTSYS_POLLNVAL

#ifdef __cplusplus
extern "C" {
#endif

typedef __uportsys_nfds_t nfds_t;

__UPORTSYS_STRUCT_POLLFD(pollfd);

int poll(struct pollfd *fds, nfds_t nfds, int timeout);

#ifdef __cplusplus
}
#endif

#endif
