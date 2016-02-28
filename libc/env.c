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
#include <stdlib.h>
#include <string.h>
#include "exit.h"

extern char **environ;

static void free_environ(void)
{ free(environ); }

char *getenv(const char *name)
{
  size_t i, len = strlen(name);
  for(i = 0; environ[i] != NULL; i++) {
    if(strncmp(environ[i], name, len) == 0 && environ[i][len] == '=')
      return &(environ[i][len + 1]);
  }
  return NULL;
}

int putenv(char *str)
{
  static int is_orig_environ = 1;
  char **new_environ;
  size_t i, count, len;
  char *ptr = strchr(str, '=');
  int is_addition;
  if(ptr != NULL) {
    len = ptr - str - 1;
    is_addition = 1;
  } else {
    len = strlen(str);
    is_addition = 0;
  }
  for(i = 0; environ[i] != NULL; i++) {
    if(strncmp(environ[i], str, len) == 0 && environ[i][len] == '=') {
      if(is_addition) {
        environ[i] = str;
        return 0;
      }
      break;
    }
  }
  if(!is_addition && environ[i] == NULL) return 0;
  for(count = 0; environ[count] != NULL; count++);
  if(is_orig_environ) {
    new_environ = (char **) malloc(count + 1);
    if(new_environ == NULL) {
      errno = ENOMEM;
      return -1;
    }
    environ = memcpy(new_environ, environ, (count + 1) * sizeof(char *));
    is_orig_environ = 0;
    lock_lock(&__uportlibc_exit_lock);
    __uportlibc_environ_exit_fun = free_environ;
    lock_unlock(&__uportlibc_exit_lock);
  }
  if(is_addition) {
    new_environ = (char **) realloc(environ, (count + 2) * sizeof(char *));
    if(new_environ == NULL) {
      errno = ENOMEM;
      return -1;
    }
    environ = new_environ;
    environ[count] = str;
  } else {
    memmove(environ + i, environ + i + 1, (count - i + 1) * sizeof(char *));
    new_environ = (char **) realloc(environ, count);
    if(new_environ == NULL) {
      errno = ENOMEM;
      return -1;
    }
  }
  return 0;
}
