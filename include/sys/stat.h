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
#ifndef _SYS_STAT_H
#define _SYS_STAT_H

#include <uportsys/sys.h>

#define S_IFMT                  __UPORTSYS_S_IFMT
#define S_IFIFO                 __UPORTSYS_S_IFIFO
#define S_IFCHR                 __UPORTSYS_S_IFCHR
#define S_IFDIR                 __UPORTSYS_S_IFDIR
#define S_IFBLK                 __UPORTSYS_S_IFBLK
#define S_IFREG                 __UPORTSYS_S_IFREG
#define S_IFLNK                 __UPORTSYS_S_IFLNK
#ifdef __UPORTSYS_S_IFSOCK
#define S_IFSOCK                __UPORTSYS_S_IFSOCK
#endif
#ifdef __UPORTSYS_S_IFWHT
#define S_IFWHT                 __UPORTSYS_S_IFWHT
#endif

#define S_ISUID                 __UPORTSYS_S_ISUID
#define S_ISGID                 __UPORTSYS_S_ISGID
#define S_ISVTX                 __UPORTSYS_S_ISVTX
#define S_IRUSR                 __UPORTSYS_S_IRUSR
#define S_IWUSR                 __UPORTSYS_S_IWUSR
#define S_IXUSR                 __UPORTSYS_S_IXUSR
#define S_IRGRP                 __UPORTSYS_S_IRGRP
#define S_IWGRP                 __UPORTSYS_S_IWGRP
#define S_IXGRP                 __UPORTSYS_S_IXGRP
#define S_IROTH                 __UPORTSYS_S_IROTH
#define S_IWOTH                 __UPORTSYS_S_IWOTH
#define S_IXOTH                 __UPORTSYS_S_IXOTH

#define S_IRWXU                 (S_IRUSR | S_IWUSR | S_IXUSR)
#define S_IRWXG                 (S_IRGRP | S_IWGRP | S_IXGRP)
#define S_IRWXO                 (S_IROTH | S_IWOTH | S_IXOTH)

#define S_ISFIFO(mode)          (((mode) & S_IFMT) == S_IFIFO)
#define S_ISCHR(mode)           (((mode) & S_IFMT) == S_IFCHR)
#define S_ISDIR(mode)           (((mode) & S_IFMT) == S_IFDIR)
#define S_ISBLK(mode)           (((mode) & S_IFMT) == S_IFBLK)
#define S_ISREG(mode)           (((mode) & S_IFMT) == S_IFREG)
#define S_ISLNK(mode)           (((mode) & S_IFMT) == S_IFLNK)
#ifdef S_IFSOCK
#define S_ISSOCK(mode)          (((mode) & S_IFMT) == S_IFSOCK)
#endif
#ifdef S_IFWHT
#define S_ISWHT(mode)           (((mode) & S_IFMT) == S_IFWHT)
#endif

#ifdef __cplusplus
extern "C" {
#endif

#ifndef _BLKCNT_T
#define _BLKCNT_T
typedef __uportsys_blkcnt_t blkcnt_t;
#endif

#ifndef _BLKSIZE_T
#define _BLKSIZE_T
typedef __uportsys_blksize_t blksize_t;
#endif

#ifndef _DEV_T
#define _DEV_T
typedef __uportsys_dev_t dev_t;
#endif

#ifndef _GID_T
#define _GID_T
typedef __uportsys_gid_t gid_t;
#endif

#ifndef _INO_T
#define _INO_T
typedef __uportsys_ino_t ino_t;
#endif

#ifndef _MODE_T
#define _MODE_T
typedef __uportsys_mode_t mode_t;
#endif

#ifndef _NLINK_T
#define _NLINK_T
typedef __uportsys_nlink_t nlink_t;
#endif

#ifndef _OFF_T
#define _OFF_T
typedef __uportsys_off_t off_t;
#endif

#ifndef _TIME_T
#define _TIME_T
typedef __uportsys_time_t time_t;
#endif

#ifndef _UID_T
#define _UID_T
typedef __uportsys_uid_t uid_t;
#endif

#ifdef ___UPORTSYS_STRUCT_STAT_XTIM
#ifndef _STRUCT_TIMESPEC
#define _STRUCT_TIMESPEC
__UPORTSYS_STRUCT_TIMESPEC(timespec);
#endif
#endif

__UPORTSYS_STRUCT_STAT(stat, timespec);

#ifdef ___UPORTSYS_STRUCT_STAT_XTIM
#define st_atime                st_atim.tv_sec
#define st_mtime                st_mtim.tv_sec
#define st_ctime                st_ctim.tv_sec
#endif

int chmod(const char *path_name, mode_t mode);
int fchmod(int fd, mode_t mode);
int fstat(int fd, struct stat *buf);
int lstat(const char *path_name, struct stat *buf);
int mkdir(const char *dir_name, mode_t mode);
int mknod(const char *file_name, mode_t mode, dev_t dev);
int stat(const char *path_name, struct stat *buf);
mode_t umask(mode_t mask);

#ifdef __cplusplus
}
#endif

#endif
