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
#ifndef _SYS_MOCK_H
#define _SYS_MOCK_H

#include <sys/types.h>
#include <sys/stat.h>
#include <stddef.h>

/* Functions and macros for the sys/stat.h header. */

int uportlibc_sys_mock_lstat(const char *path_name, struct stat *buf);

#ifdef SYS_MOCK_SYS_STAT
#define lstat uportlibc_sys_mock_lstat
#endif

/* Functions and macros for the sys/waut.h header. */

pid_t uportlibc_sys_mock_waitpid(pid_t pid, int *status, int options);

#ifdef SYS_MOCK_SYS_WAIT
#define waitpid uportlibc_sys_mock_waitpid
#endif

/* Functions and macros for the fcntl.h header. */

int uportlibc_sys_mock_open(const char *file_name, int flags, ...);

#ifdef SYS_MOCK_FCNTL
#define open uportlibc_sys_mock_open
#endif

/* Functions and macros for the limits.h header. */

#define SYS_MOCK_OPEN_MAX       256

#ifdef SYS_MOCK_LIMITS
#undef OPEN_MAX
#define OPEN_MAX SYS_MOCK_OPEN_MAX
#endif

/* Functions and macros for the unistd.h header. */

#define SYS_MOCK_STDIN_FILENO   0
#define SYS_MOCK_STDOUT_FILENO  1
#define SYS_MOCK_STDERR_FILENO  2

int uportlibc_sys_mock__exit(int status);
int uportlibc_sys_mock_close(int fd);
int uportlibc_sys_mock_dup2(int old_fd, int new_fd);
int uportlibc_sys_mock_execve(const char *file_name, char *const *argv, char *const *env);
pid_t uportlibc_sys_mock_fork(void);
int uportlibc_sys_mock_isatty(int fd);
off_t uportlibc_sys_mock_lseek(int fd, off_t offset, int whence);
int uportlibc_sys_mock_pipe(int *fds);
ssize_t uportlibc_sys_mock_read(int fd, void *buf, size_t count);
int uportlibc_sys_mock_rmdir(const char *dir_name);
ssize_t uportlibc_sys_mock_write(int fd, const void *buf, size_t count);
int uportlibc_sys_mock_unlink(const char *file_name);

#ifdef SYS_MOCK_UNISTD
#undef STDIN_FILENO
#define STDIN_FILENO SYS_MOCK_STDIN_FILENO
#undef STDOUT_FILENO
#define STDOUT_FILENO SYS_MOCK_STDOUT_FILENO
#undef STDERR_FILENO
#define STDERR_FILENO SYS_MOCK_STDERR_FILENO
#define _exit uportlibc_sys_mock__exit
#define close uportlibc_sys_mock_close
#define dup2 uportlibc_sys_mock_dup2
#define execve uportlibc_sys_mock_execve
#define fork uportlibc_sys_mock_fork
#define isatty uportlibc_sys_mock_isatty
#define lseek uportlibc_sys_mock_lseek
#define pipe uportlibc_sys_mock_pipe
#define read uportlibc_sys_mock_read
#define rmdir uportlibc_sys_mock_rmdir
#define write uportlibc_sys_mock_write
#define unlink uportlibc_sys_mock_unlink
#endif

/* Variables. */

extern char **uportlibc_sys_mock_environ;

#ifdef SYS_MOCK_ENVIRON
#define environ uportlibc_sys_mock_environ
#endif

#endif
