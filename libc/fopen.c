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
#ifndef TEST
#include <sys/stat.h>
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#else
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#define SYS_MOCK_SYS_STAT
#define SYS_MOCK_SYS_WAIT
#define SYS_MOCK_FCNTL
#define SYS_MOCK_UNISTD
#define SYS_MOCK_ENVIRON
#include "sys_mock.h"
#define UPORTLIBC_EXIT
#define UPORTLIBC_STDIO
#define UPORTLIBC_W_STDIO
#include "uportlibc.h"
#endif
#include "exit.h"
#include "stdio_priv.h"

#ifndef TEST
#define AUTO
#else
#define AUTO                    static
#endif

extern char **environ;

static int unsafely_close_stream(FILE *stream, int *status, int can_free_buf)
{
  if((stream->flags & FILE_FLAG_CLOSED) != 0) {
    errno = EBADF;
    return EOF;
  }
  if(__uportlibc_unsafely_flush_stream(stream) == EOF) return EOF;
  if((stream->flags & FILE_FLAG_PIPE) != 0 && stream->pid != -1) {
    int res;
    do {
      res = waitpid(stream->pid, status, 0);
    } while(res == -1 && errno == EINTR);
    if(res == -1) return EOF;
    stream->pid = -1;
  }
  if(close(stream->fd) == -1) return EOF;
  if(can_free_buf && (stream->flags & FILE_FLAG_STATIC_BUF) == 0) free(stream->buf);
  stream->flags |= FILE_FLAG_CLOSED;
  return 0;
}

static void close_all_streams_for_exit(void)
{ __uportlibc_for_each_stream(fclose); }

static void set_stdio_exit_fun(void)
{
  lock_lock(&__uportlibc_exit_lock);
  __uportlibc_stdio_exit_fun = close_all_streams_for_exit;
  lock_unlock(&__uportlibc_exit_lock);
}  

static void set_stdio_exit_fun_and_add_stream(FILE *stream)
{
  set_stdio_exit_fun();
  __uportlibc_add_stream(stream);
}

static int parse_mode(const char *mode, unsigned *stream_flags)
{
  if(strcmp(mode, "a") == 0 || strcmp(mode, "ab") == 0) {
    *stream_flags = FILE_FLAG_WRITABLE;
    return O_WRONLY | O_APPEND | O_CREAT;
  } else if(strcmp(mode, "a+") == 0 || strcmp(mode, "a+b") == 0 || strcmp(mode, "ab+") == 0) {
    *stream_flags = FILE_FLAG_READABLE | FILE_FLAG_WRITABLE;
    return O_RDWR | O_APPEND | O_CREAT;
  } else if(strcmp(mode, "r") == 0 || strcmp(mode, "rb") == 0) {
    *stream_flags = FILE_FLAG_READABLE;
    return O_RDONLY;
  } else if(strcmp(mode, "r+") == 0 || strcmp(mode, "r+b") == 0 || strcmp(mode, "rb+") == 0) {
    *stream_flags = FILE_FLAG_READABLE | FILE_FLAG_WRITABLE;
    return O_RDWR;
  } else if(strcmp(mode, "w") == 0 || strcmp(mode, "wb") == 0) {
    *stream_flags = FILE_FLAG_WRITABLE;
    return O_WRONLY | O_CREAT | O_TRUNC;
  } else if(strcmp(mode, "w+") == 0 || strcmp(mode, "w+b") == 0 || strcmp(mode, "wb+") == 0) {
    *stream_flags = FILE_FLAG_READABLE | FILE_FLAG_WRITABLE;
    return O_RDWR | O_CREAT | O_TRUNC;
  } else {
    errno = EINVAL;
    return -1;
  }
}

static void init_stream(FILE *stream, int fd, unsigned flags, int buf_type, char *buf, pid_t pid)
{
  lock_init(&(stream->lock));
  stream->fd = fd;
  stream->flags = flags;
  stream->buf_type = buf_type;
  stream->wide_mode = 0;
  stream->pid = pid;
  stream->buf = buf;
  stream->buf_size = BUFSIZ;
  stream->buf_data_cur = buf;
  stream->buf_data_end = buf;
  memset(&(stream->mb_state), 0, sizeof(mbstate_t));
  stream->pushed_c_count = 0;
}

FILE *fdopen(int fd, const char *mode)
{
  FILE *stream;
  char *buf;
  unsigned stream_flags;
  int flags = parse_mode(mode, &stream_flags);
  if(flags == -1) return NULL;
  stream = (FILE *) malloc(sizeof(FILE));
  if(stream == NULL) {
    errno = ENOMEM;
    return NULL;
  }
  buf = (char *) malloc(BUFSIZ);
  if(buf == NULL) {
    errno = ENOMEM;
    free(buf);
    return NULL;
  }
  init_stream(stream, fd, stream_flags, _IOFBF, buf, -1);
  set_stdio_exit_fun_and_add_stream(stream);
  return stream;
}

