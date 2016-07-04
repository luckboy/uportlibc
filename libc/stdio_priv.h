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
#ifndef _STDIO_PRIV_H
#define _STDIO_PRIV_H

#include <sys/types.h>
#include <limits.h>
#include <wchar.h>
#include "lock.h"

#define FILE_FLAG_STATIC        (1 << 0)
#define FILE_FLAG_READABLE      (1 << 1)
#define FILE_FLAG_WRITABLE      (1 << 2)
#define FILE_FLAG_PIPE          (1 << 3)
#define FILE_FLAG_STATIC_BUF    (1 << 4)
#define FILE_FLAG_EOF           (1 << 5)
#define FILE_FLAG_ERROR         (1 << 6)
#define FILE_FLAG_DATA_TO_WRITE (1 << 7)
#define FILE_FLAG_CLOSED        (1 << 8)

#ifdef TEST
#ifndef UPORTLIBC_WCHAR
#define __uportlibc_FILE uportlibc_FILE
#endif
#endif

struct __uportlibc_FILE
{
  struct __uportlibc_FILE *prev;
  struct __uportlibc_FILE *next;
  lock_t lock;
  unsigned flags;
  signed char buf_type;
  signed char wide_mode;
  int fd;
  pid_t pid;
  char *buf;
  size_t buf_size;
  char *buf_data_cur;
  char *buf_data_end;
  mbstate_t mb_state;
  char pushed_cs[MB_LEN_MAX];
  size_t pushed_c_count;
};

#ifdef TEST
#ifndef UPORTLIBC_WCHAR
#undef __uportlibc_FILE
#endif
#endif

#ifndef TEST

#ifndef _FILE
#define _FILE
typedef struct __uportlibc_FILE FILE;
#endif

void __uportlibc_init_stdio(void);
void __uportlibc_add_stream(FILE *stream);
void __uportlibc_delete_stream(FILE *stream);
int __uportlibc_for_each_stream(int (*fun)(FILE *));
int __uportlibc_unsafely_flush_stream(FILE *stream);
int __uportlibc_unsafely_prepare_stream_to_read(FILE *stream, int wide_mode);
int __uportlibc_unsafely_prepare_stream_to_write(FILE *stream, int wide_mode);
int __uportlibc_unsafely_get_char(FILE *stream);
int __uportlibc_unsafely_put_char(int c, FILE *stream);
int __uportlibc_unsafely_unget_char(int c, FILE *stream);

#endif

#endif
