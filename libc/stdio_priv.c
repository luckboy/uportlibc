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
#include <sys/types.h>
#include <errno.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "stdio_priv.h"

static int unsafely_check_and_set_stream_wide_mode(FILE *stream, int mode)
{
  if(mode == 0) return 0;
  if(mode < 0) {
    if(stream->wide_mode > 0) {
      stream->flags |= FILE_FLAG_ERROR;
      errno = EBADF;
      return EOF;
    }
    if(stream->wide_mode == 0) stream->wide_mode = -1;
  } else if(mode > 0) {
    if(stream->wide_mode < 0) {
      stream->flags |= FILE_FLAG_ERROR;
      errno = EBADF;
      return EOF;
    }
    if(stream->wide_mode == 0) stream->wide_mode = 1;
  }
  return 0;
}

int __uportlibc_unsafely_flush_stream(FILE *stream)
{
  if(stream->buf_type != _IONBF) {
    if((stream->flags & FILE_FLAG_DATA_TO_WRITE) != 0) {
      while(stream->buf_data_cur != stream->buf_data_end) {
        size_t diff = stream->buf_data_end - stream->buf_data_cur;
        size_t count = (diff <= SSIZE_MAX ? diff : SSIZE_MAX);
        ssize_t res = write(stream->fd, stream->buf_data_cur, count);
        if(res == -1) {
          stream->flags |= FILE_FLAG_ERROR;
          return EOF;
        }
        stream->buf_data_cur += res;
      }
    }
    stream->flags &= ~FILE_FLAG_DATA_TO_WRITE;
    stream->buf_data_cur = stream->buf;
    stream->buf_data_end = stream->buf;
  }
  return 0;
}

int __uportlibc_unsafely_prepare_stream_to_read(FILE *stream, int wide_mode)
{
  if(unsafely_check_and_set_stream_wide_mode(stream, wide_mode) == EOF) return EOF;
  if((stream->flags & FILE_FLAG_READABLE) == 0) {
    stream->flags |= FILE_FLAG_ERROR;
    errno = EBADF;
    return EOF;
  }
  return __uportlibc_unsafely_flush_stream(stream);
}

int __uportlibc_unsafely_prepare_stream_to_write(FILE *stream, int wide_mode)
{
  if(unsafely_check_and_set_stream_wide_mode(stream, wide_mode) == EOF) return EOF;
  if((stream->flags & FILE_FLAG_WRITABLE) == 0) {
    stream->flags |= FILE_FLAG_ERROR;
    errno = EBADF;
    return EOF;
  }
  if(stream->buf_type != _IONBF) {
    if((stream->flags & FILE_FLAG_PIPE) == 0 && (stream->flags & FILE_FLAG_DATA_TO_WRITE) == 0) {
      off_t offset = -(stream->buf_data_end - stream->buf_data_cur);
      if(offset < 0) {
        int saved_errno = errno;
        if(lseek(stream->fd, offset, SEEK_CUR) == -1) {
          if(errno != ESPIPE) {
            stream->flags |= FILE_FLAG_ERROR;
            return EOF;
          }
        }
        errno = saved_errno;
      }
    }
    stream->flags |= FILE_FLAG_DATA_TO_WRITE;
    stream->buf_data_cur = stream->buf;
    stream->buf_data_end = stream->buf;
  }
  stream->pushed_c_count = 0;
  return 0;
}

int __uportlibc_unsafely_get_char(FILE *stream)
{
  char c;
  if((stream->flags & FILE_FLAG_EOF) != 0) return EOF;
  if(stream->pushed_c_count > 0) {
    stream->pushed_c_count--;
    return stream->pushed_cs[stream->pushed_c_count];
  }
  if(stream->buf_type == _IOFBF) {
    if(stream->buf_data_cur == stream->buf_data_end) {
      size_t buf_size = (stream->buf_size < SSIZE_MAX ? stream->buf_size : SSIZE_MAX);
      ssize_t res;
      stream->buf_data_cur = stream->buf;
      stream->buf_data_end = stream->buf;
      res = read(stream->fd, stream->buf, buf_size);
      if(res == 0) {
        stream->flags |= FILE_FLAG_EOF;
        return EOF;
      }
      if(res == -1) {
        stream->flags |= FILE_FLAG_ERROR;
        return EOF;
      }
      stream->buf_data_end += res;
    }
    c = *(stream->buf_data_cur);
    stream->buf_data_cur++;
  } else {
    ssize_t res = read(stream->fd, &c, 1);
    if(res == 0) {
      stream->flags |= FILE_FLAG_EOF;
      return EOF;
    }
    if(res == -1) {
      stream->flags |= FILE_FLAG_ERROR;
      return EOF;
    }
  }
  return (int) ((unsigned char) c);
}

int __uportlibc_unsafely_put_char(int c, FILE *stream)
{
  if(stream->buf_type == _IOFBF || stream->buf_type == _IOLBF) {
    char *buf_end = stream->buf + stream->buf_size;
    if(stream->buf_data_end == buf_end) {
      if(__uportlibc_unsafely_flush_stream(stream) == EOF) return EOF;
    }
    *(stream->buf_data_end) = c;
    stream->buf_data_end++;
    if(stream->buf_data_end == buf_end) {
      if(__uportlibc_unsafely_flush_stream(stream) == EOF) return EOF;
    }
  } else {
    char tmp_c = c;
    ssize_t res = write(stream->fd, &tmp_c, 1);
    if(res == -1) {
      stream->flags |= FILE_FLAG_ERROR;
      return EOF;
    }
  }
  return (int) ((unsigned char) c);
}

int __uportlibc_unsafely_unget_char(int c, FILE *stream)
{
  if((stream->flags & FILE_FLAG_READABLE) == 0) return EOF;
  if(c == EOF) return EOF;
  if(stream->pushed_c_count < MB_LEN_MAX) {
    stream->pushed_cs[stream->pushed_c_count] = c;
    stream->pushed_c_count++;
  } else {
    if(stream->buf_type == _IONBF || (stream->flags & FILE_FLAG_DATA_TO_WRITE) != 0) 
      return EOF;
    if(stream->buf != stream->buf_data_cur) {
      stream->buf_data_cur--;
      *(stream->buf_data_cur) = c;
    } else {
      if(stream->buf_data_end == stream->buf + stream->buf_size) return EOF;
      memmove(stream->buf + 1, stream->buf, stream->buf_data_end - stream->buf);
      stream->buf_data_end++;
      *(stream->buf) = c;
    }
  }
  stream->flags &= ~FILE_FLAG_EOF;
  return (int) ((unsigned char) c); 
}
