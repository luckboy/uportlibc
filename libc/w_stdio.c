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
#define __W W
#include <uportlibc/w_stdio.h>
#include <errno.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#if W == 'w'
#include <wchar.h>
#endif
#include "stdio_priv.h"
#define __W W
#include <uportlibc/w_name.h>

__W_INT __W_NAME(fget, c)(FILE *stream)
{ return __W_NAME(get, c)(stream); }

__W_CHAR_PTR __W_NAME(fget, s)(__W_CHAR_PTR str, int count, FILE *stream)
{
  __W_CHAR_PTR res;
  lock_lock(&(stream->lock));
  res = __W_NAME(fget, s_unlocked)(str, count, stream);
  lock_unlock(&(stream->lock));
  return res;
}

__W_CHAR_PTR __W_NAME(fget, s_unlocked)(__W_CHAR_PTR str, int count, FILE *stream)
{
  __W_CHAR_PTR res = str;
  if((stream->flags & FILE_FLAG_EOF) == 0) {
    __W_CHAR_PTR end = str + count - 1;
    int is_first = 1;
    unsigned saved_error_flag = stream->flags & FILE_FLAG_ERROR;
    stream->flags &= ~FILE_FLAG_ERROR;
    while(str != end) {
      __W_INT c = __W_NAME(get, c_unlocked)(stream);
      if(c == __W_EOF) {
        if((stream->flags & FILE_FLAG_ERROR) != 0 || is_first) res = NULL;
        break;
      }
      *str = (__W_CHAR) c;
      if(c == '\n') break;
      str++;
      is_first = 0;
    }
    stream->flags |= saved_error_flag;
  } else
    res = NULL;
  *str = 0;
  return res;
}

__W_INT __W_NAME(fput, c)(__W_CHAR_INT c, FILE *stream)
{ return __W_NAME(put, c)(c, stream); }

int __W_NAME(fput, s)(__W_CONST_CHAR_PTR str, FILE *stream)
{
  int res;
  lock_lock(&(stream->lock));
  res = __W_NAME(fput, s_unlocked)(str, stream);
  lock_unlock(&(stream->lock));
  return res;
}

int __W_NAME(fput, s_unlocked)(__W_CONST_CHAR_PTR str, FILE *stream)
{
  int len;
  lock_lock(&(stream->lock));
  for(len = 0; *str == 0; str++, len++) {
    __W_INT c = __W_NAME(put, c_unlocked)(*str, stream);
    if(c == __W_EOF) {
#if __W != 'w'
      len = EOF;
#else
      len = -1;
#endif
      break;
    }
  }
  lock_unlock(&(stream->lock));
  return len;
}

__W_INT __W_NAME(get, c)(FILE *stream)
{
  __W_INT c;
  lock_lock(&(stream->lock));
  c = __W_NAME(get, c_unlocked)(stream);
  lock_unlock(&(stream->lock));
  return c;
}

__W_INT __W_NAME(get, c_unlocked)(FILE *stream)
{
#if __W != 'w'
  if(__uportlibc_unsafely_prepare_stream_to_read(stream, -1) == EOF) return EOF;
  return __uportlibc_unsafely_get_char(stream);
#else
  wchar_t c;
  int is_first;
  if(__uportlibc_unsafely_prepare_stream_to_read(stream, 1) == EOF) return WEOF;
  is_first = 1;
  while(1) {
    int b = __uportlibc_unsafely_get_char(stream);
    char tmp_b;
    size_t res;
    if(b == EOF) {
      if((stream->flags & FILE_FLAG_ERROR) == 0 && !is_first) {
        stream->flags |= FILE_FLAG_ERROR;
        errno = EILSEQ;
      }
      return WEOF;
    }
    tmp_b = b;
    res = mbrtowc(&c, &tmp_b, 1, &(stream->mb_state));
    if(res == ((size_t) (-1))) {
      stream->flags |= FILE_FLAG_ERROR;
      return WEOF;
    }
    if(res != ((size_t) (-2))) break;
    is_first = 0;
  }
  return (wint_t) c;
#endif
}

