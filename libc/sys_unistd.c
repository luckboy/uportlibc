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
#include <uportsys/sys.h>
#include <errno.h>
#include <unistd.h>

void _exit(int status)
{
  int tmp_err_num;
  __uportsys__exit(status, &tmp_err_num);
  while(1);
}

int access(const char *path_name, int mode)
{ return __uportsys_access(path_name, mode, &errno); }

void *brk(void *addr)
{ return __uportsys_brk(addr, &errno); }

int chdir(const char *dir_name)
{ return __uportsys_chdir(dir_name, &errno); }

int chroot(const char *dir_name)
{ return __uportsys_chroot(dir_name, &errno); }

int chown(const char *path_name, uid_t uid, gid_t gid)
{ return __uportsys_chown(path_name, uid, gid, &errno); }

int close(int fd)
{ return __uportsys_close(fd, &errno); }

int dup(int old_fd)
{ return __uportsys_dup(old_fd, &errno); }

int dup2(int old_fd, int new_fd)
{ return __uportsys_dup2(old_fd, new_fd, &errno); }

int execve(const char *file_name, char *const *argv, char *const *env)
{ return __uportsys_execve(file_name, argv, env, &errno); }

int fchdir(const char *dir_name)
{ return __uportsys_fchdir(dir_name, &errno); }

int fchown(const char *path_name, uid_t uid, gid_t gid)
{ return __uportsys_fchown(path_name, uid, gid, &errno); }

int fdatasync(int fd)
{ return __uportsys_fdatasync(fd, &errno); }

pid_t fork(void)
{ return __uportsys_fork(&errno); }

int fsync(int fd)
{ return __uportsys_fsync(fd, &errno); }

int ftruncate(int fd, off_t length)
{ return __uportsys_ftruncate(fd, length, &errno); }

char *getcwd(char *buf, size_t size)
{ return __uportsys_getcwd(buf, size, &errno); }

gid_t getegid(void)
{ return __uportsys_getegid(&errno); }

uid_t geteuid(void)
{ return __uportsys_geteuid(&errno); }

gid_t getgid(void)
{ return __uportsys_getgid(&errno); }

int getgroups(int size, gid_t *groups)
{ return __uportsys_getgroups(size, groups, &errno); }

pid_t getpgid(pid_t pid)
{ return __uportsys_getpgid(pid, &errno); }

pid_t getpid(void)
{ return __uportsys_getpid(&errno); }

pid_t getppid(void)
{ return __uportsys_getppid(&errno); }

pid_t getsid(void)
{ return __uportsys_getsid(&errno); }

uid_t getuid(void)
{ return __uportsys_getuid(&errno); }

int lchown(const char *path_name, uid_t uid, gid_t gid)
{ return __uportsys_lchown(path_name, uid, gid, &errno); }

int link(const char *old_file_name, const char *new_file_name)
{ return __uportsys_link(old_file_name, new_file_name, &errno); }

off_t lseek(int fd, off_t offset, int whence)
{ return __uportsys_lseek(fd, offset, whence, &errno); }

int nice(int inc)
{ return __uportsys_nice(inc, &errno); }

int pause(void)
{ return __uportsys_pause(&errno); }

int pipe(int *fds)
{ return __uportsys_pipe(fds, &errno); }

ssize_t pread(int fd, void *buf, size_t count, off_t offset)
{
#ifdef ___UPORTSYS_PREAD
  return __uportsys_pread(fd, buf, count, offset, &errno);
#else
  off_t saved_offset;
  ssize_t res;
  saved_offset = lseek(fd, SEEK_CUR, 0);
  if(saved_offset == -1) return -1;
  if(lseek(fd, SEEK_SET, offset) == -1) return -1;
  res = read(fd, buf, count);
  if(lseek(fd, SEEK_SET, saved_offset) == -1) return -1;
  return res;
#endif
}

ssize_t pwrite(int fd, const void *buf, size_t count, off_t offset)
{
#ifdef ___UPORTSYS_PWRITE
  return __uportsys_pwrite(fd, buf, count, offset, &errno);
#else
  off_t saved_offset;
  ssize_t res;
  saved_offset = lseek(fd, SEEK_CUR, 0);
  if(saved_offset == -1) return -1;
  if(lseek(fd, SEEK_SET, offset) == -1) return -1;
  res = write(fd, buf, count);
  if(lseek(fd, SEEK_SET, saved_offset) == -1) return -1;
  return res;
#endif
}

ssize_t read(int fd, void *buf, size_t count)
{ return __uportsys_read(fd, buf, count, &errno); }

int readlink(const char *path_name, char *buf, size_t size)
{ return __uportsys_readlink(path_name, buf, size, &errno); }

int rmdir(const char *dir_name)
{ return __uportsys_rmdir(dir_name, &errno); }

int setegid(gid_t gid)
{ return __uportsys_setegid(gid, &errno); }

int seteuid(uid_t uid)
{ return __uportsys_seteuid(uid, &errno); }

int setgid(gid_t gid)
{ return __uportsys_setgid(gid, &errno); }

int setgroups(int size, const gid_t *groups)
{ return __uportsys_setgroups(size, groups, &errno); }

int setpgid(pid_t pid, pid_t pgid)
{ return __uportsys_setpgid(pid, pgid, &errno); }

pid_t setsid(void)
{ return __uportsys_setsid(&errno); }

int setuid(uid_t uid)
{ return __uportsys_setuid(uid, &errno); }

int symlink(const char *old_path_name, const char *new_path_name)
{ return __uportsys_symlink(old_path_name, new_path_name, &errno); }

void sync(void)
{
  int tmp_err_num;
  __uportsys_sync(&tmp_err_num);
}

pid_t tcgetpgrp(int fd)
{ return __uportsys_tcgetpgrp(fd, &errno); }

int tcsetpgrp(int fd, pid_t pgrp)
{ return __uportsys_tcsetpgrp(fd, pgrp, &errno); }

int truncate(int fd, off_t length)
{ return __uportsys_truncate(fd, length, &errno); }

int unlink(const char *file_name)
{ return __uportsys_unlink(file_name, &errno); }

pid_t vfork(void)
{
#ifdef ___UPORTSYS_VFORK
  return __uportsys_vfork(&errno);
#else
  return fork();
#endif
}

ssize_t write(int fd, const void *buf, size_t count)
{ return __uportsys_write(fd, buf, count, &errno); }
