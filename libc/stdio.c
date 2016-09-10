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
#include <sys/stat.h>
#include <errno.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "stdio_priv.h"

union word
{
  char bytes[WORD_BIT / 8];
  int word;
};

static off_t unsafely_get_stream_offset(FILE *stream)
{
  off_t offset = lseek(stream->fd, 0, SEEK_CUR);
  if(offset == -1) return -1;
  if((stream->flags & FILE_FLAG_DATA_TO_WRITE) != 0)
    return offset + (stream->buf_data_end - stream->buf_data_cur);
  else
    return offset - (stream->buf_data_end - stream->buf_data_cur);
}

static int unsafely_set_stream_offset(FILE *stream, off_t offset, int whence)
{
  switch(whence) {
  case SEEK_SET:
    break;
  case SEEK_CUR:
    if((stream->flags & FILE_FLAG_DATA_TO_WRITE) == 0)
      offset -= stream->buf_data_end - stream->buf_data_cur;
    break;
  case SEEK_END:
    break;
  default:
    errno = EINVAL;
    return -1;
  }
  if(__uportlibc_unsafely_flush_stream(stream) == EOF) return EOF;
  if(lseek(stream->fd, offset, whence) == -1) return EOF;
  stream->flags &= ~(FILE_FLAG_EOF | FILE_FLAG_DATA_TO_WRITE);
  stream->buf_data_cur = stream->buf;
  stream->buf_data_end = stream->buf;
  stream->pushed_c_count = 0;
  return 0;
}

void clearerr(FILE *stream)
{
  lock_lock(&(stream->lock));
  stream->flags &= ~FILE_FLAG_ERROR;
  lock_unlock(&(stream->lock));
}

int feof(FILE *stream)
{
  int res;
  lock_lock(&(stream->lock));
  res = ((stream->flags & ~FILE_FLAG_EOF) != 0);
  lock_unlock(&(stream->lock));
  return res;
}

int ferror(FILE *stream)
{
  int res;
  lock_lock(&(stream->lock));
  res = ((stream->flags & ~FILE_FLAG_ERROR) != 0);
  lock_unlock(&(stream->lock));
  return res;
}

int fflush(FILE *stream)
{
  if(stream != NULL) {
    int res;
    lock_lock(&(stream->lock));
    res = __uportlibc_unsafely_flush_stream(stream);
    lock_unlock(&(stream->lock));
    return res;
  } else 
    return __uportlibc_for_each_stream(fflush);
}

int fgetpos(FILE *stream, fpos_t *pos)
{
  int res;
  lock_lock(&(stream->lock));
  do {
    off_t offset = unsafely_get_stream_offset(stream);
    if(offset == -1) {
      res = -1;
      break;
    }
    pos->offset = offset;
    pos->mb_state = stream->mb_state;
    res = 0;
  } while(0);
  lock_unlock(&(stream->lock));
  return res;
}

int fileno(FILE *stream)
{
  int fd;
  lock_lock(&(stream->lock));
  fd = stream->fd;
  lock_unlock(&(stream->lock));
  return fd;
}

void flockfile(FILE *stream)
{ lock_lock(&(stream->lock)); }

size_t fread(void *ptr, size_t elem_size, size_t elem_count, FILE *stream)
{
  char *buf = (char *) ptr;
  size_t i, count, read_byte_count;
  lock_lock(&(stream->lock));
  if(__uportlibc_unsafely_prepare_stream_to_read(stream, -1) == EOF) return 0;
  count = elem_size * elem_count;
  read_byte_count = 0;
  for(i = 0; i < stream->pushed_c_count && count > 0; i++) {
    *buf = stream->pushed_cs[i];
    buf++;
    count--;
    read_byte_count++;
  }
  if(stream->buf_type != _IONBF) {
    while(stream->buf_data_cur != stream->buf_data_end && count > 0) {
      *buf = *(stream->buf_data_cur);
      stream->buf_data_cur++;
      count--;
      read_byte_count++;
    }
    if(stream->buf_data_cur == stream->buf_data_end) {
      stream->buf_data_cur = stream->buf;
      stream->buf_data_end = stream->buf;
    }
  }
  while(count > 0) {
    size_t tmp_count = (count < SSIZE_MAX ? count : SSIZE_MAX);
    ssize_t res = read(stream->fd, buf, tmp_count);
    if(res == 0) {
      stream->flags |= FILE_FLAG_EOF;
      break;
    }
    if(res == -1) {
      stream->flags |= FILE_FLAG_ERROR;
      break;
    }
    buf += res;
    count -= res;
    read_byte_count += res;
  }
  lock_unlock(&(stream->lock));
  return read_byte_count / elem_size;
}

