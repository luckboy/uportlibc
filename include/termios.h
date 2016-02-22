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
#ifndef _TERMIOS_H
#define _TERMIOS_H

#include <uportsys/sys.h>

#define NCCS                    __UPORTSYS_NCCS

#define BRKINT                  __UPORTSYS_BRKINT
#define ICRNL                   __UPORTSYS_ICRNL
#define IGNBRK                  __UPORTSYS_IGNBRK
#define IGNCR                   __UPORTSYS_IGNCR
#define IGNPAR                  __UPORTSYS_IGNPAR
#define INLCR                   __UPORTSYS_INLCR
#define INPCK                   __UPORTSYS_INPCK
#define ISTRIP                  __UPORTSYS_ISTRIP
#ifdef __UPORTSYS_IUCLC
#define IUCLC                   __UPORTSYS_IUCLC
#endif
#define IXANY                   __UPOPTSYS_IXANY
#define IXOFF                   __UPORTSYS_IXOFF
#define IXON                    __UPORTSYS_IXON
#define PARMRK                  __UPORTSYS_PARMRK

#define OPOST                   __UPORTSYS_OPOST
#ifdef __UPORTSYS_OLCUC
#define OLCUC                   __UPORTSYS_OLCUC
#endif
#define ONLCR                   __UPORTSYS_ONLCR
#define OCRNL                   __UPORTSYS_OCRNL
#define ONOCR                   __UPORTSYS_ONOCR
#define ONLRET                  __UPORTSYS_ONLRET
#ifdef __UPORTSYS_OFILL
#define OFILL                   __UPORTSYS_OFILL
#endif
#ifdef __UPORTSYS_NLDLY
#define NLDLY                   __UPORTSYS_NLDLY
#ifdef __UPORTSYS_NL0
#define NL0                     __UPORTSYS_NL0
#endif
#ifdef __UPORTSYS_NL1
#define NL1                     __UPORTSYS_NL1
#endif
#endif
#ifdef __UPORTSYS_CRDLY
#define CRDLY                   __UPORTSYS_CRDLY
#define CR0                     __UPORTSYS_CR0
#define CR1                     __UPORTSYS_CR1
#define CR2                     __UPORTSYS_CR2
#define CR3                     __UPORTSYS_CR3
#endif
#ifdef __UPORTSYS_TABDLY
#define TABDLY                  __UPORTSYS_TABDLY
#ifdef __UPORTSYS_TAB0
#define TAB0                    __UPORTSYS_TAB0
#endif
#ifdef __UPORTSYS_TAB1
#define TAB1                    __UPORTSYS_TAB1
#endif
#ifdef __UPORTSYS_TAB2
#define TAB2                    __UPORTSYS_TAB2
#endif
#ifdef __UPORTSYS_TAB3
#define TAB3                    __UPORTSYS_TAB3
#endif
#endif
#ifdef __UPORTSYS_BSDLY 
#define BSDLY                   __UPORTSYS_BSDLY
#ifdef __UPORTSYS_BS0
#define BS0                     __UPORTSYS_BS0
#endif
#ifdef __UPORTSYS_BS1
#define BS1                     __UPORTSYS_BS1
#endif
#endif
#ifdef __UPORTSYS_VTDLY
#define VTDLY                   __UPORTSYS_VTDLY
#ifdef __UPORTSYS_VT0
#define VT0                     __UPORTSYS_VT0
#endif
#ifdef __UPORTSYS_VT1
#define VT1                     __UPORTSYS_VT1
#endif
#endif
#ifdef __UPORTSYS_FFDLY
#define FFDLY                   __UPORTSYS_FFDLY
#ifdef __UPORTSYS_FF0
#define FF0                     __UPORTSYS_FF0
#endif
#ifdef __UPORTSYS_FF1
#define FF1                     __UPORTSYS_FF1
#endif
#endif

