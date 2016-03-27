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
#include <stdio.h>
#include <unistd.h>
#include "stdio_priv.h"

static lock_t stdio_lock;
static FILE *stdio_first_stream = NULL;
static FILE *stdio_last_stream = NULL;

static char static_stdin_buf[BUFSIZ];
static char static_stdout_buf[BUFSIZ];
static char static_stderr_buf[BUFSIZ];

static FILE static_stdin = {
  .prev = NULL, 
  .next = NULL,
  .lock = LOCK,
  .flags = FILE_FLAG_STATIC | FILE_FLAG_READABLE | FILE_FLAG_STATIC_BUF,
  .buf_type = _IOFBF,
  .wide_mode = 0,
  .fd = STDIN_FILENO,
  .pid = -1,
  .buf = static_stdin_buf,
  .buf_size = BUFSIZ,
  .buf_data_cur = static_stdin_buf,
  .buf_data_end = static_stdin_buf,
  .pushed_c_count = 0
};

static FILE static_stdout = {
  .prev = NULL,
  .next = NULL,
  .lock = LOCK,
  .flags = FILE_FLAG_STATIC | FILE_FLAG_WRITABLE | FILE_FLAG_STATIC_BUF,
  .buf_type = _IOFBF,
  .wide_mode = 0,
  .fd = STDOUT_FILENO,
  .pid = -1,
  .buf = static_stdout_buf,
  .buf_size = BUFSIZ,
  .buf_data_cur = static_stdout_buf,
  .buf_data_end = static_stdout_buf,
  .pushed_c_count = 0
};

static FILE static_stderr = {
  .prev = NULL,
  .next = NULL,
  .lock = LOCK,
  .flags = FILE_FLAG_STATIC | FILE_FLAG_WRITABLE | FILE_FLAG_STATIC_BUF,
  .buf_type = _IOLBF,
  .wide_mode = 0,
  .fd = STDERR_FILENO,
  .pid = -1,
  .buf = static_stderr_buf,
  .buf_size = BUFSIZ,
  .buf_data_cur = static_stderr_buf,
  .buf_data_end = static_stderr_buf,
  .pushed_c_count = 0
};

FILE *stdin = &static_stdin;
FILE *stdout = &static_stdout;
FILE *stderr = &static_stderr;

void __uportlibc_init_stdio(void)
{
  char *ptr, *end;
  ptr = (char *) (&(stdin->mb_state));
  end = ptr + sizeof(mbstate_t);
  for(; ptr != end; ptr++) *ptr = 0; 
  if(isatty(stdin->fd)) stdin->buf_type = _IOLBF;
  ptr = (char *) (&(stdout->mb_state));
  end = ptr + sizeof(mbstate_t);
  for(; ptr != end; ptr++) *ptr = 0; 
  if(isatty(stdout->fd)) stdout->buf_type = _IOLBF;
  ptr = (char *) (&(stderr->mb_state));
  end = ptr + sizeof(mbstate_t);
  for(; ptr != end; ptr++) *ptr = 0; 
  __uportlibc_add_stream(stdin);
  __uportlibc_add_stream(stdout);
  __uportlibc_add_stream(stderr);
}

void __uportlibc_add_stream(FILE *stream)
{
  lock_lock(&stdio_lock);
  if(stdio_first_stream == NULL) stdio_first_stream = stream;
  if(stdio_last_stream != NULL) stream->prev = stdio_last_stream;
  stdio_last_stream = stream;
  lock_unlock(&stdio_lock);
}

void __uportlibc_delete_stream(FILE *stream)
{
  lock_lock(&stdio_lock);
  if(stream->prev != NULL)
    stream->prev->next = stream->next;
  else
    stdio_first_stream = stream->next;
  if(stream->next != NULL)
    stream->next->prev = stream->prev;
  else
    stdio_last_stream = stream->prev;
  stream->prev = NULL;
  stream->next = NULL;
  lock_unlock(&stdio_lock);
}

int __uportlibc_for_each_stream(int (*fun)(FILE *))
{
  FILE *stream;
  int res = 0;
  lock_lock(&stdio_lock);
  for(stream = stdio_first_stream; stream != NULL; stream = stream->next) {
    if(fun(stream) == EOF) res = EOF;
  }
  lock_unlock(&stdio_lock);
  return res;
}
