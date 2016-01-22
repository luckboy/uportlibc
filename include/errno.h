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
#ifndef _ERRNO_H
#define _ERRNO_H

#include <uportsys/errno.h>

#define EPERM                   __UPORTSYS_EPERM
#define ENOENT                  __UPORTSYS_ENOENT
#define ESRCH                   __UPORTSYS_ESRCH
#define EINTR                   __UPORTSYS_EINTR
#define EIO                     __UPORTSYS_EIO
#define ENXIO                   __UPORTSYS_ENXIO
#define E2BIG                   __UPORTSYS_E2BIG
#define ENOEXEC                 __UPORTSYS_ENOEXEC
#define EBADF                   __UPORTSYS_EBADF
#define ECHILD                  __UPORTSYS_ECHILD
#define EAGAIN                  __UPORTSYS_EAGAIN
#define ENOMEM                  __UPORTSYS_ENOMEM
#define EACCES                  __UPORTSYS_EACCES
#define EFAULT                  __UPORTSYS_EFAULT
#ifdef __UPORTSYS_ENOTBLK
#define ENOTBLK                 __UPORTSYS_ENOTBLK
#endif
#define EBUSY                   __UPORTSYS_EBUSY
#define EEXIST                  __UPORTSYS_EEXIST
#define EXDEV                   __UPORTSYS_EXDEV
#define ENODEV                  __UPORTSYS_ENODEV
#define ENOTDIR                 __UPORTSYS_ENOTDIR
#define EISDIR                  __UPORTSYS_EISDIR
#define EINVAL                  __UPORTSYS_EINVAL
#define ENFILE                  __UPORTSYS_ENFILE
#define EMFILE                  __UPORTSYS_EMFILE
#define ENOTTY                  __UPORTSYS_ENOTTY
#define ETXTBSY                 __UPORTSYS_ETXTBSY
#define EFBIG                   __UPORTSYS_EFBIG
#define ENOSPC                  __UPORTSYS_ENOSPC
#define ESPIPE                  __UPORTSYS_ESPIPE
#define EROFS                   __UPORTSYS_EROFS
#define EMLINK                  __UPORTSYS_EMLINK
#define EPIPE                   __UPORTSYS_EPIPE
#define EDOM                    __UPORTSYS_EDOM
#define ERANGE                  __UPORTSYS_ERANGE

