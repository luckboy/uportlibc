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
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include "stdio_priv.h"

void setbuf(FILE *stream, char *buf)
{ setvbuf(stream, buf, (buf != NULL ? _IOFBF : _IONBF), BUFSIZ); }

int setvbuf(FILE *stream, char *buf, int type, size_t size)
{
  int res;
  if(type != _IONBF && type != _IOFBF && type != _IOLBF) return -1;
  lock_lock(&(stream->lock));
  do {
    if(stream->buf_type != _IONBF && (stream->flags & FILE_FLAG_STATIC_BUF) == 0)
      free(stream->buf);
    stream->buf_type = type;
    if(type != _IONBF) {
      if(buf != NULL) {
        stream->flags |= FILE_FLAG_STATIC_BUF;
        stream->buf = buf;
      } else {
        buf = (char *) malloc(size);
        if(buf == NULL) {
          errno = ENOMEM;
          res = -1;
          break;
        }
        stream->flags &= ~FILE_FLAG_STATIC_BUF;
        stream->buf = buf;
      }
    } else {
      stream->flags &= ~FILE_FLAG_STATIC_BUF;
      stream->buf = NULL;
    }
    stream->buf_size = size;
    stream->buf_data_cur = buf;
    stream->buf_data_end = buf;
    res = 0;
  } while(0);
  lock_unlock(&(stream->lock));
  return res;
}