FILE *fopen(const char *file_name, const char *mode)
{
  FILE *stream;
  char *buf;
  unsigned stream_flags;
  int fd, flags = parse_mode(mode, &stream_flags);
  if(flags == -1) return NULL;
  stream = (FILE *) malloc(sizeof(FILE));
  if(stream == NULL) {
    errno = ENOMEM;
    return NULL;
  }
  buf = (char *) malloc(BUFSIZ);
  if(buf == NULL) {
    errno = ENOMEM;
    free(buf);
    return NULL;
  }
  fd = open(file_name, flags, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH);
  if(fd == -1) {
    free(buf);
    free(stream);
    return NULL;
  }
  init_stream(stream, fd, stream_flags, (isatty(fd) ? _IOLBF : _IOFBF), buf, -1);
  set_stdio_exit_fun_and_add_stream(stream);
  return stream;
}

FILE *freopen(const char *file_name, const char *mode, FILE *stream)
{
  unsigned stream_flags;
  int fd, flags = parse_mode(mode, &stream_flags);
  if(flags == -1) return NULL;
  lock_lock(&(stream->lock));
  do {
    fd = open(file_name, flags, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH);
    if(fd == -1) {
      stream->flags |= FILE_FLAG_ERROR;
      stream = NULL;
      break;
    }
    if((flags & FILE_FLAG_CLOSED) == 0) {
      AUTO int status;
      if(unsafely_close_stream(stream, &status, 0) == EOF) {
        int saved_errno = errno;
        close(fd);
        errno = saved_errno;
        stream->flags |= FILE_FLAG_ERROR;
        stream = NULL;
        break;
      }
    }
    stream->flags &= ~(FILE_FLAG_READABLE | FILE_FLAG_WRITABLE | FILE_FLAG_PIPE);
    stream->flags &= ~(FILE_FLAG_EOF | FILE_FLAG_ERROR | FILE_FLAG_DATA_TO_WRITE | FILE_FLAG_CLOSED);
    stream->flags |= stream_flags;
    stream->wide_mode = 0;
    stream->fd = fd;
    stream->pid = -1;
    stream->buf_data_cur = stream->buf;
    stream->buf_data_end = stream->buf;
    memset(&(stream->mb_state), 0, sizeof(mbstate_t));
    stream->pushed_c_count = 0;
  } while(0);
  lock_unlock(&(stream->lock));
  return stream;
}

FILE *popen(const char *command, const char *mode)
{
  FILE *stream;
  char *buf;
  unsigned stream_flags;
  pid_t pid;
  AUTO int fds[2];
  int fd, saved_errno = errno;
  if(strcmp("r", mode) == 0 || strcmp("rb", mode) == 0) {
    stream_flags = FILE_FLAG_READABLE | FILE_FLAG_PIPE;
  } else if(strcmp("w", mode) == 0 || strcmp("wb", mode) == 0) {
    stream_flags = FILE_FLAG_WRITABLE | FILE_FLAG_PIPE;
  } else {
    errno = EINVAL;
    return NULL;
  }
  stream = (FILE *) malloc(sizeof(FILE));
  if(stream == NULL) {
    errno = saved_errno;
    return NULL;
  }
  buf = (char *) malloc(BUFSIZ);
  if(buf == NULL) {
    errno = saved_errno;
    free(buf);
    return NULL;
  }
  if(pipe(fds) == -1) {
    free(buf);
    free(stream);
    return NULL;
  }
  pid = fork();
  if(pid == 0) {
    AUTO char *argv[4];
    if((stream_flags & FILE_FLAG_READABLE) != 0) {
      close(fds[0]);
      dup2(fds[1], STDOUT_FILENO);
    } else {
      dup2(fds[0], STDIN_FILENO);
      close(fds[1]);
    }
    argv[0] = "sh";
    argv[1] = "-c";
    argv[2] = (char *) command;
    argv[3] = NULL;
    execve("/bin/sh", argv, environ);
    _exit(127);
    return NULL;
  } else if(pid != -1) {
    if((stream_flags & FILE_FLAG_READABLE) != 0) {
      fd = fds[0];
      close(fds[1]);
    } else {
      close(fds[0]);
      fd = fds[1];
    }
    init_stream(stream, fd, stream_flags, _IOFBF, buf, pid);
    set_stdio_exit_fun_and_add_stream(stream);
    return stream;  
  } else {
    int saved_errno = errno;
    close(fds[0]);
    close(fds[1]);
    errno = saved_errno;
    free(buf);
    free(stream);
    return NULL;
  }
}

int fclose(FILE *stream)
{
  int res;
  AUTO int status;
  lock_lock(&(stream->lock));
  res = unsafely_close_stream(stream, &status, 1);
  lock_unlock(&(stream->lock));
  lock_destroy(&(stream->lock));
  if(res != EOF) {
    __uportlibc_delete_stream(stream);
    if((stream->flags & FILE_FLAG_STATIC) == 0) free(stream);
  }
  return res;
}

int pclose(FILE *stream)
{
  AUTO int status;
  lock_lock(&(stream->lock));
  do {
    if((stream->flags & FILE_FLAG_PIPE) == 0) {
      errno = ECHILD;
      status = -1;
      break;
    }
    if(unsafely_close_stream(stream, &status, 1) == EOF) {
      status = -1;
      break;
    }
  } while(0);
  lock_unlock(&(stream->lock));
  lock_destroy(&(stream->lock));
  if(status != -1) {
    __uportlibc_delete_stream(stream);
    if((stream->flags & FILE_FLAG_STATIC) == 0) free(stream);
  }
  return status;
}
