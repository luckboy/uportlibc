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
#include "exit.h"

struct atexit_list_elem
{
  struct atexit_list_elem *next;
  void (*fun)(void);
};

static lock_t atexit_lock = LOCK;
static struct atexit_list_elem *atexit_list;

static void invoke_atexit_funs(void)
{
  lock_lock(&atexit_lock);
  while(atexit_list != NULL) {
    struct atexit_list_elem *tmp_elem = atexit_list;
    atexit_list = atexit_list->next;
    lock_unlock(&atexit_lock);
    tmp_elem->fun();
    lock_lock(&atexit_lock);
    free(tmp_elem);
  }
  lock_unlock(&atexit_lock);
}

int atexit(void (*fun)(void))
{
  int saved_errno = errno;
  struct atexit_list_elem *elem = (struct atexit_list_elem *) malloc(sizeof(struct atexit_list_elem));
  if(elem == NULL) {
    errno = saved_errno;
    return -1;
  }
  lock_lock(&atexit_lock);
  elem->next = atexit_list;
  elem->fun = fun;
  atexit_list = elem;
  lock_unlock(&atexit_lock);
  lock_lock(&__uportlibc_exit_lock);
  __uportlibc_atexit_exit_fun = invoke_atexit_funs;
  lock_unlock(&__uportlibc_exit_lock);
  return 0;
}
