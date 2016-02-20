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
#ifndef _UNISTD_H
#define _UNISTD_H

#include <uportsys/sys.h>

#define R_OK                    __UPORTSYS_R_OK
#define W_OK                    __UPORTSYS_W_OK
#define X_OK                    __UPORTSYS_X_OK

#ifndef SEEK_SET
#define SEEK_SET                __UPORTSYS_SEEK_SET
#define SEEK_CUR                __UPORTSYS_SEEK_CUR
#define SEEK_END                __UPORTSYS_SEEK_END
#endif

#define STDIN_FILENO            0
#define STDOUT_FILENO           1
#define STDERR_FILENO           2

#define F_LOCK                  0
#define F_TEST                  1
#define F_TLOCK                 2
#define F_ULOCK                 3

#ifdef __cplusplus
extern "C" {
#endif

#ifndef _GID_T
#define _GID_T
typedef __uportsys_gid_t gid_t;
#endif

#ifndef _INTPTR_T
#define _INTPTR_T
typedef __INTPTR_TYPE__ intptr_t;
#endif

#ifndef _OFF_T
#define _OFF_T
typedef __uportsys_off_t off_t;
#endif

#ifndef _PID_T
#define _PID_T
typedef __uportsys_pid_t pid_t;
#endif

#ifndef _SIZE_T
#define _SIZE_T
typedef __SIZE_TYPE__ size_t;
#endif

#ifndef _SSIZE_T
#define _SSIZE_T
typedef __uportsys_ssize_t ssize_t;
#endif

#ifndef _UID_T
#define _UID_T
typedef __uportsys_uid_t uid_t;
#endif

#ifndef _USECONDS_T
#define _USECONDS_T
typedef __uportsys_useconds_t useconds_t;
#endif

/* System functions. */

void _exit(int status);
int access(const char *path_name, int mode);
void *brk(void *addr);
int chdir(const char *dir_name);
int chroot(const char *dir_name);
int chown(const char *path_name, uid_t uid, gid_t gid);
int close(int fd);
int dup(int old_fd);
int dup2(int old_fd, int new_fd);
int execve(const char *file_name, char *const *argv, char *const *env);
int fchdir(const char *dir_name);
int fchown(const char *path_name, uid_t uid, gid_t gid);
int fdatasync(int fd);
pid_t fork(void);
int fsync(int fd);
int ftruncate(int fd, off_t length);
char *getcwd(char *buf, size_t size);
gid_t getegid(void);
uid_t geteuid(void);
gid_t getgid(void);
int getgroups(int size, gid_t *groups);
int getpagesize(void);
pid_t getpgid(pid_t pid);
pid_t getpid(void);
pid_t getppid(void);
pid_t getsid(void);
uid_t getuid(void);
int lchown(const char *path_name, uid_t uid, gid_t gid);
int link(const char *old_file_name, const char *new_file_name);
off_t lseek(int fd, off_t offset, int whence);
int nice(int inc);
int pause(void);
int pipe(int *fds);
ssize_t pread(int fd, void *buf, size_t count, off_t offset);
ssize_t pwrite(int fd, const void *buf, size_t count, off_t offset);
ssize_t read(int fd, void *buf, size_t count);
int readlink(const char *path_name, char *buf, size_t size);
int rmdir(const char *dir_name);
int setegid(gid_t gid);
int seteuid(uid_t uid);
int setgid(gid_t gid);
int setgroups(int size, const gid_t *groups);
int setpgid(pid_t pid, pid_t pgid);
pid_t setsid(void);
int setuid(uid_t uid);
int symlink(const char *old_path_name, const char *new_path_name);
void sync(void);
pid_t tcgetpgrp(int fd);
int tcsetpgrp(int fd, pid_t pgrp);
int truncate(int fd, off_t length);
int unlink(const char *file_name);
pid_t vfork(void);
ssize_t write(int fd, const void *buf, size_t count);

/* Other functions. */

unsigned alarm(unsigned seconds);
char *ctermid(char *str);
int execl(const char *file_name, const char *arg, ...);
int execle(const char *file_name, const char *arg, ...);
int execlp(const char *file_name, const char *arg, ...);
int execv(const char *file_name, char *const *argv);
int execvp(const char *file_name, char *const *argv);
int getdtablesize(void);
long gethostid(void);
int getopt(int argc, char *const *argv, const char *options);
int getpgrp(void);
char *getwd(char *buf);
int isatty(int fd);
int lockf(int fd, int cmd, off_t len);
void *sbrk(intptr_t incr);
int setpgrp(void);
int setregid(gid_t rgid, gid_t egid);
int setreuid(uid_t ruid, gid_t euid);
unsigned sleep(unsigned seconds);
void swab(const void *src, void *dst, ssize_t count);
int usleep(useconds_t useconds);

#ifdef __cplusplus
}
#endif

#endif
