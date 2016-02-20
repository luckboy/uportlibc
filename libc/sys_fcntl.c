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
#include <uportsys/sys.h>
#include <errno.h>
#include <fcntl.h>
#include <inttypes.h>
#include <stdarg.h>

int creat(const char *file_name, mode_t mode)
{ return open(file_name, O_WRONLY | O_CREAT | O_TRUNC, mode); }

int fcntl(int fd, int cmd, ...)
{
  va_list ap;
  intptr_t arg;
  va_start(ap, cmd);
  arg = va_arg(ap, intptr_t);
  va_end(ap);
  return __uportsys_fcntl(fd, cmd, arg, &errno);
}

int open(const char *file_name, int flags, ...)
{
  va_list ap;
  mode_t mode;
  va_start(ap, flags);
  mode = va_arg(ap, unsigned);
  va_end(ap);
  return __uportsys_open(file_name, flags, mode, &errno);
}
