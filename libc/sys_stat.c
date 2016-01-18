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
#include <sys/stat.h>
#include <uportsys/sys.h>
#include <errno.h>

int chmod(const char *path_name, mode_t mode)
{ return __uportsys_chmod(path_name, mode, &errno); }

int fchmod(int fd, mode_t mode)
{ return __uportsys_fchmod(fd, mode, &errno); }

int fstat(int fd, struct stat *buf)
{ return __uportsys_fstat(fd, (struct __uportsys_stat *) buf, &errno); }

int lstat(const char *path_name, struct stat *buf)
{ return __uportsys_lstat(path_name, (struct __uportsys_stat *) buf, &errno); }

int mkdir(const char *dir_name, mode_t mode)
{ return __uportsys_mkdir(dir_name, mode, &errno); }

int mknod(const char *file_name, mode_t mode, dev_t dev)
{ return __uportsys_mknod(file_name, mode, dev, &errno); }

int stat(const char *path_name, struct stat *buf)
{ return __uportsys_stat(path_name, (struct __uportsys_stat *) buf, &errno); }

mode_t umask(mode_t mask)
{ return __uportsys_umask(mask, &errno); }