#ifdef __UPORTSYS_CSIZE
#define CSIZE                   __UPORTSYS_CSIZE
#ifdef __UPORTSYS_CS5
#define CS5                     __UPORTSYS_CS5
#endif
#ifdef __UPORTSYS_CS6
#define CS6                     __UPORTSYS_CS6
#endif
#ifdef __UPORTSYS_CS7
#define CS7                     __UPORTSYS_CS7
#endif
#ifdef __UPORTSYS_CS8
#define CS8                     __UPORTSYS_CS8
#endif
#endif
#define CSTOPB                  __UPORTSYS_CSTOPB
#define CREAD                   __UPORTSYS_CREAD
#define PARENB                  __UPORTSYS_PARENB
#define PARODD                  __UPORTSYS_PARODD
#define HUPCL                   __UPORTSYS_HUPCL
#define CLOCAL                  __UPORTSYS_CLOCAL

#define ECHO                    __UPORTSYS_ECHO
#define ECHOE                   __UPORTSYS_ECHOE
#define ECHOK                   __UPORTSYS_ECHOK
#define ECHONL                  __UPORTSYS_ECHONL
#define ICANON                  __UPORTSYS_ICANON
#define IEXTEN                  __UPORTSYS_IEXTEN
#define ISIG                    __UPORTSYS_ISIG
#define NOFLSH                  __UPORTSYS_NOFLSH
#define TOSTOP                  __UPORTSYS_TOSTOP
#ifdef __UPORTSYS_XCASE
#define XCASE                   __UPORTSYS_XCASE
#endif

#define B0                      __UPORTSYS_B0
#define B50                     __UPORTSYS_B50
#define B75                     __UPORTSYS_B75
#define B110                    __UPORTSYS_B110
#define B134                    __UPORTSYS_B134
#define B150                    __UPORTSYS_B150
#define B200                    __UPORTSYS_B200
#define B300                    __UPORTSYS_B300
#define B600                    __UPORTSYS_B600
#define B1200                   __UPORTSYS_B1200
#define B1800                   __UPORTSYS_B1800
#define B2400                   __UPORTSYS_B2400
#define B4800                   __UPORTSYS_B4800
#define B9600                   __UPORTSYS_B9600
#define B19200                  __UPORTSYS_B19200
#define B38400                  __UPORTSYS_B38400

#define VEOF                    __UPORTSYS_VEOF
#define VEOL                    __UPORTSYS_VEOL
#define VERASE                  __UPORTSYS_VERASE
#define VINTR                   __UPORTSYS_VINTR
#define VKILL                   __UPORTSYS_VKILL
#define VMIN                    __UPORTSYS_VMIN
#define VQUIT                   __UPORTSYS_VQUIT
#define VSTART                  __UPORTSYS_VSTART
#define VSTOP                   __UPORTSYS_VSTOP
#define VSUSP                   __UPORTSYS_VSUSP
#define VTIME                   __UPORTSYS_VTIME

#define TCSANOW                 __UPORTSYS_TCSANOW
#define TCSADRAIN               __UPORTSYS_TCSADRAIN
#define TCSAFLUSH               __UPORTSYS_TCSAFLUSH

#define TCOOFF                  __UPORTSYS_TCOOFF
#define TCOON                   __UPORTSYS_TCOON
#define TCIOFF                  __UPORTSYS_TCIOFF
#define TCION                   __UPORTSYS_TCION

#define TCIFLUSH                __UPORTSYS_TCIFLUSH
#define TCOFLUSH                __UPORTSYS_TCOFLUSH
#define TCIOFLUSH               __UPORTSYS_TCIOFLUSH

#ifdef __cplusplus
extern "C" {
#endif

typedef __uportsys_cc_t cc_t;

#ifndef _PID_T
#define _PID_T
typedef __uportsys_pid_t pid_t;
#endif

typedef __uportsys_speed_t speed_t;
typedef __uportsys_tcflag_t tcflag_t;

__UPORTSYS_STRUCT_TERMIOS(termios);

/* System functions. */

int tcdrain(int fd);
int tcflow(int fd, int action);
int tcflush(int fd, int queue_selector);
int tcgetattr(int fd, struct termios *termios);
pid_t tcgetsid(int fd);
int tcsendbreak(int fd, int duration);
int tcsetattr(int fd, int opt_actions, const struct termios *termios);

/* Other functions. */

speed_t cfgetispeed(const struct termios *termios);
speed_t cfgetospeed(const struct termios *termios);
int cfsetispeed(struct termios *termios, speed_t speed);
int cfsetospeed(struct termios *termios, speed_t speed);

#ifdef __cplusplus
}
#endif

#endif
