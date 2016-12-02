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
#include <string.h>
#include "uportlibc.h"
#include "exit.h"
#include "stdio_priv.h"

extern lock_t stdio_lock;
extern uportlibc_FILE *stdio_first_stream;
extern uportlibc_FILE *stdio_last_stream;

extern char static_stdin_buf[UPORTLIBC_BUFSIZ];
extern char static_stdout_buf[UPORTLIBC_BUFSIZ];
extern char static_stderr_buf[UPORTLIBC_BUFSIZ];

extern uportlibc_FILE static_stdin;
extern uportlibc_FILE static_stdout;
extern uportlibc_FILE static_stderr;

static lock_t saved_stdio_lock;
static uportlibc_FILE *saved_stdio_first_stream;
static uportlibc_FILE *saved_stdio_last_stream;

static char saved_static_stdin_buf[UPORTLIBC_BUFSIZ];
static char saved_static_stdout_buf[UPORTLIBC_BUFSIZ];
static char saved_static_stderr_buf[UPORTLIBC_BUFSIZ];

static uportlibc_FILE saved_static_stdin;
static uportlibc_FILE saved_static_stdout;
static uportlibc_FILE saved_static_stderr;

static uportlibc_FILE *saved_stdin;
static uportlibc_FILE *saved_stdout;
static uportlibc_FILE *saved_stderr;

static lock_t saved_exit_lock;
static void (*saved_stdio_exit_fun)(void);

void save_stdio_vars(void)
{
  saved_stdio_lock = stdio_lock;
  saved_stdio_first_stream = stdio_first_stream;
  saved_stdio_last_stream = stdio_last_stream;
  memcpy(saved_static_stdin_buf, static_stdin_buf, UPORTLIBC_BUFSIZ);
  memcpy(saved_static_stdout_buf, static_stdout_buf, UPORTLIBC_BUFSIZ);
  memcpy(saved_static_stderr_buf, static_stderr_buf, UPORTLIBC_BUFSIZ);
  saved_static_stdin = static_stdin;
  saved_static_stdout = static_stdout;
  saved_static_stderr = static_stderr;
  saved_stdin = uportlibc_stdin;
  saved_stdout = uportlibc_stdout;
  saved_stderr = uportlibc_stderr;
  saved_exit_lock = uportlibc_exit_lock;
  saved_stdio_exit_fun = uportlibc_stdio_exit_fun;
}

void restore_stdio_vars(void)
{
  stdio_lock = saved_stdio_lock;
  stdio_first_stream = saved_stdio_first_stream;
  stdio_last_stream = saved_stdio_last_stream;
  memcpy(static_stdin_buf, saved_static_stdin_buf, UPORTLIBC_BUFSIZ);
  memcpy(static_stdout_buf, saved_static_stdout_buf, UPORTLIBC_BUFSIZ);
  memcpy(static_stderr_buf, saved_static_stderr_buf, UPORTLIBC_BUFSIZ);
  static_stdin = saved_static_stdin;
  static_stdout = saved_static_stdout;
  static_stderr = saved_static_stderr;
  uportlibc_stdin = saved_stdin;
  uportlibc_stdout = saved_stdout;
  uportlibc_stderr = saved_stderr;
  uportlibc_exit_lock = saved_exit_lock;
  uportlibc_stdio_exit_fun = saved_stdio_exit_fun;
}
