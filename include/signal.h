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
#ifndef _SIGNAL_H
#define _SIGNAL_H

#include <uportsys/sys.h>

#define SA_NOCLDSTOP            __UPORTSYS_SA_NOCLDSTOP
#define SA_NOCLDWAIT            __UPORTSYS_SA_NOCLDWAIT
#define SA_NODEFER              __UPORTSYS_SA_NODEFER
#define SA_ONSTACK              __UPORTSYS_SA_ONSTACK
#define SA_RESETHAND            __UPORTSYS_SA_RESETHAND
#define SA_RESTART              __UPORTSYS_SA_RESTART
#define SA_SIGINFO              __UPORTSYS_SA_SIGINFO

#define SIG_DFL                 __UPORTSYS_SIG_DFL
#define SIG_IGN                 __UPORTSYS_SIG_IGN
#define SIG_HOLD                __UPORTSYS_SIG_HOLD
#define SIG_ERR                 __UPORTSYS_SIG_ERR

#define SIG_BLOCK               __UPORTSYS_SIG_BLOCK
#define SIG_SETMASK             __UPORTSYS_SIG_SETMASK
#define SIG_UNBLOCK             __UPORTSYS_SIG_UNBLOCK

#define SIGHUP                  __UPORTSYS_SIGHUP
#define SIGINT                  __UPORTSYS_SIGINT
#define SIGQUIT                 __UPORTSYS_SIGQUIT
#define SIGILL                  __UPORTSYS_SIGILL
#define SIGTRAP                 __UPORTSYS_SIGTRAP
#define SIGABRT                 __UPORTSYS_SIGABRT
#define SIGBUS                  __UPORTSYS_SIGBUS
#define SIGFPE                  __UPORTSYS_SIGFPE
#define SIGKILL                 __UPORTSYS_SIGKILL
#define SIGUSR1                 __UPORTSYS_SIGUSR1
#define SIGSEGV                 __UPORTSYS_SIGSEGV
#define SIGUSR2                 __UPORTSYS_SIGUSR2
#define SIGPIPE                 __UPORTSYS_SIGPIPE
#define SIGALRM                 __UPORTSYS_SIGALRM
#define SIGTERM                 __UPORTSYS_SIGTERM
#define SIGCHLD                 __UPORTSYS_SIGCHLD
#define SIGCONT                 __UPORTSYS_SIGCONT
#define SIGSTOP                 __UPORTSYS_SIGSTOP
#define SIGTSTP                 __UPORTSYS_SIGTSTP
#define SIGTTIN                 __UPORTSYS_SIGTTIN
#define SIGTTOU                 __UPORTSYS_SIGTTOU
#define SIGURG                  __UPORTSYS_SIGURG
#define SIGXCPU                 __UPORTSYS_SIGXCPU
#define SIGXFSZ                 __UPORTSYS_SIGXFSZ
#define SIGVTALRM               __UPORTSYS_SIGVTALRM
#define SIGPROF                 __UPORTSYS_SIGPROF
#ifdef __UPORTSYS_SIGWINCH
#define SIGWINCH                __UPORTSYS_SIGWINCH
#endif
#define SIGPOLL                 __UPORTSYS_SIGPOLL
#define SIGSYS                  __UPORTSYS_SIGSYS
#define SIGRTMIN                __UPORTSYS_SIGRTMIN
#define SIGRTMAX                __UPORTSYS_SIGRTMAX

#define MINSIGSTKSZ             __UPORTSYS_MINSIGSTKSZ
#define SIGSTKSZ                __UPORTSYS_SIGSTKSZ

#define SS_ONSTACK              __UPORTSYS_SS_ONSTACK
#define SS_DISABLE              __UPORTSYS_SS_DISABLE

#define ILL_ILLOPC              __UPORTSYS_ILL_ILLOPC
#define ILL_ILLOPN              __UPORTSYS_ILL_ILLOPN
#define ILL_ILLADR              __UPORTSYS_ILL_ILLADR
#define ILL_ILLTRP              __UPORTSYS_ILL_ILLTRP
#define ILL_PRVOPC              __UPORTSYS_ILL_PRVOPC
#define ILL_PRVREG              __UPORTSYS_ILL_PRVREG
#define ILL_COPROC              __UPORTSYS_ILL_COPROC
#define ILL_BADSTK              __UPORTSYS_ILL_BADSTK

