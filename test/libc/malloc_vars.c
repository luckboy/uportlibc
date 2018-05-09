/*
 * Copyright (c) 2018 Łukasz Szpakowski
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
#include "lock.h"
#include "malloc_priv.h"

extern lock_t malloc_lock;
extern struct malloc_header *heap_root;
extern struct malloc_header *last_header;

static lock_t saved_malloc_lock;
static struct malloc_header *saved_heap_root;
static struct malloc_header *saved_last_header;

void save_malloc_vars(void)
{
  saved_malloc_lock = malloc_lock;
  saved_heap_root = heap_root;
  saved_last_header = last_header;
}

void restore_malloc_vars(void)
{
  malloc_lock = saved_malloc_lock;
  heap_root = saved_heap_root;
  last_header = saved_last_header;
}
