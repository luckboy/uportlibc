/*
 * Copyright (c) 2016, 2018 Łukasz Szpakowski
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
#include <sys/mman.h>
#include <errno.h>
#include <fcntl.h>
#include <inttypes.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "sys_mock.h"
#include "sys_mock_priv.h"

unsigned sys_mock_op_count;
struct sys_mock_op sys_mock_ops[MAX_SYS_MOCK_OP_COUNT];

int sys_mock_last_opened_fd;
size_t sys_mock_file_buf_size;
int sys_mock_file_is_tty;
struct sys_mock_file *sys_mock_files[MAX_SYS_MOCK_FILE_COUNT];
char sys_mock_data_seg[MAX_SYS_MOCK_DATA_SEG_SIZE];
char *sys_mock_break;
struct sys_mock_mem_area sys_mock_mem_areas[MAX_SYS_MOCK_MEM_AREA_COUNT];

void *sys_mock_mmap_res;
int sys_mock_mmap_err_num;

int sys_mock_munmap_res;
int sys_mock_munmap_err_num;

int sys_mock_lstat_res;
int sys_mock_lstat_err_num;
struct stat sys_mock_lstat_buf;

int sys_mock_waitpid_res;
int sys_mock_waitpid_err_num;
int sys_mock_waitpid_status;

int sys_mock_open_res;
int sys_mock_open_err_num;

int sys_mock_close_res;
int sys_mock_close_err_num;

int sys_mock_execve_err_num;

pid_t sys_mock_fork_res;
int sys_mock_fork_err_num;

off_t sys_mock_lseek_res;
int sys_mock_lseek_err_num;

int sys_mock_pipe_res;
int sys_mock_pipe_err_num;

ssize_t sys_mock_read_res;
int sys_mock_read_err_num;

int sys_mock_rmdir_res;
int sys_mock_rmdir_err_num;

void *sys_mock_sbrk_res;
int sys_mock_sbrk_err_num;

ssize_t sys_mock_write_res;
int sys_mock_write_err_num;

int sys_mock_unlink_res;
int sys_mock_unlink_err_num;

static char *static_environ[MAX_SYS_MOCK_ENV_COUNT];

char **uportlibc_sys_mock_environ;

static struct sys_mock_file stdin_file;
static struct sys_mock_file stdout_file;
static struct sys_mock_file stderr_file;
static char stdin_buf[SYS_MOCK_FILE_BUF_SIZE];
static char stdout_buf[SYS_MOCK_FILE_BUF_SIZE];
static char stderr_buf[SYS_MOCK_FILE_BUF_SIZE];

void init_sys_mock(void)
{
  int fd, i;
  sys_mock_op_count = 0;
  sys_mock_last_opened_fd = -1;
  sys_mock_file_is_tty = 0;
  sys_mock_file_buf_size = SYS_MOCK_FILE_BUF_SIZE;
  i = 0;
  for(fd = 0; fd < MAX_SYS_MOCK_FILE_COUNT; fd++) {
    if(fd == SYS_MOCK_STDIN_FILENO) {
      stdin_file.count = 1;
      stdin_file.is_static = 1;
      stdin_file.has_static_buf = 1;
      stdin_file.is_tty = 0;
      stdin_file.file_name = NULL;
      stdin_file.flags = O_RDONLY;
      stdin_file.mode = 0;
      stdin_file.buf = stdin_buf;
      stdin_file.buf_size = sizeof(stdin_buf);
      stdin_file.buf_data_pos = 0;
      stdin_file.buf_data_size = 0;
      memset(stdin_buf, 0, sizeof(stdin_buf));
      sys_mock_files[fd] = &stdin_file;
      i++;
    } else if(fd == SYS_MOCK_STDOUT_FILENO) {
      stdout_file.count = 1;
      stdout_file.is_static = 1;
      stdout_file.has_static_buf = 1;
      stdout_file.is_tty = 0;
      stdout_file.file_name = NULL;
      stdout_file.flags = O_WRONLY;
      stdout_file.mode = 0;
      stdout_file.buf = stdout_buf;
      stdout_file.buf_size = sizeof(stdout_buf);
      stdout_file.buf_data_pos = 0;
      stdout_file.buf_data_size = 0;
      memset(stdout_buf, 0, sizeof(stdout_buf));
      sys_mock_files[fd] = &stdout_file;
      i++;
    } else if(fd == SYS_MOCK_STDERR_FILENO) {
      stderr_file.count = 1;
      stderr_file.is_static = 1;
      stderr_file.has_static_buf = 1;
      stderr_file.is_tty = 0;
      stderr_file.file_name = NULL;
      stderr_file.flags = O_WRONLY;
      stderr_file.mode = 0;
      stderr_file.buf = stderr_buf;
      stderr_file.buf_size = sizeof(stderr_buf);
      stderr_file.buf_data_pos = 0;
      stderr_file.buf_data_size = 0;
      memset(stderr_buf, 0, sizeof(stderr_buf));
      sys_mock_files[fd] = &stderr_file;
      i++;
    } else
      sys_mock_files[fd] = NULL;
  }
  sys_mock_break = sys_mock_data_seg;
  for(i = 0; i < MAX_SYS_MOCK_MEM_AREA_COUNT; i++) {
    sys_mock_mem_areas[i].ptr = NULL;
    sys_mock_mem_areas[i].ptr_to_free = NULL;
    sys_mock_mem_areas[i].len = 0;
  }
  sys_mock_mmap_res = (void *) 0;      
  sys_mock_mmap_err_num = 0;
  sys_mock_munmap_res = 0;
  sys_mock_munmap_err_num = 0;
  sys_mock_lstat_res = 0;
  sys_mock_lstat_err_num = 0;
  memset(&sys_mock_lstat_buf, 0, sizeof(struct stat));
  sys_mock_waitpid_res = 0;
  sys_mock_waitpid_err_num = 0;
  sys_mock_waitpid_status = 0;
  sys_mock_open_res = 0 ;
  sys_mock_open_err_num = 0;
  sys_mock_close_res = 0;
  sys_mock_close_err_num = 0;
  sys_mock_execve_err_num = 0;
  sys_mock_fork_res = 0;
  sys_mock_fork_err_num = 0;
  sys_mock_lseek_res = 0;
  sys_mock_lseek_err_num = 0;
  sys_mock_pipe_res = 0;
  sys_mock_pipe_err_num = 0;
  sys_mock_read_res = 0;
  sys_mock_read_err_num = 0;
  sys_mock_rmdir_res = 0;
  sys_mock_rmdir_err_num = 0;
  sys_mock_sbrk_res = (void *) 0;
  sys_mock_sbrk_err_num = 0;
  sys_mock_write_res = 0;
  sys_mock_write_err_num = 0;
  sys_mock_unlink_res = 0;
  sys_mock_unlink_err_num = 0;
  uportlibc_sys_mock_environ = static_environ;
  errno = 0;
}

void final_sys_mock(void)
{
  int fd, i;
  for(fd = 0; fd < MAX_SYS_MOCK_FILE_COUNT; fd++) {
    if(sys_mock_files[fd] != NULL) {
      sys_mock_files[fd]->count--;
      if(sys_mock_files[fd]->count <= 0) {
        if(!sys_mock_files[fd]->has_static_buf) free(sys_mock_files[fd]->buf);
        if(!sys_mock_files[fd]->is_static) free(sys_mock_files[fd]);
      }
    }
  }
  for(i = 0; i < MAX_SYS_MOCK_MEM_AREA_COUNT; i++) {
    if(sys_mock_mem_areas[i].ptr_to_free != NULL)
      free(sys_mock_mem_areas[i].ptr_to_free);
  }
}

void clear_sys_mock(void)
{
  final_sys_mock();
  init_sys_mock();
}

int add_sys_mock_op(const struct sys_mock_op *op)
{
  if(sys_mock_op_count >= MAX_SYS_MOCK_OP_COUNT) {
    errno = ENOMEM;
    return -1;
  }
  sys_mock_ops[sys_mock_op_count] = *op;
  sys_mock_op_count++;
  return 0;
}

int add_sys_mock_file(const char *file_name, int flags, mode_t mode)
{
  int fd, i;
  for(fd = 0; fd < MAX_SYS_MOCK_FILE_COUNT; fd++) {
    if(sys_mock_files[fd] == NULL) {
      struct sys_mock_file *file; 
      char *buf;
      file = (struct sys_mock_file *) malloc(sizeof(struct sys_mock_file));
      if(file == NULL) {
        errno = ENOMEM;
        return -1;
      }
      buf = (char *) malloc(sys_mock_file_buf_size);
      if(buf == NULL) {
        errno = ENOMEM;
        return -1;
      }
      memset(buf, 0, sys_mock_file_buf_size);
      file->count = 1;
      file->is_static = 0;
      file->has_static_buf = 0;
      file->is_tty = sys_mock_file_is_tty;
      file->file_name = file_name;
      file->flags = flags;
      file->mode = mode;
      file->buf = buf;
      file->buf_size = sys_mock_file_buf_size;
      file->buf_data_pos = 0;
      file->buf_data_size = 0;
      sys_mock_files[fd] = file;
      return fd;
    }
  }
  errno = ENFILE;
  return -1;
}

int delete_sys_mock_file(int fd)
{
  if(is_opened_sys_mock_file(fd)) {
    sys_mock_files[fd]->count--;
    if(sys_mock_files[fd]->count <= 0) {
      if(!sys_mock_files[fd]->has_static_buf) free(sys_mock_files[fd]->buf);
      if(!sys_mock_files[fd]->is_static) free(sys_mock_files[fd]);
    }
    sys_mock_files[fd] = NULL;
    return 0;
  } else {
    errno = EBADF;
    return -1;
  }
}

int dup_sys_mock_file(int old_fd, int new_fd)
{
  if(is_opened_sys_mock_file(old_fd) && new_fd >= 0 && new_fd < MAX_SYS_MOCK_FILE_COUNT) {
    sys_mock_files[old_fd]->count++;
    delete_sys_mock_file(new_fd);
    sys_mock_files[new_fd] = sys_mock_files[old_fd]; 
    return 0;
  } else {
    errno = EBADF;
    return -1;
  }
}

int is_opened_sys_mock_file(int fd)
{ return fd >= 0 && fd < MAX_SYS_MOCK_FILE_COUNT && sys_mock_files[fd] != NULL; }

void *uportlibc_sys_mock_mmap(void *addr, size_t len, int prot, int flags, int fd, off_t offset)
{
  struct sys_mock_op op;
  void *res;
  op.fun_name = "mmap";
  op.args[0].vp = addr;
  op.args[1].u = len;
  op.args[2].i = prot;
  op.args[3].i = flags;
  op.args[4].i = fd;
  op.args[5].o = offset;
  if(sys_mock_mmap_res != MAP_FAILED) {
    int i, page_size = getpagesize();
    len = ((len + page_size - 1) / page_size) * page_size;
    for(i = 0; i < MAX_SYS_MOCK_MEM_AREA_COUNT; i++) {
      if(sys_mock_mem_areas[i].ptr == NULL) break;
    }
    if(i < MAX_SYS_MOCK_MEM_AREA_COUNT) {
      void *ptr = malloc(len + page_size);
      if(ptr != NULL) {
        sys_mock_mem_areas[i].ptr = (char *) (((((unsigned long) ptr) + page_size - 1) / page_size) * page_size);
        sys_mock_mem_areas[i].ptr_to_free = ptr;
        sys_mock_mem_areas[i].len = len;
        res = sys_mock_mem_areas[i].ptr;
        op.err_num = 0;
      } else {
        errno = ENOMEM;
        res = MAP_FAILED;
        op.err_num = errno;
      }
    } else {
      errno = ENOMEM;
      res = MAP_FAILED;
      op.err_num = errno;
    }
  } else {
    errno = sys_mock_mmap_err_num;
    res = sys_mock_mmap_res;
    op.err_num = errno;
  }
  op.res.vp = res;
  if(add_sys_mock_op(&op) == -1) return MAP_FAILED;
  return res;
}

int uportlibc_sys_mock_munmap(void *addr, size_t len)
{
  struct sys_mock_op op;
  int res;
  op.fun_name = "munmap";
  op.args[0].vp = addr;
  op.args[1].u = len;
  if(sys_mock_munmap_res != -1) {
    int i, page_size = getpagesize();
    len = ((len + page_size - 1) / page_size) * page_size;
    for(i = 0; i < MAX_SYS_MOCK_MEM_AREA_COUNT; i++) {
      if(sys_mock_mem_areas[i].ptr == ((char *) addr)) {
        break;
      }
    }
    if(i < MAX_SYS_MOCK_MEM_AREA_COUNT) {
      if(sys_mock_mem_areas[i].len == len) {
        free(sys_mock_mem_areas[i].ptr_to_free);
        sys_mock_mem_areas[i].ptr = NULL;
        sys_mock_mem_areas[i].ptr_to_free = NULL;
        sys_mock_mem_areas[i].len = 0;
      }
    }
    res = 0;
    op.err_num = 0;
  } else {
    errno = sys_mock_munmap_err_num;
    res = sys_mock_munmap_res;
    op.err_num = errno;
  }
  op.res.i = res;
  if(add_sys_mock_op(&op) == -1) return -1;
  return res;
}

int uportlibc_sys_mock_lstat(const char *path_name, struct stat *buf)
{
  struct sys_mock_op op;
  int res;
  op.fun_name = "lstat";
  op.args[0].cp = (char *) path_name;
  op.args[1].vp = buf;
  if(sys_mock_lstat_res != -1) {
    *buf = sys_mock_lstat_buf;
    res = 0;
    op.err_num = 0;
  } else {
    errno = sys_mock_lstat_err_num;
    res = sys_mock_lstat_res;
    op.err_num = errno;
  }
  op.res.i = res;
  if(add_sys_mock_op(&op) == -1) return -1;
  return res;
}

pid_t uportlibc_sys_mock_waitpid(pid_t pid, int *status, int options)
{
  struct sys_mock_op op;
  int res = 0;
  op.fun_name = "waitpid";
  op.args[0].i = pid;
  op.args[1].ip = status;
  op.args[2].i = options;
  if(sys_mock_waitpid_res != -1) {
    res = sys_mock_waitpid_res;
    *status = sys_mock_waitpid_status;
    op.err_num = 0;
  } else {
    errno = sys_mock_waitpid_err_num;
    res = -1;
    op.err_num = errno;
  }
  op.res.i = res;
  if(add_sys_mock_op(&op) == -1) return -1;
  return res;
}

int uportlibc_sys_mock_open(const char *file_name, int flags, ...)
{
  va_list ap;
  mode_t mode;
  struct sys_mock_op op;
  int fd;
  va_start(ap, flags);
  mode = va_arg(ap, unsigned);
  va_end(ap);
  op.fun_name = "open";
  op.args[0].cp = (char *) file_name;
  op.args[1].i = flags;
  op.args[2].i = mode;
  if(sys_mock_open_res != -1) {
    fd = add_sys_mock_file(file_name, flags, mode);
    op.err_num = 0;
  } else {
    errno = sys_mock_open_err_num;
    fd = sys_mock_open_res;
    op.err_num = errno;
  }
  op.res.i = fd;
  sys_mock_last_opened_fd = fd;
  if(add_sys_mock_op(&op) == -1) return -1;
  return fd;
}

int uportlibc_sys_mock__exit(int status)
{
  struct sys_mock_op op;
  int res = 0;
  op.fun_name = "_exit";
  op.args[0].i = status;
  op.res.i = res;
  op.err_num = 0;
  if(add_sys_mock_op(&op) == -1) return -1;
  return res;
}

int uportlibc_sys_mock_close(int fd)
{
  struct sys_mock_op op;
  int res = 0;
  op.fun_name = "close";
  op.args[0].i = fd;
  if(sys_mock_close_res != -1) {
    if(delete_sys_mock_file(fd) == -1) res = -1;
    op.err_num = 0;
  } else {
    errno = sys_mock_close_err_num;
    res = -1;
    op.err_num = errno;
  }
  op.res.i = res;
  if(add_sys_mock_op(&op) == -1) return -1;
  return res;
}

int uportlibc_sys_mock_dup2(int old_fd, int new_fd)
{
  struct sys_mock_op op;
  int res = 0;
  op.fun_name = "dup2";
  op.args[0].i = old_fd;
  op.args[1].i = new_fd;
  if(dup_sys_mock_file(old_fd, new_fd) != -1) {
    op.err_num = 0;
  } else {
    res = -1;
    op.err_num = errno;
  }
  op.res.i = res;
  if(add_sys_mock_op(&op) == -1) return -1;
  return res;
}

int uportlibc_sys_mock_execve(const char *file_name, char *const *argv, char *const *env)
{
  struct sys_mock_op op;
  pid_t res = -1;
  op.fun_name = "execve";
  op.args[0].cp = (char *) file_name;
  op.args[1].vp = (void *) argv;
  op.args[2].vp = (void *) env;
  op.res.i = res;
  op.err_num = sys_mock_execve_err_num;
  errno = op.err_num;
  if(add_sys_mock_op(&op) == -1) return -1;
  return res;
}

pid_t uportlibc_sys_mock_fork(void)
{
  struct sys_mock_op op;
  pid_t res = sys_mock_fork_res;
  op.fun_name = "fork";
  op.res.i = res;
  if(res == -1) {
    op.err_num = sys_mock_fork_err_num;
    errno = op.err_num;
  } else
    op.err_num = 0;
  if(add_sys_mock_op(&op) == -1) return -1;
  return res;
}

int uportlibc_sys_mock_getpagesize(void)
{
  struct sys_mock_op op;
  int res = getpagesize();
  op.fun_name = "getpagesize";
  op.res.i = res;
  op.err_num = 0;
  if(add_sys_mock_op(&op) == -1) return -1;
  return res;  
}

int uportlibc_sys_mock_isatty(int fd)
{
  struct sys_mock_op op;
  int res;
  op.fun_name = "isatty";
  op.args[0].i = fd;
  if(is_opened_sys_mock_file(fd)) {
    res = sys_mock_files[fd]->is_tty;
    op.err_num = 0;
  } else {
    errno = EBADF;
    res = -1;
    op.err_num = errno;
  }
  op.res.i = res;
  if(add_sys_mock_op(&op) == -1) return -1;
  return res;
}

off_t uportlibc_sys_mock_lseek(int fd, off_t offset, int whence)
{
  struct sys_mock_op op;
  off_t res;
  op.fun_name = "lseek";
  op.args[0].i = fd;
  op.args[1].i = offset;
  op.args[2].i = whence;
  if(sys_mock_lseek_res != -1) {
    if(is_opened_sys_mock_file(fd)) {
      switch(whence) {
      case SEEK_SET:
        res = offset;
        break;
      case SEEK_CUR:
        res = sys_mock_files[fd]->buf_data_pos + offset;
        break;
      case SEEK_END:
        res = sys_mock_files[fd]->buf_data_size + offset;
        break;
      default:
        errno = EINVAL;
        res = -1;
        break;
      }
      if(res != -1) {
        if(res < 0 || (res >= ((off_t) sys_mock_files[fd]->buf_size))) {
          errno = EOVERFLOW;
          res = -1;
        }
      }
      if(res != -1) {
        sys_mock_files[fd]->buf_data_pos = res;
        op.err_num = 0;
      } else
        op.err_num = errno;
    } else {
      errno = EBADF;
      res = -1;
      op.err_num = errno;
    }
  } else {
    errno = sys_mock_lseek_err_num;
    res = -1;
    op.err_num = errno;
  }
  op.res.i = res;
  if(add_sys_mock_op(&op) == -1) return -1;
  return res;
}

int uportlibc_sys_mock_pipe(int *fds)
{
  struct sys_mock_op op;
  size_t res;
  op.fun_name = "pipe";
  op.args[0].ip = fds;
  if(sys_mock_pipe_res != -1) {
    fds[0] = add_sys_mock_file(NULL, O_RDONLY, 0);
    fds[1] = add_sys_mock_file(NULL, O_WRONLY, 0);
    res = 0;
    op.err_num = 0;
  } else {
    errno = sys_mock_pipe_err_num;
    res = -1;
    op.err_num = errno;
  }
  op.res.i = res;
  if(add_sys_mock_op(&op) == -1) return -1;
  return res;
}

ssize_t uportlibc_sys_mock_read(int fd, void *buf, size_t count)
{
  struct sys_mock_op op;
  size_t res;
  op.fun_name = "read";
  op.args[0].i = fd;
  op.args[1].vp = buf;
  op.args[2].i = count;
  if(sys_mock_read_res != -1) {
    if(is_opened_sys_mock_file(fd)) {
      off_t old_pos = sys_mock_files[fd]->buf_data_pos;
      if(sys_mock_files[fd]->buf_data_pos + ((off_t) count) < sys_mock_files[fd]->buf_data_size)
        sys_mock_files[fd]->buf_data_pos += (off_t) count;
      else
        sys_mock_files[fd]->buf_data_pos = sys_mock_files[fd]->buf_data_size;
      res = sys_mock_files[fd]->buf_data_pos - old_pos;
      memcpy(buf, sys_mock_files[fd]->buf + old_pos, res);
      op.err_num = 0;
    } else {
      errno = EBADF;
      res = -1;
      op.err_num = errno;
    }
  } else {
    errno = sys_mock_read_err_num;
    res = -1;
    op.err_num = errno;
  }
  op.res.i = res;
  op.err_num = errno;
  if(add_sys_mock_op(&op) == -1) return -1;
  return res;
}

int uportlibc_sys_mock_rmdir(const char *dir_name)
{
  struct sys_mock_op op;
  int res;
  op.fun_name = "rmdir";
  op.args[0].cp = (char *) dir_name;
  if(sys_mock_rmdir_res != -1) {
    res = 0;
    op.err_num = 0;
  } else {
    res = sys_mock_rmdir_res;
    errno = sys_mock_rmdir_err_num;
    op.err_num = errno;
  }
  op.res.i = res;
  if(add_sys_mock_op(&op) == -1) return -1;
  return res;
}

void *uportlibc_sys_mock_sbrk(intptr_t incr)
{
  struct sys_mock_op op;
  void *res;
  op.fun_name = "sbrk";
  op.args[0].i = incr;
  if(incr != 0) {
    if(sys_mock_sbrk_res != ((void *) (-1))) {
      if(sys_mock_break + incr <= sys_mock_data_seg + MAX_SYS_MOCK_DATA_SEG_SIZE) {
        res = (void *) sys_mock_break;
        op.err_num = 0;
        sys_mock_break += incr;
      } else {
        res = (void *) (-1);
        errno = ENOMEM;
        op.err_num = errno;
      }
    } else {
      res = sys_mock_sbrk_res;
      errno = sys_mock_sbrk_err_num;
      op.err_num = errno;
    }
  } else {
    res = (void *) sys_mock_break;
    op.err_num = 0;
  }
  op.res.vp = res;
  if(add_sys_mock_op(&op) == -1) return (void *) (-1);
  return res;
}

ssize_t uportlibc_sys_mock_write(int fd, const void *buf, size_t count)
{
  struct sys_mock_op op;
  ssize_t res;
  op.fun_name = "write";
  op.args[0].i = fd;
  op.args[1].vp = (void *) buf;
  op.args[2].i = count;
  if(sys_mock_write_res != -1) {
    if(is_opened_sys_mock_file(fd)) {
      off_t old_pos = sys_mock_files[fd]->buf_data_pos;
      if(sys_mock_files[fd]->buf_data_pos + ((off_t) count) < sys_mock_files[fd]->buf_size) {
        sys_mock_files[fd]->buf_data_pos += (off_t) count;
        res = sys_mock_files[fd]->buf_data_pos - old_pos;
        memcpy(sys_mock_files[fd]->buf + old_pos, buf, res);
        if(sys_mock_files[fd]->buf_data_pos >= sys_mock_files[fd]->buf_data_size)
          sys_mock_files[fd]->buf_data_size = sys_mock_files[fd]->buf_data_pos;
        op.err_num = 0;
      } else {
        errno = EOVERFLOW;
        res = -1;
        op.err_num = errno;
      }
    } else {
      errno = EBADF;
      res = -1;
      op.err_num = errno;
    }
  } else {
    errno = sys_mock_write_err_num;
    res = -1;
    op.err_num = errno;
  }
  op.res.i = res;
  if(add_sys_mock_op(&op) == -1) return -1;
  return res;
}

int uportlibc_sys_mock_unlink(const char *file_name)
{
  struct sys_mock_op op;
  int res;
  op.fun_name = "unlink";
  op.args[0].cp = (char *) file_name;
  if(sys_mock_unlink_res != -1) {
    res = 0;
    op.err_num = 0;
  } else {
    res = sys_mock_unlink_res;
    errno = sys_mock_unlink_err_num;
    op.err_num = errno;
  }
  op.res.i = res;
  if(add_sys_mock_op(&op) == -1) return -1;
  return res;
}
