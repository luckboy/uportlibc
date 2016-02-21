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
#include <sys/uio.h>
#include <uportsys/sys.h>
#include <errno.h>
#ifndef ___UPORTSYS_XXXV_VECTOR_IO
#include <limits.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#endif

#ifdef ___UPORTSYS_XXXV_VECTOR_IO

ssize_t readv(int fd, const struct iovec *iov, int iov_count)
{ return __uportsys_readv(fd, (const struct __uportsys_iovec *) iov, iov_count, &errno); }

ssize_t writev(int fd, const struct iovec *iov, int iov_count)
{ return __uportsys_writev(fd, (const struct __uportsys_iovec *) iov, iov_count, &errno); }

#else

ssize_t readv(int fd, const struct iovec *iov, int iov_count)
{
  void *buf;
  size_t buf_size;
  ssize_t res;
  int i;
  if(iov_count < 0 || iov_count > IOV_MAX) {
    errno = EINVAL;
    return -1;
  }
  buf_size = 0;
  for(i = 0; i < iov_count; i++) {
    if(iov[i].iov_len > SSIZE_MAX) {
      errno = EINVAL;
      return -1;
    }
    buf_size += iov[i].iov_len;
    if(buf_size > SSIZE_MAX) {
      errno = EINVAL;
      return -1;
    }
  }
  buf = malloc(buf_size != 0 ? buf_size : 1);
  if(buf == NULL) {
    errno = ENOMEM;
    return -1;
  }
  res = read(fd, buf, buf_size);
  if(res != -1) {
    size_t offset = 0, count = (size_t) res;
    for(i = 0; i < iov_count && offset < ((size_t) res); i++) {
      const void *src = (const void *) (((const char *) buf) + offset);
      memcpy(iov[i].iov_base, src, (count < iov[i].iov_len ? count : iov[i].iov_len));
      offset += iov[i].iov_len;
      count -= iov[i].iov_len;
    }
  }
  free(buf);
  return res;
}

ssize_t writev(int fd, const struct iovec *iov, int iov_count)
{
  void *buf;
  size_t buf_size;
  ssize_t res;
  int i;
  size_t offset;
  if(iov_count < 0 || iov_count > IOV_MAX) {
    errno = EINVAL;
    return -1;
  }
  buf_size = 0;
  for(i = 0; i < iov_count; i++) {
    if(iov[i].iov_len > SSIZE_MAX) {
      errno = EINVAL;
      return -1;
    }
    buf_size += iov[i].iov_len;
    if(buf_size > SSIZE_MAX) {
      errno = EINVAL;
      return -1;
    }
  }
  buf = malloc(buf_size != 0 ? buf_size : 1);
  if(buf == NULL) {
    errno = ENOMEM;
    return -1;
  }
  offset = 0;
  for(i = 0; i < iov_count; i++) {
    void *dst = (void *) (((char *) buf) + offset);
    memcpy(dst, iov[i].iov_base, iov[i].iov_len);
    offset += iov[i].iov_len;
  }
  res = write(fd, buf, buf_size);
  free(buf);
  return res;
}

#endif
