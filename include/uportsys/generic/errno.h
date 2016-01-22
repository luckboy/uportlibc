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
#ifndef _UPORTSYS_GENERIC_ERRNO_H
#define _UPORTSYS_GENERIC_ERRNO_H

#ifndef ___UPORTSYS_EXXX_BASE_ERROR
#define ___UPORTSYS_EXXX_BASE_ERROR
#define __UPORTSYS_EPERM                1
#define __UPORTSYS_ENOENT               2
#define __UPORTSYS_ESRCH                3
#define __UPORTSYS_EINTR                4
#define __UPORTSYS_EIO                  5
#define __UPORTSYS_ENXIO                6
#define __UPORTSYS_E2BIG                7
#define __UPORTSYS_ENOEXEC              8
#define __UPORTSYS_EBADF                9
#define __UPORTSYS_ECHILD               10
#define __UPORTSYS_EAGAIN               11
#define __UPORTSYS_ENOMEM               12
#define __UPORTSYS_EACCES               13
#define __UPORTSYS_EFAULT               14
#define __UPORTSYS_EBUSY                16
#define __UPORTSYS_EEXIST               17
#define __UPORTSYS_EXDEV                18
#define __UPORTSYS_ENODEV               19
#define __UPORTSYS_ENOTDIR              20
#define __UPORTSYS_EISDIR               21
#define __UPORTSYS_EINVAL               22
#define __UPORTSYS_ENFILE               23
#define __UPORTSYS_EMFILE               24
#define __UPORTSYS_ENOTTY               25
#define __UPORTSYS_ETXTBSY              26
#define __UPORTSYS_EFBIG                27
#define __UPORTSYS_ENOSPC               28
#define __UPORTSYS_ESPIPE               29
#define __UPORTSYS_EROFS                30
#define __UPORTSYS_EMLINK               31
#define __UPORTSYS_EPIPE                32
#define __UPORTSYS_EDOM                 33
#define __UPORTSYS_ERANGE               34
#endif

#ifndef ___UPORTSYS_EXXX_OTHER_ERROR
#define ___UPORTSYS_EXXX_OTHER_ERROR
#define __UPORTSYS_EADDRINUSE           35
#define __UPORTSYS_EADDRNOTAVAIL        36
#define __UPORTSYS_EAFNOSUPPORT         37
#define __UPORTSYS_EALREADY             38
#define __UPORTSYS_EBADMSG              39
#define __UPORTSYS_ECANCELED            40
#define __UPORTSYS_ECONNABORTED         41
#define __UPORTSYS_ECONNREFUSED         42
#define __UPORTSYS_ECONNRESET           43
#define __UPORTSYS_EDEADLK              44
#define __UPORTSYS_EDESTADDRREQ         45
#define __UPORTSYS_EDQUOT               46
#define __UPORTSYS_EHOSTUNREACH         47
#define __UPORTSYS_EIDRM                48
#define __UPORTSYS_EILSEQ               49
#define __UPORTSYS_EINPROGRESS          50
#define __UPORTSYS_EISCONN              51
#define __UPORTSYS_ELOOP                52
#define __UPORTSYS_EMSGSIZE             53
#define __UPORTSYS_EMULTIHOP            54
#define __UPORTSYS_ENAMETOOLONG         55
#define __UPORTSYS_ENETDOWN             56
#define __UPORTSYS_ENETUNREACH          57
#define __UPORTSYS_ENOBUFS              58
#define __UPORTSYS_ENODATA              59
#define __UPORTSYS_ENOLCK               60
#define __UPORTSYS_ENOLINK              61
#define __UPORTSYS_ENOMSG               62
#define __UPORTSYS_ENOPROTOOPT          63
#define __UPORTSYS_ENOSR                64
#define __UPORTSYS_ENOSTR               65
#define __UPORTSYS_ENOSYS               66
#define __UPORTSYS_ENOTCONN             67
#define __UPORTSYS_ENOTEMPTY            68
#define __UPORTSYS_ENOTSOCK             69
#define __UPORTSYS_ENOTSUP              70
#define __UPORTSYS_EOPNOTSUPP           71
#define __UPORTSYS_EOVERFLOW            72
#define __UPORTSYS_EPROTO               73
#define __UPORTSYS_EPROTONOSUPPORT      74
#define __UPORTSYS_EPROTOTYPE           75
#define __UPORTSYS_ESTALE               76
#define __UPORTSYS_ETIME                78
#define __UPORTSYS_ETIMEDOUT            79
#endif

#ifndef __UPORTSYS_EWOULDBLOCK
#define __UPORTSYS_EWOULDBLOCK          __UPORTSYS_EAGAIN
#endif

#endif
