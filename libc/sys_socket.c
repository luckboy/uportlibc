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
#include <sys/socket.h>
#include <sys/resource.h>
#include <errno.h>

int accept(int sd, struct sockaddr *addr, socklen_t *addr_len)
{ return __uportsys_accept(sd, (struct __uportsys_sockaddr *) addr, addr_len, &errno); }

int bind(int sd, const struct sockaddr *addr, socklen_t addr_len)
{ return __uportsys_bind(sd, (const struct __uportsys_sockaddr *) addr, addr_len, &errno); }

int connect(int sd, const struct sockaddr *addr, socklen_t addr_len)
{ return __uportsys_connect(sd, (const struct __uportsys_sockaddr *) addr, addr_len, &errno); }

int getpeername(int sd, struct sockaddr *addr, socklen_t *addr_len)
{ return __uportsys_getpeername(sd, (struct __uportsys_sockaddr *) addr, addr_len, &errno); }

int getsockname(int sd, struct sockaddr *addr, socklen_t *addr_len)
{ return __uportsys_getsockname(sd, (struct __uportsys_sockaddr *) addr, addr_len, &errno); }

int getsockopt(int sd, int level, int opt_name, void *opt_value, socklen_t *opt_len)
{ return __uportsys_getsockopt(sd, level, opt_name, opt_value, opt_len, &errno); }

int listen(int sd, int backlog)
{ return __uportsys_listen(sd, backlog, &errno); }

ssize_t recv(int sd, void *buf, size_t len, int flags)
{ return __uportsys_recv(sd, buf, len, flags, &errno); }

ssize_t recvfrom(int sd, void *buf, size_t len, int flags, struct sockaddr *addr, socklen_t *addr_len)
{ return __uportsys_recvfrom(sd, buf, len, flags, (struct __uportsys_sockaddr *) addr, addr_len, &errno); }

ssize_t recvmsg(int sd, struct msghdr *msg, int flags)
{ return __uportsys_recvmsg(sd, (struct __uportsys_msghdr *) msg, flags, &errno); }

ssize_t send(int sd, const void *buf, size_t len, int flags)
{ return __uportsys_send(sd, buf, len, flags, &errno); }

ssize_t sendmsg(int sd, const struct msghdr *msg, int flags)
{ return __uportsys_sendmsg(sd, (const struct __uportsys_msghdr *) msg, flags, &errno); }

ssize_t sendto(int sd, const void *buf, size_t len, int flags, const struct sockaddr *addr, socklen_t addr_len)
{ return __uportsys_sendto(sd, buf, len, flags, (const struct __uportsys_sockaddr *) addr, addr_len, &errno); }

int setsockopt(int sd, int level, int opt_name, const void *opt_value, socklen_t opt_len)
{ return __uportsys_setsockopt(sd, level, opt_name, opt_value, opt_len, &errno); }

int shutdown(int sd, int how)
{ return __uportsys_shutdown(sd, how, &errno); }

int socket(int domain, int type, int protocol)
{ return __uportsys_socket(domain, type, protocol, &errno); }

int socket2(int domain, int type, int protocol, int *sds)
{ return __uportsys_socket2(domain, type, protocol, sds, &errno); }