#ifdef __UPORTSYS_EADDRINUSE
#define EADDRINUSE              __UPORTSYS_EADDRINUSE
#endif
#ifdef __UPORTSYS_EADDRNOTAVAIL
#define EADDRNOTAVAIL           __UPORTSYS_EADDRNOTAVAIL
#endif
#ifdef __UPORTSYS_EAFNOSUPPORT
#define EAFNOSUPPORT            __UPORTSYS_EAFNOSUPPORT
#endif
#ifdef __UPORTSYS_EALREADY
#define EALREADY                __UPORTSYS_EALREADY
#endif
#ifdef __UPORTSYS_EBADMSG
#define EBADMSG                 __UPORTSYS_EBADMSG
#endif
#ifdef __UPORTSYS_ECANCELED
#define ECANCELED               __UPORTSYS_ECANCELED
#endif
#ifdef __UPORTSYS_ECONNABORTED
#define ECONNABORTED            __UPORTSYS_ECONNABORTED
#endif
#ifdef __UPORTSYS_ECONNREFUSED
#define ECONNREFUSED            __UPORTSYS_ECONNREFUSED
#endif
#ifdef __UPORTSYS_ECONNRESET
#define ECONNRESET              __UPORTSYS_ECONNRESET
#endif
#ifdef __UPORTSYS_EDEADLK
#define EDEADLK                 __UPORTSYS_EDEADLK
#endif
#ifdef __UPORTSYS_EDESTADDRREQ
#define EDESTADDRREQ            __UPORTSYS_EDESTADDRREQ
#endif
#ifdef __UPORTSYS_EDQUOT
#define EDQUOT                  __UPORTSYS_EDQUOT
#endif
#ifdef __UPORTSYS_EHOSTUNREACH
#define EHOSTUNREACH            __UPORTSYS_EHOSTUNREACH
#endif
#ifdef __UPORTSYS_EIDRM
#define EIDRM                   __UPORTSYS_EIDRM
#endif
#ifdef __UPORTSYS_EILSEQ
#define EILSEQ                  __UPORTSYS_EILSEQ
#endif
#ifdef __UPORTSYS_EINPROGRESS
#define EINPROGRESS             __UPORTSYS_EINPROGRESS
#endif
#ifdef __UPORTSYS_EISCONN
#define EISCONN                 __UPORTSYS_EISCONN
#endif
#ifdef __UPORTSYS_ELOOP
#define ELOOP                   __UPORTSYS_ELOOP
#endif
#ifdef __UPORTSYS_EMSGSIZE
#define EMSGSIZE                __UPORTSYS_EMSGSIZE
#endif
#ifdef __UPORTSYS_EMULTIHOP
#define EMULTIHOP               __UPORTSYS_EMULTIHOP
#endif
#ifdef __UPORTSYS_ENAMETOOLONG
#define ENAMETOOLONG            __UPORTSYS_ENAMETOOLONG
#endif
#ifdef __UPORTSYS_ENETDOWN
#define ENETDOWN                __UPORTSYS_ENETDOWN
#endif
#ifdef __UPORTSYS_ENETUNREACH
#define ENETUNREACH             __UPORTSYS_ENETUNREACH
#endif
#ifdef __UPORTSYS_ENOBUFS
#define ENOBUFS                 __UPORTSYS_ENOBUFS
#endif
#ifdef __UPORTSYS_ENODATA
#define ENODATA                 __UPORTSYS_ENODATA
#endif
#ifdef __UPORTSYS_ENOLCK
#define ENOLCK                  __UPORTSYS_ENOLCK
#endif
#ifdef __UPORTSYS_ENOLINK
#define ENOLINK                 __UPORTSYS_ENOLINK
#endif
#ifdef __UPORTSYS_ENOMSG
#define ENOMSG                  __UPORTSYS_ENOMSG
#endif
#ifdef __UPORTSYS_ENOPROTOOPT
#define ENOPROTOOPT             __UPORTSYS_ENOPROTOOPT
#endif
#ifdef __UPORTSYS_ENOSR
#define ENOSR                   __UPORTSYS_ENOSR
#endif
#ifdef __UPORTSYS_ENOSTR
#define ENOSTR                  __UPORTSYS_ENOSTR
#endif
#ifdef __UPORTSYS_ENOSYS
#define ENOSYS                  __UPORTSYS_ENOSYS
#endif
#ifdef __UPORTSYS_ENOTCONN
#define ENOTCONN                __UPORTSYS_ENOTCONN
#endif
#ifdef __UPORTSYS_ENOTEMPTY
#define ENOTEMPTY               __UPORTSYS_ENOTEMPTY
#endif
#ifdef __UPORTSYS_ENOTSOCK
#define ENOTSOCK                __UPORTSYS_ENOTSOCK
#endif
#ifdef __UPORTSYS_ENOTSUP
#define ENOTSUP                 __UPORTSYS_ENOTSUP
#endif
#ifdef __UPORTSYS_EOPNOTSUPP
#define EOPNOTSUPP              __UPORTSYS_EOPNOTSUPP
#endif
#ifdef __UPORTSYS_EOVERFLOW
#define EOVERFLOW               __UPORTSYS_EOVERFLOW
#endif
#ifdef __UPORTSYS_EPROTO
#define EPROTO                  __UPORTSYS_EPROTO
#endif
#ifdef __UPORTSYS_EPROTONOSUPPORT
#define EPROTONOSUPPORT         __UPORTSYS_EPROTONOSUPPORT
#endif
#ifdef __UPORTSYS_EPROTOTYPE
#define EPROTOTYPE              __UPORTSYS_EPROTOTYPE
#endif
#ifdef __UPORTSYS_ESTALE
#define ESTALE                  __UPORTSYS_ESTALE
#endif
#ifdef __UPORTSYS_ETIME
#define ETIME                   __UPORTSYS_ETIME
#endif
#ifdef __UPORTSYS_ETIMEDOUT
#define ETIMEDOUT               __UPORTSYS_ETIMEDOUT
#endif
#ifdef __UPORTSYS_ENETRESET
#define ENETRESET               __UPORTSYS_ENETRESET
#endif

#define EWOULDBLOCK             __UPORTSYS_EWOULDBLOCK

#ifdef __cplusplus
extern "C" {
#endif

int *__uportlibc_errno_ptr(void);

#ifdef __cplusplus
}
#endif

#define errno                   (*__uportlibc_errno_ptr())

#endif
