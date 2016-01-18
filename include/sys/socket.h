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
#ifndef _SYS_SOCKET_H
#define _SYS_SOCKET_H

#include <uportsys/sys.h>

#define AF_UNSPEC               __UPORTSYS_AF_UNSPEC
#define AF_UNIX                 __UPORTSYS_AF_UNIX
#define AF_INET                 __UPORTSYS_AF_INET
#ifdef __UPORTSYS_AF_INET6
#define AF_INET6                __UPORTSYS_AF_INET6
#endif

#define CMSG_DATA(cmsg)         __UPORTSYS_CMSG_DATA((struct __uportsys_cmsghdr *) (cmsg))
#define CMSG_FIRSTHDR(mhdr)     __UPORTSYS_CMSG_FIRSTHDR((struct __uportsys_cmsghdr *) (cmsg))
#define CMSG_NXTHDR(mhdr, cmsg) __UPORTSYS_CMSG_NXTHDR((struct __uportsys_msghdr *) (mhdr), (struct __uportsys_cmsghdr *) (cmsg))

#define MSG_CTRUNC              __UPORTSYS_MSG_CTRUNC
#define MSG_DONTROUTE           __UPORTSYS_MSG_DONTROUTE
#define MSG_EOR                 __UPORTSYS_MSG_EOR
#define MSG_OOB                 __UPORTSYS_MSG_OOB
#define MSG_PEEK                __UPORTSYS_MSG_PEEK
#define MSG_TRUNC               __UPORTSYS_MSG_TRUNC
#define MSG_WAITALL             __UPORTSYS_MSG_WAITALL

#define SCM_RIGHTS              __UPORTSYS_SCM_RIGHTS

#define SHUT_RD                 __UPORTSYS_SHUT_RD
#define SHUT_WR                 __UPORTSYS_SHUT_WR
#define SHUT_RDWR               __UPORTSYS_SHUT_RDWR

#define SO_ACCEPTCONN           __UPORTSYS_SO_ACCEPTCONN
#define SO_BROADCAST            __UPORTSYS_SO_BROADCAST
#define SO_DEBUG                __UPORTSYS_SO_DEBUG
#define SO_DONTROUTE            __UPORTSYS_SO_DONTROUTE
#define SO_ERROR                __UPORTSYS_SO_ERROR
#define SO_KEEPALIVE            __UPORTSYS_SO_KEEPALIVE
#define SO_LINGER               __UPORTSYS_SO_LINGER
#define SO_OOBINLINE            __UPORTSYS_SO_OOBINLINE
#define SO_RCVBUF               __UPORTSYS_SO_RCVBUF
#define SO_RCVLOWAT             __UPORTSYS_SO_RCVLOWAT
#define SO_RCVTIMEO             __UPORTSYS_SO_RCVTIMEO
#define SO_REUSEADDR            __UPORTSYS_SO_REUSEADDR
#define SO_SNDBUF               __UPORTSYS_SO_SNDBUF
#define SO_SNDLOWAT             __UPORTSYS_SO_SNDLOWAT
#define SO_SNDTIMEO             __UPORTSYS_SO_SNDTIMEO
#define SO_TYPE                 __UPORTSYS_SO_TYPE

#define SOCK_STREAM             __UPORTSYS_SOCK_STREAM
#define SOCK_DGRAM              __UPORTSYS_SOCK_DGRAM
#define SOCK_SEQPACKET          __UPORTSYS_SOCK_SEQPACKET

#define SOL_SOCKET              __UPORTSYS_SOL_SOCKET

#ifdef __cplusplus
extern "C" {
#endif

#ifndef _SIZE_T
#define _SIZE_T
typedef __SIZE_TYPE__ size_t;
#endif

typedef __uportsys_socklen_t socklen_t;
typedef __uportsys_sa_family_t sa_family_t;

#ifndef _SSIZE_T
#define _SSIZE_T
typedef __uportsys_ssize_t ssize_t;
#endif

#ifndef _STRUCT_IOVEC
#define _STRUCT_IOVEC
__UPORTSYS_STRUCT_IOVEC(iovec);
#endif

__UPORTSYS_STRUCT_CMSGHDR(cmsghdr);
__UPORTSYS_STRUCT_LINGER(linger);
__UPORTSYS_STRUCT_MSGHDR(msghdr, iovec);
__UPORTSYS_STRUCT_SOCKADDR(sockaddr);

int accept(int sd, struct sockaddr *addr, socklen_t *addr_len);
int bind(int sd, const struct sockaddr *addr, socklen_t addr_len);
int connect(int sd, const struct sockaddr *addr, socklen_t addr_len);
int getpeername(int sd, struct sockaddr *addr, socklen_t *addr_len);
int getsockname(int sd, struct sockaddr *addr, socklen_t *addr_len);
int getsockopt(int sd, int level, int opt_name, void *opt_value, socklen_t *opt_len);
int listen(int sd, int backlog);
ssize_t recv(int sd, void *buf, size_t len, int flags);
ssize_t recvfrom(int sd, void *buf, size_t len, int flags, struct sockaddr *addr, socklen_t *addr_len);
ssize_t recvmsg(int sd, struct msghdr *msg, int flags);
ssize_t send(int sd, const void *buf, size_t len, int flags);
ssize_t sendmsg(int sd, const struct msghdr *msg, int flags);
ssize_t sendto(int sd, const void *buf, size_t len, int flags, const struct sockaddr *addr, socklen_t addr_len);
int setsockopt(int sd, int level, int opt_name, const void *opt_value, socklen_t opt_len);
int shutdown(int sd, int how);
int socket(int domain, int type, int protocol);
int socket2(int domain, int type, int protocol, int *sds);

#ifdef __cplusplus
}
#endif

#endif