int fseek(FILE *stream, long offset, int whence)
{ return fseeko(stream, offset, whence); }

int fseeko(FILE *stream, off_t offset, int whence)
{
  int res;
  lock_lock(&(stream->lock));
  do {
    if(unsafely_set_stream_offset(stream, offset, whence) == -1) {
      res = -1;
      break;
    }
    memset(&(stream->mb_state), 0, sizeof(mbstate_t));
    res = 0;
  } while(0);
  lock_unlock(&(stream->lock));
  return res;
}

int fsetpos(FILE *stream, const fpos_t *pos)
{
  int res;
  lock_lock(&(stream->lock));
  do {
    if(unsafely_set_stream_offset(stream, pos->offset, SEEK_SET) == -1) {
      res = -1;
      break;
    }
    stream->mb_state = pos->mb_state;
    res = 0;
  } while(0);
  lock_unlock(&(stream->lock));
  return res;
}

long ftell(FILE *stream)
{
  off_t offset = ftello(stream);
  if(offset == -1) return -1;
  if(offset < 0 || offset > LONG_MAX) {
    errno = EOVERFLOW;
    return -1;
  }
  return (long) offset;
}

off_t ftello(FILE *stream)
{
  off_t offset;
  lock_lock(&(stream->lock));
  offset = unsafely_get_stream_offset(stream);
  lock_unlock(&(stream->lock));  
  return offset;
}

int ftrylockfile(FILE *stream)
{ return lock_try_lock(&(stream->lock)); }

void funlockfile(FILE *stream)
{ lock_unlock(&(stream->lock)); }

size_t fwrite(const void *ptr, size_t elem_size, size_t elem_count, FILE *stream)
{
  const char *buf = (const char *) ptr;
  size_t count, written_byte_count;
  lock_lock(&(stream->lock));
  if(__uportlibc_unsafely_prepare_stream_to_write(stream, -1) == EOF) return 0;
  if(__uportlibc_unsafely_flush_stream(stream) == EOF) return 0;
  count = elem_size * elem_count;
  written_byte_count = 0;
  while(count > 0) {
    size_t tmp_count = (count < SSIZE_MAX ? count : SSIZE_MAX);
    ssize_t res = write(stream->fd, buf, tmp_count);
    if(res == -1) {
      stream->flags |= FILE_FLAG_ERROR;
      break;
    }
    buf += res;
    count -= res;
    written_byte_count += res;
  }
  lock_unlock(&(stream->lock));
  return written_byte_count / elem_size;
}

int getw(FILE *stream)
{
  union word word_union;
  size_t i;
  int w;
  w = 0;
  lock_lock(&(stream->lock));
  for(i = 0; i < WORD_BIT / 8; i++) {
    int c = getc_unlocked(stream);
    if(c == EOF) {
      w = EOF;
      break;
    }
    word_union.bytes[i] = ((char) ((unsigned char) c));
  }
  lock_unlock(&(stream->lock));
  if(w != EOF) w = word_union.word;
  return w;
}

void perror(const char *str)
{
  fputs(str, stderr);
  fputs(": ", stderr);
  fputs(strerror(errno), stderr);
  fputc('\n', stderr);
}

int putw(int w, FILE *stream)
{
  union word word_union;
  size_t i;
  int res;
  word_union.word = w;
  res = 0;
  lock_lock(&(stream->lock));
  for(i = 0; i < WORD_BIT / 8; i++) {
    if(putc_unlocked(word_union.bytes[i], stream) == EOF) {
      res = EOF;
      break;
    }
  }
  lock_unlock(&(stream->lock));
  if(res != EOF) res = w;
  return res;
}

int puts(const char *str)
{
  int res;
  lock_lock(&(stdout->lock));
  do {
    res = fputs_unlocked(str, stdout);
    if(res == EOF) {
      res = EOF;
      break;
    }
    if(putchar_unlocked('\n') == EOF) {
      res = EOF;
      break;
    }
    res++;
  } while(0);
  lock_unlock(&(stdout->lock));
  return res;
}

int remove(const char *path_name)
{
  struct stat stat_buf;
  if(lstat(path_name, &stat_buf) == -1) return -1;
  if(S_ISDIR(stat_buf.st_mode) == 0)
    return unlink(path_name);
  else
    return rmdir(path_name);
}

void rewind(FILE *stream)
{ fseek(stream, 0, SEEK_SET); }