#define FPE_INTDIV              __UPORTSYS_FPE_INTDIV
#define FPE_INTOVF              __UPORTSYS_FPE_INTOVF
#define FPE_FLTDIV              __UPORTSYS_FPE_FLTDIV
#define FPE_FLTOVF              __UPORTSYS_FPE_FLTOVF
#define FPE_FLTUND              __UPORTSYS_FPE_FLTUND
#define FPE_FLTRES              __UPORTSYS_FPE_FLTRES
#define FPE_FLTINV              __UPORTSYS_FPE_FLTINV
#define FPE_FLTSUB              __UPORTSYS_FPE_FLTSUB

#define SEGV_MAPERR             __UPORTSYS_SEGV_MAPERR
#define SEGV_ACCERR             __UPORTSYS_SEGV_ACCERR

#define BUS_ADRALN              __UPORTSYS_BUS_ADRALN
#define BUS_ADRERR              __UPORTSYS_BUS_ADRERR
#define BUS_OBJERR              __UPORTSYS_BUS_OBJERR

#define TRAP_BRKPT              __UPORTSYS_TRAP_BRKPT
#define TRAP_TRACE              __UPORTSYS_TRAP_TRACE

#define CLD_EXITED              __UPORTSYS_CLD_EXITED
#define CLD_KILLED              __UPORTSYS_CLD_KILLED
#define CLD_DUMPED              __UPORTSYS_CLD_DUMPED
#define CLD_TRAPPED             __UPORTSYS_CLD_TRAPPED
#define CLD_STOPPED             __UPORTSYS_CLD_STOPPED
#define CLD_CONTINUED           __UPORTSYS_CLD_CONTINUED

#define POLL_IN                 __UPORTSYS_POLL_IN
#define POLL_OUT                __UPORTSYS_POLL_OUT
#define POLL_MSG                __UPORTSYS_POLL_MSG
#define POLL_ERR                __UPORTSYS_POLL_ERR
#define POLL_PRI                __UPORTSYS_POLL_PRI
#define POLL_HUP                __UPORTSYS_POLL_HUP

#define SI_USER                 __UPORTSYS_SI_USER
#define SI_QUEUE                __UPORTSYS_SI_QUEUE
#define SI_TIMER                __UPORTSYS_SI_TIMER
#define SI_ASYNCIO              __UPORTSYS_SI_ASYNCIO
#define SI_MESGQ                __UPORTSYS_SI_MESGQ

#ifdef __cplusplus
extern "C" {
#endif

#ifndef _PID_T
#define _PID_T
typedef __uportsys_pid_t pid_t;
#endif

typedef __uportsys_sigset_t sigset_t;
typedef __uportsys_stack_t stack_t;

#ifndef _UID_T
#define _UID_T
typedef __uportsys_uid_t uid_t;
#endif

struct timespec;

__UPORTSYS_UNION_SIGVAL(sigval);
typedef __UPORTSYS_SIGINFO_T(sigval) siginfo_t;
__UPORTSYS_STRUCT_SIGACTION(sigaction, siginfo_t);

int kill(pid_t pid, int sig_num);
int sigaction(int sig_num, const struct sigaction *act, struct sigaction *old_act);
int sigaltstack(const stack_t *stack, stack_t *old_stack);
int sigpending(sigset_t *set);
int sigprocmask(int how, sigset_t *set, sigset_t *old_set);
int sigsuspend(const sigset_t *set);
int sigtimedwait(const sigset_t *set, siginfo_t *info, const struct timespec *timeout);

int sigaddset(sigset_t *set, int sig_num);
int sigdelset(sigset_t *set, int sig_num);
int sigemptyset(sigset_t *set);
int sigfillset(sigset_t *set);
int sigismember(sigset_t *set, int sig_num);

#ifdef __cplusplus
}
#endif

#endif
