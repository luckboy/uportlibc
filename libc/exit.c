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
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#else
#define SYS_MOCK_UNISTD
#include "sys_mock.h"
#define UPORTLIBC_EXIT
#define UPORTLIBC_STDIO
#include "uportlibc.h"
#endif
#include "exit.h"
#include "stdio_priv.h"

static int close_stream(FILE *stream)
{
  int res;
  lock_lock(&(stream->lock));
  do {
    if(__uportlibc_unsafely_flush_stream(stream) == EOF) {
      res = EOF;
      break;
    }
    if(close(stream->fd) == -1) {
      res = EOF;
      break;
    }
  } while(0);
  lock_unlock(&(stream->lock));
  return res;
}

static void close_all_streams(void)
{ __uportlibc_for_each_stream(close_stream); }

lock_t __uportlibc_exit_lock = LOCK;
void (*__uportlibc_atexit_exit_fun)(void) = NULL;
void (*__uportlibc_environ_exit_fun)(void) = NULL;
void (*__uportlibc_stdio_exit_fun)(void) = close_all_streams;

void exit(int status)
{
  void (*atexit_exit_fun)(void);
  void (*environ_exit_fun)(void);
  void (*stdio_exit_fun)(void);
  lock_lock(&__uportlibc_exit_lock);
  atexit_exit_fun = __uportlibc_atexit_exit_fun;
  environ_exit_fun = __uportlibc_environ_exit_fun;
  stdio_exit_fun = __uportlibc_stdio_exit_fun;
  lock_unlock(&__uportlibc_exit_lock);
  if(atexit_exit_fun != NULL) atexit_exit_fun();
  if(environ_exit_fun != NULL) environ_exit_fun();
  stdio_exit_fun();
  _exit(status);
  while(1);
}
