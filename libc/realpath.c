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
#include <errno.h>
#include <libgen.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

static char *concat_path_names(char *path_name1, const char *path_name2)
{
  size_t len;
  if(path_name2[0] == '/') {
    len = strlen(path_name1);
    if(len >= 1 && path_name1[len - 1] == '/') {
      if(len + 1 > PATH_MAX - 1) {
        errno = ENAMETOOLONG;
        return NULL;
      }
      path_name1[len] = '/';
      path_name1[len + 1] = 0;
      len++;
    }
  } else {
    path_name1[0] = 0;
    len = 0;
  }
  if(len + strlen(path_name2) > PATH_MAX - 1) {
    errno = ENAMETOOLONG;
    return NULL;
  }
  strcat(path_name1, path_name2);
  return path_name1;
}

static int follow_links(const char *path_name, char *buf)
{
  struct stat stat_buf;
  char *readlink_path_name;
  int res, saved_errno;
  if(stat(path_name, &stat_buf) == -1) {
    return errno == ENOENT ? 0 : -1;
  }
  saved_errno = errno;
  if(strlen(path_name) > PATH_MAX - 1) {
    errno = ENAMETOOLONG;
    return -1;
  }
  readlink_path_name = malloc(PATH_MAX);
  if(readlink_path_name == NULL) {
    errno = ENOMEM;
    return -1;
  }
  strcpy(buf, path_name);
  res = 0;
  while(1) {
    if(readlink(buf, readlink_path_name, PATH_MAX) == -1) {
      if(errno == EINVAL)
        errno = saved_errno;
      else
        res = -1;
      break;
    }
    if(readlink_path_name[0] != '/') dirname(buf);
    if(concat_path_names(buf, readlink_path_name) == NULL) {
      res = -1;
      break;
    }
    res = 1;
  }
  free(readlink_path_name);
  return res;
}

char *realpath(const char *path_name, char *resolved_path_name)
{
  char *tmp_path_name, *link_path_name, *res;
  size_t tmp_path_name_len;
  size_t i, j, k;
  if(strlen(path_name) > PATH_MAX - 1) {
    errno = ENAMETOOLONG;
    return NULL;
  }
  tmp_path_name = malloc(PATH_MAX);
  if(tmp_path_name == NULL) {
    errno = ENOMEM;
    return NULL;
  }
  link_path_name = malloc(PATH_MAX);
  if(link_path_name == NULL) {
    errno = ENOMEM;
    free(tmp_path_name);
    return NULL;
  }
  if(path_name[0] != '/') {
    if(getcwd(tmp_path_name, PATH_MAX) == NULL) {
      free(link_path_name);
      free(tmp_path_name);
      return NULL;
    }
  }
  concat_path_names(tmp_path_name, path_name);
  tmp_path_name_len = strlen(tmp_path_name);
  i = j = 0;
  while(1) {
    for(; tmp_path_name[i] == '/'; i++);
    if(tmp_path_name[i] == 0) break;
    resolved_path_name[j] = '/';
    j++;
    k = j;
    for(; tmp_path_name[i] != 0 && tmp_path_name[i] != '/'; i++, j++) {
      resolved_path_name[j] = tmp_path_name[i];
    }
    resolved_path_name[j] = 0;
    if(strcmp(resolved_path_name + k, ".") == 0) {
      j = k - 1;
      resolved_path_name[j] = 0;
    } else if(strcmp(resolved_path_name + k, "..") == 0) {
      j = k - 1;
      while(j != 0) {
        j--;
        if(resolved_path_name[j] == '/') break;
      }
      resolved_path_name[j] = 0;
    } else {
      size_t link_path_name_len;
      int tmp_res = follow_links(resolved_path_name, link_path_name);
      if(tmp_res == 1) {
        link_path_name_len = strlen(link_path_name);
        if(strlen(link_path_name) + tmp_path_name_len - i > PATH_MAX - 1) {
          errno = ENAMETOOLONG;
          res = NULL;
          break;
        }
        if(i != link_path_name_len)
          memmove(tmp_path_name + link_path_name_len, tmp_path_name + i, tmp_path_name_len - i + 1);
        memcpy(tmp_path_name, link_path_name, link_path_name_len);
        tmp_path_name_len = strlen(tmp_path_name);
        i = j = 0;
      } else if(tmp_res == -1) {
        res = NULL;
        break;
      }
    }
  }
  if(j == 0) {
    resolved_path_name[0] = '/';
    resolved_path_name[1] = 0;
  }
  free(link_path_name);
  free(tmp_path_name);
  return res;
}