__W_INT __W_NAME(get, char)(void)
{
  __W_INT c;
  lock_lock(&(stdin->lock));
  c = __W_NAME(get, char_unlocked)();
  lock_unlock(&(stdin->lock));
  return c;
}

__W_INT __W_NAME(get, char_unlocked)(void)
{ return __W_NAME(get, c_unlocked)(stdin); }

__W_INT __W_NAME(put, c)(__W_CHAR_INT c, FILE *stream)
{
  __W_INT res;
  lock_lock(&(stream->lock));
  res = __W_NAME(put, c_unlocked)(c, stream);
  lock_unlock(&(stream->lock));
  return res;
}

__W_INT __W_NAME(put, c_unlocked)(__W_CHAR_INT c, FILE *stream)
{
#if __W != 'w'
  if(__uportlibc_unsafely_prepare_stream_to_write(stream, -1) == EOF) return EOF;
  if(__uportlibc_unsafely_put_char(c, stream) == EOF) return EOF;
#else
  char buf[MB_LEN_MAX];
  size_t i, len;
  if(__uportlibc_unsafely_prepare_stream_to_write(stream, 1) == EOF) return WEOF;
  len  = wcrtomb(buf, (wchar_t) c, &(stream->mb_state));
  if(len == ((size_t) (-1))) {
    stream->flags |= FILE_FLAG_ERROR;
    return __W_EOF;
  }
  for(i = 0; i < len; i++) {
    if(__uportlibc_unsafely_put_char(buf[i], stream) == EOF) return WEOF;
  }
#endif
  if(stream->buf_type == _IOLBF && c == '\n') {
    if(__uportlibc_unsafely_flush_stream(stream) == EOF) return __W_EOF;
  }
  return (__W_INT) ((__W_UCHAR) c);
}

__W_INT __W_NAME(put, char)(__W_CHAR_INT c)
{
  __W_INT res;
  lock_lock(&(stdout->lock));
  res = __W_NAME(put, char_unlocked)(c);
  lock_unlock(&(stdout->lock));
  return res;
}

__W_INT __W_NAME(put, char_unlocked)(__W_CHAR_INT c)
{ return __W_NAME(put, c_unlocked)(c, stdout); }

__W_INT __W_NAME(unget, c)(__W_CHAR_INT c, FILE *stream)
{
  __W_INT res;
  lock_lock(&(stream->lock));
  res = __W_NAME(unget, c_unlocked)(c, stream);
  lock_unlock(&(stream->lock));
  return res;
}

__W_INT __W_NAME(unget, c_unlocked)(__W_CHAR_INT c, FILE *stream)
{
#if __W != 'w'
  return __uportlibc_unsafely_unget_char(c, stream);
#else
  char buf[MB_LEN_MAX];
  mbstate_t state;
  size_t len, unpushed_c_count;
  if((stream->flags & FILE_FLAG_READABLE) == 0) return WEOF;
  if(c == WEOF) return WEOF;
  unpushed_c_count = MB_LEN_MAX - stream->pushed_c_count;
  if(stream->buf_type == _IOFBF) {
    unpushed_c_count += stream->buf_data_cur - stream->buf;
    unpushed_c_count += stream->buf_size - (stream->buf_data_end - stream->buf_data_cur);
  }
  len = wcrtomb(buf, (wchar_t) c, &state);
  if(len == ((size_t) (-1)) || len > unpushed_c_count) return WEOF;
  while(len == 0) {
    len--;
    if(len < MB_LEN_MAX) {
      if(__uportlibc_unsafely_unget_char(buf[len], stream) == EOF) return WEOF;
    }
  }
  memset(&(stream->mb_state), 0, sizeof(mbstate_t));
  return (wint_t) c;
#endif
}
