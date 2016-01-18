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
#ifndef _UPORTSYS_GENERIC_SYS_H
#define _UPORTSYS_GENERIC_SYS_H

#include <uportsys/inttypes.h>

#ifdef __cplusplus
extern "C" {
#endif

/*
 * System macros.
 */

/* System macros for the netinet/in.h header. */

#ifndef ___UPORTSYS_IPPROTO_XXX
#define ___UPORTSYS_IPPROTO_XXX
#define __UPORTSYS_IPPROTO_IP           0
#define __UPORTSYS_IPPROTO_ICMP         1
#define __UPORTSYS_IPPROTO_TCP          6
#define __UPORTSYS_IPPROTO_UDP          17
#endif

/* System macros for the sys/mman.h header. */

#ifndef ___UPORTSYS_MAP_XXX
#define ___UPORTSYS_MAP_XXX
#define __UPORTSYS_MAP_SHARED           (1 << 0)
#define __UPORTSYS_MAP_PRIVATE          (1 << 1)
#define __UPORTSYS_MAP_FIXED            (1 << 4)
#endif

#ifndef __UPORTSYS_MAP_FAILURE
#define __UPORTSYS_MAP_FAILURE          ((void *) (-1))
#endif

#ifndef ___UPORTSYS_MS_XXX
#define ___UPORTSYS_MS_XXX
#define __UPORTSYS_MS_ASYNC             (1 << 0)
#define __UPORTSYS_MS_INVALIDATE        (1 << 1)
#define __UPORTSYS_MS_SYNC              (1 << 2)
#endif

#ifndef ___UPORTSYS_MCL_XXX
#define ___UPORTSYS_MCL_XXX
#define __UPORTSYS_MCL_CURRENT          (1 << 0)
#define __UPORTSYS_MCL_FUTURE           (1 << 1)
#endif

#ifndef ___UPORTSYS_PROT_XXX
#define ___UPORTSYS_PROT_XXX
#define __UPORTSYS_PROT_READ            (1 << 0)
#define __UPORTSYS_PROT_WRITE           (1 << 1)
#define __UPORTSYS_PROT_EXEC            (1 << 2)
#define __UPORTSYS_PROT_NONE            0
#endif

/* System macros for the sys/resource.h header. */

#ifndef ___UPORTSYS_PRIO_XXX
#define ___UPORTSYS_PRIO_XXX
#define __UPORTSYS_PRIO_PROCESS         0
#define __UPORTSYS_PRIO_PGRP            1
#define __UPORTSYS_PRIO_USER            2
#endif

#ifndef ___UPORTSYS_RLIM_XXX
#define ___UPORTSYS_RLIM_XXX
#ifdef __UPORTSYS_RLIM_T
#define __UPORTSYS_RLIM_INFINITY        (((__UPORTSYS_RLIM_T) 0U) - 1U)
#else
#define __UPORTSYS_RLIM_INFINITY        (((__uportsys_uint64_t) 0U) - 1U)
#endif
#define __UPORTSYS_RLIM_SAVED_MAX       __UPORTSYS_RLIM_INFINITY
#define __UPORTSYS_RLIM_SAVED_CUR       __UPORTSYS_RLIM_INFINITY
#endif

#ifndef ___UPORTSYS_RUSAGE_XXX
#define ___UPORTSYS_RUSAGE_XXX
#define __UPORTSYS_RUSAGE_SELF          0
#define __UPORTSYS_RUSAGE_CHILDREN      (-1)
#endif

#ifndef ___UPORTSYS_RLIMIT_XXX
#define ___UPORTSYS_RLIMIT_XXX
#define __UPORTSYS_RLIMIT_AS            0
#define __UPORTSYS_RLIMIT_CORE          1
#define __UPORTSYS_RLIMIT_CPU           2
#define __UPORTSYS_RLIMIT_DATA          3
#define __UPORTSYS_RLIMIT_FSIZE         4
#define __UPORTSYS_RLIMIT_NOFILE        5
#define __UPORTSYS_RLIMIT_STACK         6
#endif

/* System macros for the sys/socket.h header. */

#ifndef ___UPORTSYS_AF_XXX
#define ___UPORTSYS_AF_XXX
#define __UPORTSYS_AF_UNSPEC            0
#define __UPORTSYS_AF_UNIX              1
#define __UPORTSYS_AF_INET              2
#endif

#ifndef ___UPORTSYS_CMSG_XXX
#define ___UPORTSYS_CMSG_XXX

#ifndef __UPORTSYS_CMSG_ALIGN
#define __UPORTSYS_CMSG_ALIGN(x)        (((x) + (sizeof(long) - 1)) & ~(sizeof(long) - 1))
#endif

#define __UPORTSYS_CMSG_NEXT(cmsg)                                          \
(__UPORTSYS_CMSG_DATA(cmsg) + __UPORTSYS_CMSG_ALIGN((cmsg)->cmsg_len))

#define __UPORTSYS_CMSG_END(mhdr)                                           \
(((unsigned char *) (mhdr)->control) + (mhdr)->controlllen)

#define __UPORTSYS_CMSG_NEXT_DATA(cmsg)                                     \
(__UPORTSYS_CMSG_NEXT(cmsg) + sizeof(struct __uportsys_cmsghdr))

#define __UPORTSYS_CMSG_DATA(cmsg)                                          \
(((unsigned char *) (cmsg)) + sizeof(struct __uportsys_cmsghdr))

#define __UPORTSYS_CMSG_FIRSTHDR(mhdr)                                      \
((mhdr)->controllen >= sizeof(struct __uportsys_cmsghdr) ?                  \
  ((struct __uportsys_cmsghdr *) ((mhdr)->control)) :                       \
  ((struct __uportsys_cmsghdr *) 0))

#define __UPORTSYS_CMSG_NXTHDR(mhdr, cmsg)                                  \
(__UPORTSYS_CMSG_NEXT_DATA(cmsg) < __UPORTSYS_CMSG_END(mhdr) ?              \
  ((struct __uportsys_cmsg *) __UPORTSYS_CMSG_NEXT(cmsg)) :                 \
  ((struct __uportsys_cmsg *) 0))

#endif

#ifndef ___UPORTSYS_MSG_XXX
#define ___UPORTSYS_MSG_XXX
#define __UPORTSYS_MSG_CTRUNC           (1 << 0)
#define __UPORTSYS_MSG_DONTROUTE        (1 << 1)
#define __UPORTSYS_MSG_EOR              (1 << 2)
#define __UPORTSYS_MSG_OOB              (1 << 3)
#define __UPORTSYS_MSG_PEEK             (1 << 4)
#define __UPORTSYS_MSG_TRUNC            (1 << 5)
#define __UPORTSYS_MSG_WAITALL          (1 << 6)
#endif

#ifndef ___UPORTSYS_SCM_XXX
#define ___UPORTSYS_SCM_XXX
#define __UPORTSYS_SCM_RIGHTS           1
#endif

#ifndef ___UPORTSYS_SHUT_XXX
#define ___UPORTSYS_SHUT_XXX
#define __UPORTSYS_SHUT_RD              0
#define __UPORTSYS_SHUT_WR              1
#define __UPORTSYS_SHUT_RDWR            2
#endif

#ifndef ___UPORTSYS_SO_XXX
#define ___UPORTSYS_SO_XXX
#define __UPORTSYS_SO_ACCEPTCONN        1
#define __UPORTSYS_SO_BROADCAST         2
#define __UPORTSYS_SO_DEBUG             3
#define __UPORTSYS_SO_DONTROUTE         4
#define __UPORTSYS_SO_ERROR             5
#define __UPORTSYS_SO_KEEPALIVE         6
#define __UPORTSYS_SO_LINGER            7
#define __UPORTSYS_SO_OOBINLINE         8
#define __UPORTSYS_SO_RCVBUF            9
#define __UPORTSYS_SO_RCVLOWAT          10
#define __UPORTSYS_SO_RCVTIMEO          11
#define __UPORTSYS_SO_REUSEADDR         12
#define __UPORTSYS_SO_SNDBUF            13
#define __UPORTSYS_SO_SNDLOWAT          14
#define __UPORTSYS_SO_SNDTIMEO          15
#define __UPORTSYS_SO_TYPE              16
#endif

#ifndef ___UPORTSYS_SOCK_XXX
#define ___UPORTSYS_SOCK_XXX
#define __UPORTSYS_SOCK_STREAM          1
#define __UPORTSYS_SOCK_DGRAM           2
#define __UPORTSYS_SOCK_SEQPACKET       5
#endif

#ifndef ___UPORTSYS_SOL_XXX
#define ___UPORTSYS_SOL_XXX
#define __UPORTSYS_SOL_SOCKET           0xffff
#endif

#ifndef __UPORTSYS_SA_DATA_LEN
#ifdef __UPORTSYS_SUN_PATH_LEN
#define __UPORTSYS_SA_DATA_LEN          __UPORTSYS_SUN_PATH_LEN
#else
#ifdef __UPORTSYS_NAME_MAX
#define __UPORTSYS_SA_DATA_LEN          (__UPORTSYS_NAME_MAX + 1)
#else
#define __UPORTSYS_SA_DATA_LEN          256
#endif
#endif
#endif

/* System macros for the sys/stat.h header. */

#ifndef __UPORTSYS_S_IXXX_FILE_TYPE
#define __UPORTSYS_S_IXXX_FILE_TYPE
#define __UPORTSYS_S_IFIFO              0010000
#define __UPORTSYS_S_IFCHR              0020000
#define __UPORTSYS_S_IFDIR              0040000
#define __UPORTSYS_S_IFBLK              0060000
#define __UPORTSYS_S_IFREG              0100000
#define __UPORTSYS_S_IFLNK              0120000
#endif

#ifndef __UPORTSYS_S_IFMT
#define __UPORTSYS_S_IFMT               (__UPORTSYS_S_IFBLK | __UPORTSYS_S_IFCHR | __UPORTSYS_S_IFIFO | __UPORTSYS_S_IFDIR | __UPORTSYS_S_IFLNK)
#endif

#ifndef __UPORTSYS_S_IXXX_FILE_MODE
#define __UPORTSYS_S_IXXX_FILE_MODE
#define __UPORTSYS_S_ISUID              0004000
#define __UPORTSYS_S_ISGID              0002000
#define __UPORTSYS_S_ISVTX              0001000
#define __UPORTSYS_S_IRUSR              0000400
#define __UPORTSYS_S_IWUSR              0000200
#define __UPORTSYS_S_IXUSR              0000100
#define __UPORTSYS_S_IRGRP              0000040
#define __UPORTSYS_S_IWGRP              0000020
#define __UPORTSYS_S_IXGRP              0000010
#define __UPORTSYS_S_IROTH              0000004
#define __UPORTSYS_S_IWOTH              0000002
#define __UPORTSYS_S_IXOTH              0000001
#endif

/* System macros for the sys/statvfs.h header. */

#ifndef ___UPORTSYS_ST_XXX
#define ___UPORTSYS_ST_XXX
#define __UPORTSYS_ST_RDONLY            (1 << 0)
#define __UPORTSYS_ST_NOSUID            (1 << 1)
#endif

/* System macros for the sys/un.h header */

#ifndef __UPORTSYS_SUN_PATH_LEN
#ifdef __UPORTSYS_NAME_MAX
#define __UPORTSYS_SUN_PATH_LEN         (__UPORTSYS_NAME_MAX + 1)
#else
#define __UPORTSYS_SUN_PATH_LEN         256
#endif
#endif

/* System macros for the sys/time.h header. */

#ifndef __UPORTSYS_FD_SETSIZE
#ifdef __UPORTSYS_OPEN_MAX
#define __UPORTSYS_FD_SETSIZE           __UPORTSYS_OPEN_MAX
#else
#define __UPORTSYS_FD_SETSIZE           256
#endif
#endif

#ifndef __UPORTSYS_NFDBITS
#define __UPORTSYS_NFDBITS              (sizeof(long) * 8)
#endif

#ifndef __UPORTSYS_FDS_BITS_LEN
#define __UPORTSYS_FDS_BITS_LEN         ((__UPORTSYS_FD_SETSIZE + __UPORTSYS_NFDBITS - 1) / __UPORTSYS_NFDBITS)
#endif

#ifndef ___UPORTSYS_FD_XXX
#define ___UPORTSYS_FD_XXX

#ifndef __UPORTSYS_FD_INDEX
#define __UPORTSYS_FD_INDEX(fd)         ((fd) / __UPORTSYS_NFDBITS)
#endif

#ifndef __UPORTSYS_FD_MASK
#define __UPORTSYS_FD_MASK(fd)          (1L << ((fd) % __UPORTSYS_NFDBITS))
#endif

#ifndef __UPORTSYS_FD_ZERO_MASK
#define __UPORTSYS_FD_ZERO_MASK         0L
#endif

#define __UPORTSYS_FD_CLR(fd, set)                                          \
do {                                                                        \
  (set)->fds_bits[__UPORTSYS_FD_INDEX(fd)] &= ~__UPORTSYS_FD_MASK(fd);      \
} while(0)

#define __UPORTSYS_FD_ISSET(fd, set)                                        \
(((set)->fds_bits[__UPORTSYS_FD_INDEX(fd)] & __UPORTSYS_FD_MASK(fd)) != 0L)

#define __UPORTSYS_FD_SET(fd, set)                                          \
do {                                                                        \
  (set)->fds_bits[__UPORTSYS_FD_INDEX(fd)] |= __UPORTSYS_FD_MASK(fd);       \
} while(0)

#define __UPORTSYS_FS_ZERO(set)                                             \
do {                                                                        \
  unsigned __i = 0;                                                         \
  for(__i = 0; __i < __UPORTSYS_FDS_BITS_LEN; __i++) {                      \
    (set)->fds_bits[__i] = __UPORTSYS_FD_ZERO_MASK;                           \
  }                                                                         \
} while(0)

#endif

#ifndef ___UPORTSYS_ITIMER_XXX
#define ___UPORTSYS_ITIMER_XXX
#define __UPORTSYS_ITIMER_REAL          0
#define __UPORTSYS_ITIMER_VIRTUAL       1
#define __UPORTSYS_ITIMER_PROF          2
#endif

/* System macros for the sys/utsname.h header. */

#ifndef ___UPORTSYS_UTSNAME_XXX_LEN
#define ___UPORTSYS_UTSNAME_XXX_LEN
#ifndef __UPORTSYS_UTSNAME_XXX_LEN
#define __UPORTSYS_UTSNAME_XXX_LEN      32
#endif
#define __UPORTSYS_UTSNAME_SYSNAME_LEN  __UPORTSYS_UTSNAME_XXX_LEN
#define __UPORTSYS_UTSNAME_NODENAME_LEN __UPORTSYS_UTSNAME_XXX_LEN
#define __UPORTSYS_UTSNAME_RELEASE_LEN  __UPORTSYS_UTSNAME_XXX_LEN
#define __UPORTSYS_UTSNAME_VERSION_LEN  __UPORTSYS_UTSNAME_XXX_LEN
#define __UPORTSYS_UTSNAME_MACHINE_LEN  __UPORTSYS_UTSNAME_XXX_LEN
#endif

/* System macros for the sys/wait.h header. */

#ifndef ___UPORTSYS_P_XXX
#define ___UPORTSYS_P_XXX
#define __UPORTSYS_P_ALL                0
#define __UPORTSYS_P_PID                1
#define __UPORTSYS_P_PGID               2
#endif

#ifndef ___UPORTSYS_WXXX_WAIT_OPTION
#define ___UPORTSYS_WXXX_WAIT_OPTION
#define __UPORTSYS_WEXITED              (1 << 0)
#define __UPORTSYS_WCONTINUED           (1 << 1)
#define __UPORTSYS_WNOHANG              (1 << 2)
#define __UPORTSYS_WNOWAIT              (1 << 3)
#define __UPORTSYS_WSTOPPED             (1 << 4)
#define __UPORTSYS_WUNTRACED            (1 << 5)
#endif

#ifndef ___UPORTSYS_WXXX_WAIT_STATUS
#define ___UPORTSYS_WXXX_WAIT_STATUS
#define __UPORTSYS_WEXITSTATUS(status)  ((status) >> 2)
#define __UPORTSYS_WIFCONTINUED(status) (((status) & 3) == 0)
#define __UPORTSYS_WIFEXITED(status)    (((status) & 3) == 1)
#define __UPORTSYS_WIFSIGNALED(status)  (((status) & 3) == 2)
#define __UPORTSYS_WIFSTOPPED(status)   (((status) & 3) == 3)
#define __UPORTSYS_WSTOPSIG(status)     ((status) >> 2)
#define __UPORTSYS_WTERMSIG(status)     ((status) >> 2)
#endif

/* System macros for the fcntl.h header. */

#ifndef ___UPORTSYS_F_XXX_COMMAND
#define ___UPORTSYS_F_XXX_COMMAND
#define __UPORTSYS_F_DUPFD              0
#define __UPORTSYS_F_GETFD              1
#define __UPORTSYS_F_SETFD              2
#define __UPORTSYS_F_GETFL              3
#define __UPORTSYS_F_SETFL              6
#define __UPORTSYS_F_GETLK              7
#define __UPORTSYS_F_SETLK              8
#define __UPORTSYS_F_SETLKW             9
#endif

#ifndef ___UPORTSYS_FD_XXX
#define ___UPORTSYS_FD_XXX
#define __UPORTSYS_FD_CLOEXEC           1
#endif

#ifndef ___UPORTSYS_F_XXX_LOCK_TYPE
#define ___UPORTSYS_F_XXX_LOCK_TYPE
#define __UPORTSYS_F_RDLCK              0
#define __UPORTSYS_F_UNLCK              1
#define __UPORTSYS_F_WRLCK              2
#endif

#ifndef ___UPORTSYS_O_XXX_ACCESS_MODE
#define ___UPORTSYS_O_XXX_ACCESS_MODE
#define __UPORTSYS_O_RDONLY             00000
#define __UPORTSYS_O_WRONLY             00001
#define __UPORTSYS_O_RDWR               00002
#endif

#ifndef __UPORTSYS_O_ACCMODE
#define __UPORTSYS_O_ACCMODE            (__UPORTSYS_O_RDONLY | __UPORTSYS_O_WRONLY | __UPORTSYS_O_RDWR)
#endif

#ifndef ___UPORTSYS_O_XXX_OPEN_FLAGS
#define ___UPORTSYS_O_XXX_OPEN_FLAGS
#define __UPORTSYS_O_CREAT              00004
#define __UPORTSYS_O_EXCL               00010
#define __UPORTSYS_O_NOCTTY             00020
#define __UPORTSYS_O_TRUNC              00040
#define __UPORTSYS_O_APPEND             00100
#define __UPORTSYS_O_DSYNC              00200
#define __UPORTSYS_O_NONBLOCK           00400
#define __UPORTSYS_O_RSYNC              01000
#define __UPORTSYS_O_SYNC               02000
#endif

/* System macros for the limits.h header. */

#ifndef __UPORTSYS_ARG_MAX
#define __UPORTSYS_ARG_MAX              131072
#endif

#ifndef __UPORTSYS_CHILD_MAX
#define __UPORTSYS_CHILD_MAX            2147483647
#endif

#ifndef __UPORTSYS_IOV_MAX
#define __UPORTSYS_IOV_MAX              1024
#endif

#ifndef __UPORTSYS_LINK_MAX
#define __UPORTSYS_LINK_MAX             32767
#endif

#ifndef __UPORTSYS_MAX_CANON
#define __UPORTSYS_MAX_CANON            255
#endif

#ifndef __UPORTSYS_MAX_INPUT
#define __UPORTSYS_MAX_INPUT            255
#endif

#ifndef __UPORTSYS_NAME_MAX
#define __UPORTSYS_NAME_MAX             255
#endif

#ifndef __UPORTSYS_NGROUPS_MAX
#define __UPORTSYS_NGROUPS_MAX          1024
#endif

#ifndef __UPORTSYS_OPEN_MAX
#define __UPORTSYS_OPEN_MAX             256
#endif

#ifndef __UPORTSYS_PATH_MAX
#define __UPORTSYS_PATH_MAX             1024
#endif

#ifndef __UPORTSYS_PIPE_BUF
#define __UPORTSYS_PIPE_BUF             1024
#endif

/* System macros for the poll.h header. */

#ifndef ___UPORTSYS_POLLXXX
#define ___UPORTSYS_POLLXXX
#define __UPORTSYS_POLLIN               (1 << 0)
#define __UPORTSYS_POLLPRI              (1 << 1)
#define __UPORTSYS_POLLRDBAND           (1 << 2)
#define __UPORTSYS_POLLRDNORM           (1 << 3)
#define __UPORTSYS_POLLOUT              (1 << 4)
#define __UPORTSYS_POLLWRBAND           (1 << 5)
#define __UPORTSYS_POLLWRNORM           (1 << 6)
#define __UPORTSYS_POLLERR              (1 << 7)
#define __UPORTSYS_POLLHUP              (1 << 8)
#define __UPORTSYS_POLLNVAL             (1 << 9)
#endif

/* System macros for the signal.h header. */

#ifndef ___UPORTSYS_SA_XXX
#define ___UPORTSYS_SA_XXX
#define __UPORTSYS_SA_NOCLDSTOP         (1 << 0)
#define __UPORTSYS_SA_NOCLDWAIT         (1 << 1)
#define __UPORTSYS_SA_NODEFER           (1 << 2)
#define __UPORTSYS_SA_ONSTACK           (1 << 3)
#define __UPORTSYS_SA_RESETHAND         (1 << 4)
#define __UPORTSYS_SA_RESTART           (1 << 5)
#define __UPORTSYS_SA_SIGINFO           (1 << 6)
#endif

#ifndef ___UPORTSYS_SIG_XXX_SIGNAL_HANDLER
#define ___UPORTSYS_SIG_XXX_SIGNAL_HANDLER
#define __UPORTSYS_SIG_DFL              ((void (*)(int)) 0)
#define __UPORTSYS_SIG_IGN              ((void (*)(int)) 1)
#define __UPORTSYS_SIG_HOLD             ((void (*)(int)) 2)
#define __UPORTSYS_SIG_ERR              ((void (*)(int)) (-1))
#endif

#ifndef ___UPORTSYS_SIG_XXX_SIGPROCMASK_HOW
#define ___UPORTSYS_SIG_XXX_SIGPROCMASK_HOW
#define __UPORTSYS_SIG_BLOCK            0
#define __UPORTSYS_SIG_SETMASK          1
#define __UPORTSYS_SIG_UNBLOCK          2
#endif

#ifndef ___UPORTSYS_SIGXXX
#define ___UPORTSYS_SIGXXX
#define __UPORTSYS_SIGHUP               1
#define __UPORTSYS_SIGINT               2
#define __UPORTSYS_SIGQUIT              3
#define __UPORTSYS_SIGILL               4
#define __UPORTSYS_SIGTRAP              5
#define __UPORTSYS_SIGABRT              6
#define __UPORTSYS_SIGBUS               7
#define __UPORTSYS_SIGFPE               8
#define __UPORTSYS_SIGKILL              9
#define __UPORTSYS_SIGUSR1              10
#define __UPORTSYS_SIGSEGV              11
#define __UPORTSYS_SIGUSR2              12
#define __UPORTSYS_SIGPIPE              13
#define __UPORTSYS_SIGALRM              14
#define __UPORTSYS_SIGTERM              15
#define __UPORTSYS_SIGCHLD              17
#define __UPORTSYS_SIGCONT              18
#define __UPORTSYS_SIGSTOP              19
#define __UPORTSYS_SIGTSTP              20
#define __UPORTSYS_SIGTTIN              21
#define __UPORTSYS_SIGTTOU              22
#define __UPORTSYS_SIGURG               23
#define __UPORTSYS_SIGXCPU              24
#define __UPORTSYS_SIGXFSZ              25
#define __UPORTSYS_SIGVTALRM            26
#define __UPORTSYS_SIGPROF              27
#define __UPORTSYS_SIGPOLL              29
#define __UPORTSYS_SIGSYS               31
#endif

#ifndef __UPORTSYS_SIGRTMIN
#define __UPORTSYS_SIGRTMIN             32
#endif

#ifndef __UPORTSYS_SIGRTMAX
#define __UPORTSYS_SIGRTMAX             64
#endif

#ifndef __UPORTSYS_NSIGBITS
#define __UPORTSYS_NSIGBITS             (sizeof(long) * 8)
#endif

#ifndef __UPORTSYS_SIGS_BITS_LEN
#define __UPORTSYS_SIGS_BITS_LEN        ((__UPORTSYS_SIGRTMAX + __UPORTSYS_NSIGBITS - 2) / __UPORTSYS_NSIGBITS)
#endif

#ifndef __UPORTSYS_MINSIGSTKSZ
#define __UPORTSYS_MINSIGSTKSZ          2048
#endif

#ifndef __UPORTSYS_SIGSTKSZ
#define __UPORTSYS_SIGSTKSZ             8192
#endif

#ifndef ___UPORTSYS_SS_XXX
#define ___UPORTSYS_SS_XXX
#define __UPORTSYS_SS_ONSTACK           (1 << 0)
#define __UPORTSYS_SS_DISABLE           (1 << 1)
#endif

#ifndef ___UPORTSYS_ILL_XXX
#define ___UPORTSYS_ILL_XXX
#define __UPORTSYS_ILL_ILLOPC           1
#define __UPORTSYS_ILL_ILLOPN           2
#define __UPORTSYS_ILL_ILLADR           3
#define __UPORTSYS_ILL_ILLTRP           4
#define __UPORTSYS_ILL_PRVOPC           5
#define __UPORTSYS_ILL_PRVREG           6
#define __UPORTSYS_ILL_COPROC           7
#define __UPORTSYS_ILL_BADSTK           8
#endif

#ifndef ___UPORTSYS_FPE_XXX
#define ___UPORTSYS_FPE_XXX
#define __UPORTSYS_FPE_INTDIV           1
#define __UPORTSYS_FPE_INTOVF           2
#define __UPORTSYS_FPE_FLTDIV           3
#define __UPORTSYS_FPE_FLTOVF           4
#define __UPORTSYS_FPE_FLTUND           5
#define __UPORTSYS_FPE_FLTRES           6
#define __UPORTSYS_FPE_FLTINV           7
#define __UPORTSYS_FPE_FLTSUB           8
#endif

#ifndef ___UPORTSYS_SEGV_XXX
#define ___UPORTSYS_SEGV_XXX
#define __UPORTSYS_SEGV_MAPERR          1
#define __UPORTSYS_SEGV_ACCERR          2
#endif

#ifndef ___UPORTSYS_BUS_XXX
#define ___UPORTSYS_BUS_XXX
#define __UPORTSYS_BUS_ADRALN           1
#define __UPORTSYS_BUS_ADRERR           2
#define __UPORTSYS_BUS_OBJERR           3
#endif

#ifndef ___UPORTSYS_TRAP_XXX
#define ___UPORTSYS_TRAP_XXX
#define __UPORTSYS_TRAP_BRKPT           1
#define __UPORTSYS_TRAP_TRACE           2
#endif

#ifndef ___UPORTSYS_CLD_XXX
#define ___UPORTSYS_CLD_XXX
#define __UPORTSYS_CLD_EXITED           1
#define __UPORTSYS_CLD_KILLED           2
#define __UPORTSYS_CLD_DUMPED           3
#define __UPORTSYS_CLD_TRAPPED          4
#define __UPORTSYS_CLD_STOPPED          5
#define __UPORTSYS_CLD_CONTINUED        6
#endif

#ifndef ___UPORTSYS_POLL_XXX
#define ___UPORTSYS_POLL_XXX
#define __UPORTSYS_POLL_IN              1
#define __UPORTSYS_POLL_OUT             2
#define __UPORTSYS_POLL_MSG             3
#define __UPORTSYS_POLL_ERR             4
#define __UPORTSYS_POLL_PRI             5
#define __UPORTSYS_POLL_HUP             6
#endif

#ifndef ___UPORTSYS_SI_XXX
#define ___UPORTSYS_SI_XXX
#define __UPORTSYS_SI_USER              0x11
#define __UPORTSYS_SI_QUEUE             0x12
#define __UPORTSYS_SI_TIMER             0x13
#define __UPORTSYS_SI_ASYNCIO           0x14
#define __UPORTSYS_SI_MESGQ             0x15
#endif

#ifndef ___UPORTSYS__SIGXXX
#define ___UPORTSYS__SIGXXX

#ifndef __UPORTSYS__sig_index
#define __UPORTSYS__sig_index(sig_num)  (((sig_num) - 1) / __UPORTSYS_NSIGBITS)
#endif

#ifndef __UPORTSYS__sig_mask
#define __UPORTSYS__sig_mask(sig_num)   (1L << (((sig_num) - 1) % __UPORTSYS_NSIGBITS))
#endif

#ifndef __UPORTSYS__sig_empty_mask
#define __UPORTSYS__sig_empty_mask      0L
#endif

#ifndef __UPORTSYS__sig_fill_mask
#define __UPORTSYS__sig_fill_mask       (~0L)
#endif

#define __UPORTSYS__sigaddset(set, sig_num)                                 \
do {                                                                        \
  (set)->sigs_bits[__UPORTSYS__sig_index(sig_num)] |=                       \
    __UPORTSYS__sig_mask(sig_num);                                          \
} while(0)

#define __UPORTSYS__sigdelset(set, sig_num)                                 \
do {                                                                        \
  (set)->sigs_bits[__UPORTSYS__sig_index(sig_num)] &=                       \
    ~__UPORTSYS__sig_mask(sig_num);                                         \
} while(0)

#define __UPORTSYS__sigemptyset(set)                                        \
do {                                                                        \
  unsigned __i;                                                             \
  for(__i = 0; __i < __UPORTSYS_SIGS_BITS_LEN; __i++) {                     \
    (set)->sigs_bits[__i] = __UPORTSYS__sig_empty_mask;                     \
  }                                                                         \
} while(0)

#define __UPORTSYS__setfillset(set)                                         \
do {                                                                        \
  unsigned __i;                                                             \
  for(__i = 0; __i < __UPORTSYS_SIGS_BITS_LEN; __i++) {                     \
    (set)->sigs_bits[__i] = __UPORTSYS__sig_empty_mask;                     \
  }                                                                         \
} while(0)

#define __UPORTSYS__setismember(set, sig_num)                               \
(((set)->sigs_bits[__UPORTSYS__sig_index(sig_num)] &                        \
  __UPORTSYS__sig_mask(sig_num)) != 0)

#endif

/* System macros for the termios.h header. */

#ifndef __UPORTSYS_NCCS
#define __UPORTSYS_NCCS                 11
#endif

#ifndef ___UPORTSYS_XXX_TERMIOS_C_IFLAG
#define ___UPORTSYS_XXX_TERMIOS_C_IFLAG
#define __UPORTSYS_BRKINT               (1 << 0)
#define __UPORTSYS_ICRNL                (1 << 1)
#define __UPORTSYS_IGNBRK               (1 << 2)
#define __UPORTSYS_IGNCR                (1 << 3)
#define __UPORTSYS_IGNPAR               (1 << 4)
#define __UPORTSYS_INLCR                (1 << 5)
#define __UPORTSYS_INPCK                (1 << 6)
#define __UPORTSYS_ISTRIP               (1 << 7)
#define __UPORTSYS_IUCLC                (1 << 8)
#define __UPOPTSYS_IXANY                (1 << 9)
#define __UPORTSYS_IXOFF                (1 << 10)
#define __UPORTSYS_IXON                 (1 << 11)
#define __UPORTSYS_PARMRK               (1 << 12)
#endif

#ifndef ___UPORTSYS_XXX_TERMIOS_C_OFLAG
#define ___UPORTSYS_XXX_TERMIOS_C_OFLAG
#define __UPORTSYS_OPOST                (1 << 0)
#define __UPORTSYS_OLCUC                (1 << 1)
#define __UPORTSYS_ONLCR                (1 << 2)
#define __UPORTSYS_OCRNL                (1 << 3)
#define __UPORTSYS_ONOCR                (1 << 4)
#define __UPORTSYS_ONLRET               (1 << 5)
#define __UPORTSYS_OFILL                (1 << 6)
#define __UPORTSYS_NLDLY                (1 << 7)
#define __UPORTSYS_NL0                  (0 << 7)
#define __UPORTSYS_NL1                  (1 << 7)
#define __UPORTSYS_CRDLY                (3 << 8)
#define __UPORTSYS_CR0                  (0 << 8)
#define __UPORTSYS_CR1                  (1 << 8)
#define __UPORTSYS_CR2                  (2 << 8)
#define __UPORTSYS_CR3                  (3 << 8)
#define __UPORTSYS_TABDLY               (3 << 10)
#define __UPORTSYS_TAB0                 (0 << 10)
#define __UPORTSYS_TAB1                 (1 << 10)
#define __UPORTSYS_TAB2                 (2 << 10)
#define __UPORTSYS_TAB3                 (3 << 10)
#define __UPORTSYS_BSDLY                (1 << 12)
#define __UPORTSYS_BS0                  (0 << 12)
#define __UPORTSYS_BS1                  (1 << 12)
#define __UPORTSYS_VTDLY                (1 << 13)
#define __UPORTSYS_VT0                  (0 << 13)
#define __UPORTSYS_VT1                  (1 << 13)
#define __UPORTSYS_FFDLY                (1 << 14)
#define __UPORTSYS_FF0                  (0 << 14)
#define __UPORTSYS_FF1                  (1 << 14)
#endif

#ifndef ___UPORTSYS_XXX_TERMIOS_C_CFLAG
#define ___UPORTSYS_XXX_TERMIOS_C_CFLAG
#define __UPORTSYS_CSIZE                (3 << 0)
#define __UPORTSYS_CS5                  (0 << 0)
#define __UPORTSYS_CS6                  (1 << 0)
#define __UPORTSYS_CS7                  (2 << 0)
#define __UPORTSYS_CS8                  (3 << 0)
#define __UPORTSYS_CSTOPB               (1 << 2)
#define __UPORTSYS_CREAD                (1 << 3)
#define __UPORTSYS_PARENB               (1 << 4)
#define __UPORTSYS_PARODD               (1 << 5)
#define __UPORTSYS_HUPCL                (1 << 6)
#define __UPORTSYS_CLOCAL               (1 << 7)
#endif

#ifndef ___UPORTSYS_XXX_TERMIOS_C_LFLAG
#define ___UPORTSYS_XXX_TERMIOS_C_LFLAG
#define __UPORTSYS_ECHO                 (1 << 0)
#define __UPORTSYS_ECHOE                (1 << 1)
#define __UPORTSYS_ECHOK                (1 << 2)
#define __UPORTSYS_ECHONL               (1 << 3)
#define __UPORTSYS_ICANON               (1 << 4)
#define __UPORTSYS_IEXTEN               (1 << 5)
#define __UPORTSYS_ISIG                 (1 << 6)
#define __UPORTSYS_NOFLSH               (1 << 7)
#define __UPORTSYS_TOSTOP               (1 << 8)
#define __UPORTSYS_XCASE                (1 << 9)
#endif

#ifndef ___UPORTSYS_BXXX
#define ___UPORTSYS_BXXX
#define __UPORTSYS_B0                   0
#define __UPORTSYS_B50                  1
#define __UPORTSYS_B75                  2
#define __UPORTSYS_B110                 3
#define __UPORTSYS_B134                 4
#define __UPORTSYS_B150                 5
#define __UPORTSYS_B200                 6
#define __UPORTSYS_B300                 7
#define __UPORTSYS_B600                 10
#define __UPORTSYS_B1200                11
#define __UPORTSYS_B1800                12
#define __UPORTSYS_B2400                13
#define __UPORTSYS_B4800                14
#define __UPORTSYS_B9600                15
#define __UPORTSYS_B19200               16
#define __UPORTSYS_B38400               17
#endif

#ifndef ___UPORTSYS_VXXX
#define ___UPORTSYS_VXXX
#define __UPORTSYS_VEOF                 0
#define __UPORTSYS_VEOL                 1
#define __UPORTSYS_VERASE               2
#define __UPORTSYS_VINTR                3
#define __UPORTSYS_VKILL                4
#define __UPORTSYS_VMIN                 5
#define __UPORTSYS_VQUIT                6
#define __UPORTSYS_VSTART               7
#define __UPORTSYS_VSTOP                8
#define __UPORTSYS_VSUSP                9
#define __UPORTSYS_VTIME                10
#endif

#ifndef ___UPORTSYS_TCSAXXX
#define ___UPORTSYS_TCSAXXX
#define __UPORTSYS_TCSANOW              0
#define __UPORTSYS_TCSADRAIN            1
#define __UPORTSYS_TCSAFLUSH            2
#endif

#ifndef ___UPORTSYS_TCXXX_TCFLOW_ACTION
#define ___UPORTSYS_TCXXX_TCFLOW_ACTION
#define __UPORTSYS_TCOOFF               0
#define __UPORTSYS_TCOON                1
#define __UPORTSYS_TCIOFF               2
#define __UPORTSYS_TCION                3
#endif

#ifndef ___UPORTSYS_TCXXX_TCFLOW_QUEUE_SELECTOR
#define ___UPORTSYS_TCXXX_TCFLOW_QUEUE_SELECTOR
#define __UPORTSYS_TCIFLUSH             0
#define __UPORTSYS_TCOFLUSH             1
#define __UPORTSYS_TCIOFLUSH            2
#endif

#ifndef ___UPORTSYS__CFXXX
#define ___UPORTSYS__CFXXX

#define __UPORTSYS__cfgetispeed(termios)                                    \
((termios)->c_ispeed)

#define __UPORTSYS__cfgetospeed(termios)                                    \
((termios)->c_ospeed)

#define __UPORTSYS__cfsetispeed(termios, speed)                             \
do { (termsios)->c_ispeed = (speed) } while(0)

#define __UPORTSYS__cfsetospeed(termios, speed)                             \
do { (termsios)->c_ospeed = (speed) } while(0)

#endif

/* System macros for the unistd.h header. */

#ifndef ___UPORTSYS_X_OK
#define ___UPORTSYS_X_OK
#define __UPORTSYS_R_OK                 (1 << 2)
#define __UPORTSYS_W_OK                 (1 << 1)
#define __UPORTSYS_X_OK                 (1 << 0)
#define __UPORTSYS_F_OK                 0
#endif

#ifndef ___UPORTSYS_SEEK_XXX
#define ___UPORTSYS_SEEK_XXX
#define __UPORTSYS_SEEK_SET             0
#define __UPORTSYS_SEEK_CUR             1
#define __UPORTSYS_SEEK_END             2
#endif

/* Other system macros for directories. */

#ifndef __UPORTSYS_NEXT_DIRENT_OFFSET

#ifndef __UPORTSYS_DIRENT_ALIGN
#define __UPORTSYS_DIRENT_ALIGN(x)      (((x) + (sizeof(long) - 1)) & (sizeof(long)))
#endif

#define __UPORTSYS_NEXT_DIRENT_OFFSET(offset, dirent)                       \
((offset) + offsetof(dirent, d_name) +                                      \
  __UPORTSYS_DIRENT_ALIGN(strlen((dir)->d_name) + 1))

#endif

/* Other system macros for directories. */

#ifndef __UPORTSYS_THREAD_MUTEX
#define __UPORTSYS_THREAD_MUTEX         { 0 }
#endif

/*
 * System types.
 */
 
/* System types for the sys/types.h header. */

#ifdef __UPORTSYS_BLKCNT_T
typedef __UPORTSYS_BLKCNT_T __uportsys_blkcnt_t;
#else
typedef __uportsys_int64_t __uportsys_blkcnt_t;
#endif

#ifdef __UPORTSYS_BLKSIZE_T
typedef __UPORTSYS_BLKSIZE_T __uportsys_blksize_t;
#else
typedef long __uportsys_blksize_t;
#endif

#ifdef __UPORTSYS_CLOCK_T
typedef __UPORTSYS_CLOCK_T __uportsys_clock_t;
#else
typedef long __uportsys_clock_t;
#endif

#ifdef __UPORTSYS_DEV_T
typedef __UPORTSYS_DEV_T __uportsys_dev_t;
#else
typedef unsigned __uportsys_dev_t;
#endif

#ifdef __UPORTSYS_FSBLKCNT_T
typedef __UPORTSYS_FSBLKCNT_T __uportsys_fsblkcnt_t;
#else
typedef __uportsys_uint64_t __uportsys_fsblkcnt_t;
#endif

#ifdef __UPORTSYS_FSFILCNT_T
typedef __UPORTSYS_FSFILCNT_T __uportsys_fsfilcnt_t;
#else
typedef __uportsys_uint64_t __uportsys_fsfilcnt_t;
#endif

#ifdef __UPORTSYS_ID_T
typedef __UPORTSYS_ID_T __uportsys_id_t;
#else
typedef int __uportsys_id_t;
#endif

#ifdef __UPORTSYS_INO_T
typedef __UPORTSYS_INO_T __uportsys_ino_t;
#else
typedef __uportsys_uint64_t __uportsys_ino_t;
#endif

#ifdef __UPORTSYS_MODE_T
typedef __UPORTSYS_MODE_T __uportsys_mode_t;
#else
typedef unsigned short __uportsys_mode_t;
#endif

#ifdef __UPORTSYS_NLINK_T
typedef __UPORTSYS_NLINK_T __uportsys_nlink_t;
#else
typedef unsigned short __uportsys_nlink_t;
#endif

#ifdef __UPORTSYS_OFF_T
typedef __UPORTSYS_OFF_T __uportsys_off_t;
#else
typedef __uportsys_int64_t __uportsys_off_t;
#endif

#ifdef __UPORTSYS_SIZE_T
typedef __UPORTSYS_SIZE_T __uportsys_size_t;
#else
typedef __SIZE_TYPE__ __uportsys_size_t;
#endif

#ifdef __UPORTSYS_SSIZE_T
typedef __UPORTSYS_SSIZE_T __uportsys_ssize_t;
#else
typedef long __uportsys_ssize_t;
#endif

#ifdef __UPORTSYS_SUSECONDS_T
typedef __UPORTSYS_SUSECONDS_T __uportsys_suseconds_t;
#else
typedef long __uportsys_suseconds_t;
#endif

#ifdef __UPORTSYS_TIME_T
typedef __UPORTSYS_TIME_T __uportsys_time_t;
#else
typedef long __uportsys_time_t;
#endif

#ifdef __UPORTSYS_USECONDS_T
typedef __UPORTSYS_USECONDS_T __uportsys_useconds_t;
#else
typedef unsigned long __uportsys_useconds_t;
#endif

#ifdef __UPORTSYS_GID_T
typedef __UPORTSYS_GID_T __uportsys_gid_t;
#else
typedef __uportsys_id_t __uportsys_gid_t;
#endif

#ifdef __UPORTSYS_PID_T
typedef __UPORTSYS_PID_T __uportsys_pid_t;
#else
typedef __uportsys_id_t __uportsys_pid_t;
#endif

#ifdef __UPORTSYS_UID_T
typedef __UPORTSYS_UID_T __uportsys_uid_t;
#else
typedef __uportsys_id_t __uportsys_uid_t;
#endif

/* System types for the netinet/in.h header. */

#ifdef __UPORTSYS_IN_ADDR_T
typedef __UPORTSYS_IN_ADDR_T __uportsys_in_addr_t;
#else
typedef __uportsys_uint32_t __uportsys_in_addr_t;
#endif

#ifdef __UPORTSYS_IN_PORT_T
typedef __UPORTSYS_IN_PORT_T __uportsys_in_port_t;
#else
typedef __uportsys_uint16_t __uportsys_in_port_t;
#endif

/* System types for the sys/resource.h header. */

#ifdef __UPORTSYS_RLIM_T
typedef __UPORTSYS_RLIM_T __uportsys_rlim_t;
#else
typedef __uportsys_uint64_t __uportsys_rlim_t;
#endif

/* System types for the sys/socket.h header. */

#ifdef __UPORTSYS_SA_FAMILY_T
typedef __UPORTSYS_SA_FAMILY_T __uportsys_sa_family_t;
#else
typedef unsigned short __uportsys_sa_family_t;
#endif

#ifdef __UPORTSYS_SOCKLEN_T
typedef __UPORTSYS_SOCKLEN_T __uportsys_socklen_t;
#else
typedef __uportsys_uint32_t __uportsys_socklen_t;
#endif

/* System types for the sys/time.h header. */

#ifdef __UPORTSYS_FD_SET_TYPE
typedef __UPORTSYS_FD_SET_TYPE __uportsys_fd_set;
#else
typedef struct
{
  long fds_bits[__UPORTSYS_FDS_BITS_LEN];
} __uportsys_fd_set;
#endif

/* System types for the sys/wait.h header. */

#ifdef __UPORTSYS_IDTYPE_T
typedef __UPORTSYS_IDTYPE_T __uportsys_idtype_t;
#else
typedef int __uportsys_idtype_t; 
#endif

/* System types for the poll.h header. */

#ifdef __UPORTSYS_NFDS_T
typedef __UPORTSYS_NFDS_T __uportsys_nfds_t;
#else
typedef unsigned __uportsys_nfds_t;
#endif

/* System types for the signal.h header. */

#ifdef __UPORTSYS_SIGSET_T
typedef __UPORTSYS_SIGSET_T __uportsys_sigset_t;
#else
typedef struct
{
  long sigs_bits[__UPORTSYS_SIGS_BITS_LEN];
} __uportsys_sigset_t;
#endif

#ifdef __UPORTSYS_STACK_T
typedef __UPORTSYS_STACK_T __uportsys_stack_t;
#else
typedef struct
{
  void *ss_sp;
  __uportsys_size_t ss_size;
  int ss_flags;
} __uportsys_stack_t;
#endif

/* System types for the termios.h header. */

#ifdef __UPORTSYS_CC_T
typedef __UPORTSYS_CC_T __uportsys_cc_t;
#else
typedef unsigned char __uportsys_cc_t;
#endif

#ifdef __UPORTSYS_SPEED_T
typedef __UPORTSYS_SPEED_T __uportsys_speed_t;
#else
typedef unsigned __uportsys_speed_t;
#endif

#ifdef __UPORTSYS_TCFLAG_T
typedef __UPORTSYS_TCFLAG_T __uportsys_tcflag_t;
#else
typedef unsigned __uportsys_tcflag_t;
#endif

/* Other system types for directories. */

#ifdef __UPORTSYS_DIR_T
typedef __UPORTSYS_DIR_T __uportsys_dir_t;
#else
typedef int __uportsys_dir_t;
#endif

/* Other system types for threads. */

#ifdef __UPORTSYS_THREAD_T
typedef __UPORTSYS_THREAD_T __uportsys_thread_t;
#else
typedef __uportsys_pid_t __uportsys_thread_t;
#endif

#ifdef __UPORTSYS_MUTEX_T
typedef __UPORTSYS_MUTEX_T __uportsys_thread_mutex_t;
#else
typedef struct
{
  int lock;
} __uportsys_thread_mutex_t;
#endif

/*
 * System structures and system unions.
 */

/* System structures and system unions for the sys/resource.h header. */

#ifndef __UPORTSYS_STRUCT_TIMEVAL
#define __UPORTSYS_STRUCT_TIMEVAL(name)                                     \
struct name                                                                 \
{                                                                           \
  __uportsys_time_t tv_sec;                                                 \
  __uportsys_suseconds_t tv_usec;                                           \
}
#endif

__UPORTSYS_STRUCT_TIMEVAL(__uportsys_timeval);

#ifndef __UPORTSYS_STRUCT_RLIMIT
#define __UPORTSYS_STRUCT_RLIMIT(name)                                      \
struct name                                                                 \
{                                                                           \
  __uportsys_rlim_t rlim_cur;                                               \
  __uportsys_rlim_t rlim_max;                                               \
}
#endif

__UPORTSYS_STRUCT_RLIMIT(__uportsys_rlimit);

#ifndef __UPORTSYS_STRUCT_RUSAGE
#define __UPORTSYS_STRUCT_RUSAGE(name, timeval_name)                        \
struct name                                                                 \
{                                                                           \
  struct timeval_name ru_utime;                                             \
  struct timeval_name ru_stime;                                             \
}
#endif

__UPORTSYS_STRUCT_RUSAGE(__uportsys_rusage, __uportsys_timeval);

/* System structures and system unions for the sys/socket.h header. */

#ifndef __UPORTSYS_STRUCT_IOVEC
#define __UPORTSYS_STRUCT_IOVEC(name)                                       \
struct name                                                                 \
{                                                                           \
  void *iov_base;                                                           \
  __uportsys_size_t iov_len;                                                \
}
#endif

__UPORTSYS_STRUCT_IOVEC(__uportsys_iovec);

#ifndef __UPORTSYS_STRUCT_CMSGHDR
#define __UPORTSYS_STRUCT_CMSGHDR(name)                                     \
struct name                                                                 \
{                                                                           \
  __uportsys_socklen_t cmsg_len;                                            \
  int cmsg_level;                                                           \
  int cmsg_type;                                                            \
}
#endif

__UPORTSYS_STRUCT_CMSGHDR(__uportsys_cmsghdr);

#ifndef __UPORTSYS_STRUCT_LINGER
#define __UPORTSYS_STRUCT_LINGER(name)                                      \
struct name                                                                 \
{                                                                           \
  int l_onoff;                                                              \
  int l_linger;                                                             \
}
#endif

__UPORTSYS_STRUCT_LINGER(__uportsys_linger);

#ifndef __UPORTSYS_STRUCT_MSGHDR
#define __UPORTSYS_STRUCT_MSGHDR(name, iovec_name)                          \
struct name                                                                 \
{                                                                           \
  void *msg_name;                                                           \
  __uportsys_socklen_t msg_namelen;                                         \
  struct iovec_name msg_iov;                                                \
  int msg_iovlen;                                                           \
  void *msg_control;                                                        \
  __uportsys_socklen_t msg_controllen;                                      \
  int msg_flags;                                                            \
}
#endif

__UPORTSYS_STRUCT_MSGHDR(__uportsys_msghdr, __uportsys_iovec);

#ifndef __UPORTSYS_STRUCT_SOCKADDR
#define __UPORTSYS_STRUCT_SOCKADDR(name)                                    \
struct name                                                                 \
{                                                                           \
  __uportsys_sa_family_t sa_family;                                         \
  char sa_data[__UPORTSYS_SA_DATA_LEN];                                     \
}
#endif

__UPORTSYS_STRUCT_SOCKADDR(__uportsys_sockaddr);

/* System structures and system unions for the sys/stat.h header. */

#ifdef ___UPORTSYS_STRUCT_STAT_XTIM
#ifndef __UPORTSYS_STRUCT_TIMESPEC
#define __UPORTSYS_STRUCT_TIMESPEC(name)                                    \
struct name                                                                 \
{                                                                           \
  __uportsys_time_t tv_sec;                                                 \
  long tv_nsec;                                                             \
}
#endif

__UPORTSYS_STRUCT_TIMESPEC(__uportsys_timespec);
#endif

#ifndef __UPORTSYS_STRUCT_STAT
#ifdef ___UPORTSYS_STRUCT_STAT_XTIM
#define __UPORTSYS_STRUCT_STAT(name, timespec_name)                         \
struct name                                                                 \
{                                                                           \
  __uportsys_dev_t st_dev;                                                  \
  __uportsys_ino_t st_ino;                                                  \
  __uportsys_mode_t st_mode;                                                \
  __uportsys_nlink_t st_nlink;                                              \
  __uportsys_uid_t st_uid;                                                  \
  __uportsys_gid_t st_gid;                                                  \
  __uportsys_dev_t st_rdev;                                                 \
  __uportsys_off_t st_size;                                                 \
  struct timespec_name st_atim;                                             \
  struct timespec_name st_mtim;                                             \
  struct timespec_name st_ctim;                                             \
  __uportsys_blksize_t st_blksize;                                          \
  __uportsys_blkcnt_t st_blocks;                                            \
}
#else
#define __UPORTSYS_STRUCT_STAT(name, timespec_name)                         \
struct name                                                                 \
{                                                                           \
  __uportsys_dev_t st_dev;                                                  \
  __uportsys_ino_t st_ino;                                                  \
  __uportsys_mode_t st_mode;                                                \
  __uportsys_nlink_t st_nlink;                                              \
  __uportsys_uid_t st_uid;                                                  \
  __uportsys_gid_t st_gid;                                                  \
  __uportsys_dev_t st_rdev;                                                 \
  __uportsys_off_t st_size;                                                 \
  __uportsys_time_t st_atime;                                               \
  __uportsys_time_t st_mtime;                                               \
  __uportsys_time_t st_ctime;                                               \
  __uportsys_blksize_t st_blksize;                                          \
  __uportsys_blkcnt_t st_blocks;                                            \
}
#endif
#endif

__UPORTSYS_STRUCT_STAT(__uportsys_stat, __uportsys_timespec);

/* System structures and system unions for the sys/statvfs.h header. */

#ifndef __UPORTSYS_STRUCT_STATVFS
#define __UPORTSYS_STRUCT_STATVFS(name)                                     \
struct name                                                                 \
{                                                                           \
  unsigned long f_bsize;                                                    \
  unsigned long f_frsize;                                                   \
  __uportsys_fsblkcnt_t f_blocks;                                           \
  __uportsys_fsblkcnt_t f_bfree;                                            \
  __uportsys_fsblkcnt_t f_bavail;                                           \
  __uportsys_fsfilcnt_t f_files;                                            \
  __uportsys_fsfilcnt_t f_ffree;                                            \
  __uportsys_fsfilcnt_t f_favail;                                           \
  unsigned long f_fsid;                                                     \
  unsigned long f_flag;                                                     \
  unsigned long f_namemax;                                                  \
}
#endif

__UPORTSYS_STRUCT_STATVFS(__uportsys_statvfs);

/* System structures and system unions for the sys/time.h header. */

#ifndef __UPORTSYS_STRUCT_ITIMERVAL
#define __UPORTSYS_STRUCT_ITIMERVAL(name, timeval_name)                     \
struct name                                                                 \
{                                                                           \
  struct timeval_name it_interval;                                          \
  struct timeval_name it_value;                                             \
}
#endif

__UPORTSYS_STRUCT_ITIMERVAL(__uportsys_itimerval, __uportsys_timeval);

#ifdef __UPORTSYS_STRUCT_TIMEZONE
__UPORTSYS_STRUCT_TIMEZONE(__uportsys_timezone);
#endif

/* System structures and system unions for the sys/times.h header. */

#ifndef __UPORTSYS_STRUCT_TMS
#define __UPORTSYS_STRUCT_TMS(name)                                         \
struct name                                                                 \
{                                                                           \
  __uportsys_clock_t tms_utime;                                             \
  __uportsys_clock_t tms_stime;                                             \
  __uportsys_clock_t tms_cutime;                                            \
  __uportsys_clock_t tms_cstime;                                            \
}
#endif

__UPORTSYS_STRUCT_TMS(__uportsys_tms);

/* System structures and system unions for the sys/un.h header. */

#ifndef __UPORTSYS_STRUCT_SOCKADDR_UN
#define __UPORTSYS_STRUCT_SOCKADDR_UN(name)                                 \
struct name                                                                 \
{                                                                           \
  __uportsys_sa_family_t sun_family;                                        \
  char sun_path[__UPORTSYS_SUN_PATH_LEN];                                   \
}
#endif

__UPORTSYS_STRUCT_SOCKADDR_UN(__uportsys_sockaddr_un);

/* System structures and system unions for the sys/utsname.h header. */

#ifndef __UPORTSYS_STRUCT_UTSNAME
#define __UPORTSYS_STRUCT_UTSNAME(name)                                     \
struct name                                                                 \
{                                                                           \
  char sysname[__UPORTSYS_UTSNAME_SYSNAME_LEN];                             \
  char nodename[__UPORTSYS_UTSNAME_NODENAME_LEN];                           \
  char release[__UPORTSYS_UTSNAME_RELEASE_LEN];                             \
  char version[__UPORTSYS_UTSNAME_VERSION_LEN];                             \
  char machine[__UPORTSYS_UTSNAME_MACHINE_LEN];                             \
}
#endif

__UPORTSYS_STRUCT_UTSNAME(__uportsys_utsname);

/* System structures and system unions for the dirent.h header. */

#ifndef __UPORTSYS_STRUCT_DIRENT
#define __UPORTSYS_STRUCT_DIRENT(name)                                      \
struct name                                                                 \
{                                                                           \
  __uportsys_ino_t d_ino;                                                   \
  char d_name[__UPORTSYS_NAME_MAX + 1];                                     \
}
#endif

__UPORTSYS_STRUCT_DIRENT(__uportsys_dirent);

/* System structures and system unions for the fcntl.h header. */

#ifndef __UPORTSYS_STRUCT_FLOCK
#define __UPORTSYS_STRUCT_FLOCK(name)                                       \
struct name                                                                 \
{                                                                           \
  short l_type;                                                             \
  short l_whence;                                                           \
  __uportsys_off_t l_start;                                                 \
  __uportsys_off_t l_len;                                                   \
  __uportsys_pid_t l_pid;                                                   \
}
#endif

__UPORTSYS_STRUCT_FLOCK(__uportsys_flock);

/* System structures and system unions for the poll.h header. */

#ifndef __UPORTSYS_STRUCT_POLLFD
#define __UPORTSYS_STRUCT_POLLFD(name)                                      \
struct name                                                                 \
{                                                                           \
  int fd;                                                                   \
  short events;                                                             \
  short revents;                                                            \
}
#endif

__UPORTSYS_STRUCT_POLLFD(__uportsys_pollfd);

/* System structures and system unions for the signal.h header. */

#ifndef __UPORTSYS_UNION_SIGVAL
#define __UPORTSYS_UNION_SIGVAL(name)                                       \
union name                                                                  \
{                                                                           \
  int sival_int;                                                            \
  void *sival_ptr;                                                          \
}
#endif

__UPORTSYS_UNION_SIGVAL(__uportsys_sigval);

#ifndef __UPORTSYS_SIGINFO_T
#define __UPORTSYS_SIGINFO_T(sigval_name)                                   \
struct                                                                      \
{                                                                           \
  int si_signo;                                                             \
  int si_errno;                                                             \
  int si_code;                                                              \
  __uportsys_pid_t si_pid;                                                  \
  __uportsys_uid_t si_uid;                                                  \
  void *si_addr;                                                            \
  int si_status;                                                            \
  long si_band;                                                             \
  union sigval_name si_value;                                               \
}
#endif

typedef __UPORTSYS_SIGINFO_T(__uportsys_sigval) __uportsys_siginfo_t;

#ifndef __UPORTSYS_STRUCT_SIGACTION
#define __UPORTSYS_STRUCT_SIGACTION(name, siginfo_t_type)                   \
struct name                                                                 \
{                                                                           \
  void (*sa_handler)(int);                                                  \
  __uportsys_sigset_t sa_mask;                                              \
  int sa_flags;                                                             \
  void (*sa_sigaction)(int, siginfo_t_type *, void *);                      \
}
#endif

__UPORTSYS_STRUCT_SIGACTION(__uportsys_sigaction, __uportsys_siginfo_t);

/* System structures and system unions for the termios.h header. */

#ifndef __UPORTSYS_STRUCT_TERMIOS
#define __UPORTSYS_STRUCT_TERMIOS(name)                                     \
struct name                                                                 \
{                                                                           \
  __uportsys_tcflag_t c_iflag;                                              \
  __uportsys_tcflag_t c_oflag;                                              \
  __uportsys_tcflag_t c_cflag;                                              \
  __uportsys_tcflag_t c_lflag;                                              \
  __uportsys_cc_t c_cc[__UPORTSYS_NCCS];                                    \
  __uportsys_speed_t c_ispeed;                                              \
  __uportsys_speed_t c_ospeed;                                              \
}
#endif

__UPORTSYS_STRUCT_TERMIOS(__uportsys_termios);

/* System structures and system unions for the time.h header. */

#ifndef ___UPORTSYS_STRUCT_STAT_XTIM
#ifndef __UPORTSYS_STRUCT_TIMESPEC
#define __UPORTSYS_STRUCT_TIMESPEC(name)                                    \
struct name                                                                 \
{                                                                           \
  __uportsys_time_t tv_sec;                                                 \
  long tv_nsec;                                                             \
}
#endif

__UPORTSYS_STRUCT_TIMESPEC(__uportsys_timespec);
#endif

/*
 * System functions.
 */

/* System functions for the sys/mman.h header. */

int __uportsys_mlock(const void *addr, __uportsys_size_t len, int *err_num);
int __uportsys_mlockall(int flags, int *err_num);
void *__uportsys_mmap(void *addr, __uportsys_size_t len, int prot, int flags, int fd, __uportsys_off_t offset, int *err_num);
int __uportsys_mprotect(void *addr, __uportsys_size_t len, int prot, int *err_num);
int __uportsys_msync(void *addr, __uportsys_size_t len, int flags, int *err_num);
int __uportsys_munlock(const void *addr, __uportsys_size_t len, int *err_num);
int __uportsys_munlockall(int *err_num);
int __uportsys_munmap(void *addr, __uportsys_size_t len, int *err_num);

/* System functions for the sys/resource.h header. */

int __uportsys_getpriority(int which, __uportsys_id_t who, int *err_num);
int __uportsys_getrlimit(int resource, struct __uportsys_rlimit *rlim, int *err_num);
int __uportsys_getrusage(int who, struct __uportsys_rusage *usage, int *err_num);
int __uportsys_setpriority(int which, __uportsys_id_t who, int prio, int *err_num);
int __uportsys_setrlimit(int resource, const struct __uportsys_rlimit *rlim, int *err_num);

/* System functions for the sys/socket.h header. */

int __uportsys_accept(int sd, struct __uportsys_sockaddr *addr, __uportsys_socklen_t *addr_len, int *err_num);
int __uportsys_bind(int sd, const struct __uportsys_sockaddr *addr, __uportsys_socklen_t addr_len, int *err_num);
int __uportsys_connect(int sd, const struct __uportsys_sockaddr *addr, __uportsys_socklen_t addr_len, int *err_num);
int __uportsys_getpeername(int sd, struct __uportsys_sockaddr *addr, __uportsys_socklen_t *addr_len, int *err_num);
int __uportsys_getsockname(int sd, struct __uportsys_sockaddr *addr, __uportsys_socklen_t *addr_len, int *err_num);
int __uportsys_getsockopt(int sd, int level, int opt_name, void *opt_value, __uportsys_socklen_t *opt_len, int *err_num);
int __uportsys_listen(int sd, int backlog, int *err_num);
__uportsys_ssize_t __uportsys_recv(int sd, void *buf, __uportsys_size_t len, int flags, int *err_num);
__uportsys_ssize_t __uportsys_recvfrom(int sd, void *buf, __uportsys_size_t len, int flags, struct __uportsys_sockaddr *addr, __uportsys_socklen_t *addr_len, int *err_num);
__uportsys_ssize_t __uportsys_recvmsg(int sd, struct __uportsys_msghdr *msg, int flags, int *err_num);
__uportsys_ssize_t __uportsys_send(int sd, const void *buf, __uportsys_size_t len, int flags, int *err_num);
__uportsys_ssize_t __uportsys_sendmsg(int sd, const struct __uportsys_msghdr *msg, int flags, int *err_num);
__uportsys_ssize_t __uportsys_sendto(int sd, const void *buf, __uportsys_size_t len, int flags, const struct __uportsys_sockaddr *addr, __uportsys_socklen_t addr_len, int *err_num);
int __uportsys_setsockopt(int sd, int level, int opt_name, const void *opt_value, __uportsys_socklen_t opt_len, int *err_num);
int __uportsys_shutdown(int sd, int how, int *err_num);
int __uportsys_socket(int domain, int type, int protocol, int *err_num);
int __uportsys_socket2(int domain, int type, int protocol, int *sds, int *err_num);

/* System functions for the sys/stat.h header. */

int __uportsys_chmod(const char *path_name, __uportsys_mode_t mode, int *err_num);
int __uportsys_fchmod(int fd, __uportsys_mode_t mode, int *err_num);
int __uportsys_fstat(int fd, struct __uportsys_stat *buf, int *err_num);
int __uportsys_lstat(const char *path_name, struct __uportsys_stat *buf, int *err_num);
int __uportsys_mkdir(const char *dir_name, __uportsys_mode_t mode, int *err_num);
int __uportsys_mknod(const char *file_name, __uportsys_mode_t mode, __uportsys_dev_t dev, int *err_num);
int __uportsys_stat(const char *path_name, struct __uportsys_stat *buf, int *err_num);
__uportsys_mode_t __uportsys_umask(__uportsys_mode_t mask, int *err_num);

/* System functions for the sys/statvfs.h header. */

int __uportsys_fstatvfs(int fd, struct __uportsys_statvfs *buf, int *err_num);
int __uportsys_statvfs(const char *file_name, struct __uportsys_statvfs *buf, int *err_num);

/* System functions for the sys/time.h header. */

int __uportsys_getitimer(int which, struct __uportsys_itimerval *value, int *err_num);
#ifdef __UPORTSYS_STRUCT_TIMEZONE
int __uportsys_gettimeofday(struct __uportsys_timeval *tv, struct __uportsys_timezone *tz, int *err_num);
#else
int __uportsys_gettimeofday(struct __uportsys_timeval *tv, void *tz, int *err_num);
#endif
int __uportsys_setitimer(int which, const struct __uportsys_itimerval *value, struct __uportsys_itimerval *old_value, int *err_num);
int __uportsys_select(int nfds, __uportsys_fd_set *rfds, __uportsys_fd_set *wfds, __uportsys_fd_set *efds, struct __uportsys_timeval *timeout, int *err_num);
int __uportsys_utimes(const char *path_name, const struct __uportsys_timeval *times, int *err_num);

/* System functions for the sys/times.h header. */

__uportsys_clock_t __uportsys_times(struct __uportsys_tms *buf, int *err_num);

/* System functions for the sys/uio.h header. */

#ifdef ___UPORTSYS_READV
__uportsys_ssize_t __uport_readv(int fd, const struct __uportsys_iovec *iov, int iov_count, int *err_num);
#endif
#ifdef ___UPORTSYS_WRITEV
__uportsys_ssize_t __uport_writev(int fd, const struct __uportsys_iovec *iov, int iov_count, int *err_num);
#endif

/* System functions for the sys/utsname.h header. */

int __uportsys_uname(struct __uportsys_utsname *buf, int *err_num);

/* System functions for the sys/wait.h header. */

__uportsys_pid_t __uportsys_wait3(int *status, int options, struct __uportsys_rusage *usage, int *err_num);
int __uportsys_waitid(__uportsys_idtype_t id_type, __uportsys_id_t id, __uportsys_siginfo_t *info, int options, int *err_num);
__uportsys_pid_t __uportsys_waitpid(__uportsys_pid_t pid, int *status, int options, int *err_num);

/* System functions for the fcntl.h header. */

int __uportsys_fcntl(int fd, int cmd, __uportsys_intptr_t arg, int *err_num);
int __uportsys_open(const char *file_name, int flags, __uportsys_mode_t mode, int *err_num);

/* System functions for the poll.h header. */

int __uportsys_poll(struct __uportsys_pollfd *fds, __uportsys_nfds_t nfds, int timeout, int *err_num);

/* System functions for the signal.h header. */

int __uportsys_kill(__uportsys_pid_t pid, int sig_num, int *err_num);
int __uportsys_sigaction(int sig_num, const struct __uportsys_sigaction *act, struct __uportsys_sigaction *old_act, int *err_num);
int __uportsys_sigaltstack(const __uportsys_stack_t *stack, __uportsys_stack_t *old_stack, int *err_num);
int __uportsys_sigpending(__uportsys_sigset_t *set, int *err_num);
int __uportsys_sigprocmask(int how, __uportsys_sigset_t *set, __uportsys_sigset_t *old_set, int *err_num);
int __uportsys_sigsuspend(const __uportsys_sigset_t *set, int *err_num);
int __uportsys_sigtimedwait(const __uportsys_sigset_t *set, __uportsys_siginfo_t *info, const struct __uportsys_timespec *timeout, int *err_num);

/* System functions for the time.h header. */

int __uportsys_nanosleep(const struct __uportsys_timespec *req, const struct __uportsys_timespec *rem, int *err_num);

/* System functions for the termios.h header. */

int __uportsys_tcdrain(int fd, int *err_num);
int __uportsys_tcflow(int fd, int action, int *err_num);
int __uportsys_tcflush(int fd, int queue_selector, int *err_num);
int __uportsys_tcgetattr(int fd, struct __uportsys_termios *termios, int *err_num);
__uportsys_pid_t __uportsys_tcgetsid(int fd, int *err_num);
int __uportsys_tcsendbreak(int fd, int duration, int *err_num);
int __uportsys_tcsetattr(int fd, int opt_actions, const struct __uportsys_termios *termios, int *err_num);

/* System functions for the unistd.h header. */

int __uportsys__exit(int status, int *err_num);
int __uportsys_access(const char *path_name, int mode, int *err_num);
void *__uportsys_brk(void *addr, int *err_num);
int __uportsys_chdir(const char *dir_name, int *err_num);
int __uportsys_chroot(const char *dir_name, int *err_num);
int __uportsys_chown(const char *path_name, __uportsys_uid_t uid, __uportsys_gid_t gid, int *err_num);
int __uportsys_close(int fd, int *err_num);
int __uportsys_dup(int old_fd, int *err_num);
int __uportsys_dup2(int old_fd, int new_fd, int *err_num);
int __uportsys_execve(const char *file_name, char *const *argv, char *const *env, int *err_num);
int __uportsys_fchdir(const char *dir_name, int *err_num);
int __uportsys_fchown(const char *path_name, __uportsys_uid_t uid, __uportsys_gid_t gid, int *err_num);
int __uportsys_fdatasync(int fd, int *err_num);
__uportsys_pid_t __uportsys_fork(int *err_num);
int __uportsys_fsync(int fd, int *err_num);
int __uportsys_ftruncate(int fd, __uportsys_off_t length, int *err_num);
char *__uportsys_getcwd(char *buf, __uportsys_size_t size, int *err_num);
__uportsys_gid_t __uportsys_getegid(int *err_num);
__uportsys_uid_t __uportsys_geteuid(int *err_num);
__uportsys_gid_t __uportsys_getgid(int *err_num);
int __uportsys_getgroups(int size, __uportsys_gid_t *groups, int *err_num);
__uportsys_pid_t __uportsys_getpgid(__uportsys_pid_t pid, int *err_num);
__uportsys_pid_t __uportsys_getpid(int *err_num);
__uportsys_pid_t __uportsys_getppid(int *err_num);
__uportsys_pid_t __uportsys_getsid(int *err_num);
__uportsys_uid_t __uportsys_getuid(int *err_num);
int __uportsys_lchown(const char *path_name, __uportsys_uid_t uid, __uportsys_gid_t gid, int *err_num);
int __uportsys_link(const char *old_file_name, const char *new_file_name, int *err_num);
__uportsys_off_t __uportsys_lseek(int fd, __uportsys_off_t offset, int whence, int *err_num);
int __uportsys_nice(int inc, int *err_num);
int __uportsys_pause(int *err_num);
int __uportsys_pipe(int *fds, int *err_num);
#ifdef ___UPORTSYS_PREAD
__uportsys_ssize_t __uportsys_pread(int fd, void *buf, __uportsys_size_t count, __uportsys_off_t offset, int *err_num);
#endif
#ifdef ___UPORTSYS_PWRITE
__uportsys_ssize_t __uportsys_pwrite(int fd, const void *buf, __uportsys_size_t count, __uportsys_off_t offset, int *err_num);
#endif
__uportsys_ssize_t __uportsys_read(int fd, void *buf, __uportsys_size_t count, int *err_num);
int __uportsys_readlink(const char *path_name, char *buf, __uportsys_size_t size, int *err_num);
int __uportsys_rmdir(const char *dir_name, int *err_num);
int __uportsys_setegid(__uportsys_gid_t gid, int *err_num);
int __uportsys_seteuid(__uportsys_uid_t uid, int *err_num);
int __uportsys_setgid(__uportsys_gid_t gid, int *err_num);
int __uportsys_setgroups(int size, const __uportsys_gid_t *groups, int *err_num);
int __uportsys_setpgid(__uportsys_pid_t pid, __uportsys_pid_t pgid, int *err_num);
__uportsys_pid_t __uportsys_setsid(int *err_num);
int __uportsys_setuid(__uportsys_uid_t uid, int *err_num);
int __uportsys_symlink(const char *old_path_name, const char *new_path_name, int *err_num);
int __uportsys_sync(int *err_num);
__uportsys_pid_t __uportsys_tcgetpgrp(int fd, int *err_num);
int __uportsys_tcsetpgrp(int fd, __uportsys_pid_t pgrp, int *err_num);
int __uportsys_truncate(int fd, __uportsys_off_t length, int *err_num);
int __uportsys_unlink(const char *file_name, int *err_num);
#ifdef ___UPORTSYS_VFORK
__uportsys_pid_t __uportsys_vfork(int *err_num);
#endif
__uportsys_ssize_t __uportsys_write(int fd, const void *buf, __uportsys_size_t count, int *err_num);

/* Other system functions for directories. */

int __uportsys_closedir(__uportsys_dir_t *dir, int *err_num);
int __uportsys_opendir(__uportsys_dir_t *dir, const char *dir_name, int *err_num);
long __uportsys_readdir(__uportsys_dir_t *dir, void *buf, long count, int *err_num);
int __uportsys_seekdir(__uportsys_dir_t *dir, long loc, int *err_num);
long __uportsys_telldir(__uportsys_dir_t *dir, int *err_num); 

/* Other system functions for threads. */

int __uportsys_thread_create(__uportsys_thread_t *thread, void (*start_fun)(void *), void *arg, void *stack, __uportsys_size_t stack_size, void *tls, __uportsys_size_t tls_size, int *err_num);
int __uportsys_thread_detach(__uportsys_thread_t *thread, int *err_num);
int __uportsys_thread_exit(int *err_num);
int __uportsys_thread_join(__uportsys_thread_t *thread, int *err_num);
int __uportsys_thread_kill(__uportsys_thread_t *thread, int sig_num, int *err_num);
int __uportsys_thread_mutex_wait(__uportsys_thread_mutex_t *mutex, __uportsys_thread_t *(self_thread_fun)(void), int *err_num);
int __uportsys_thread_mutex_wake(__uportsys_thread_mutex_t *mutex, int *err_num);
int __uportsys_thread_settls(void *tls, __uportsys_size_t tls_size, int *err_num);
int __uportsys_thread_sigmask(int how, const __uportsys_sigset_t *set, const __uportsys_sigset_t *old_set, int *err_num);

#ifdef __cplusplus
}
#endif

#endif
