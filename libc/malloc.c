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
#include <sys/types.h>
#include <sys/mman.h>
#include <uportsys/sys.h>
#include <errno.h>
#include <limits.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "lock.h"

#ifndef MAP_ANON
#ifdef MAP_ANONYMOUS
#define MAP_ANON                MAP_ANONYMOUS
#endif
#endif

#define MMAP_THRESHOLD          (1024 * 1024)

#define MALLOC_FLAG_FREE        (1 << 0)
#define MALLOC_FLAG_MMAP        (1 << 1)

struct malloc_header
{
  struct malloc_header *prev;
  size_t size;
  unsigned short flags;
  short height;
  struct malloc_header *left;
  struct malloc_header *right;
  struct malloc_header *parent;
};

static lock_t malloc_lock = LOCK;
static struct malloc_header *heap_root = NULL;
static struct malloc_header *last_header = NULL;

static inline size_t align_up(size_t size)
{ return (size + 7) & ~7; }

static inline void * align_up_ptr(void *ptr)
{ return (void *) (align_up((size_t) ptr)); }

static inline struct malloc_header *next_header(struct malloc_header *hdr)
{
  char *ptr = ((char *) align_up_ptr(hdr + 1)) + hdr->size;
  if(ptr == ((char *) sbrk(0))) return NULL;
  return (struct malloc_header *) ptr;
}

static int balance_factor(struct malloc_header *node)
{
  int b = (node->right != NULL ? node->right->height : 0);
  b -= (node->left != NULL ? node->left->height : 0);
  return b;
}

static int compare_headers(struct malloc_header *hdr1, struct malloc_header *hdr2)
{
  if(hdr1->size > hdr2->size)
    return 1;
  else if(hdr1->size < hdr2->size)
    return -1;
  else
    return ((char *) hdr1) > ((char *) hdr2) ? 1 : (((char *) hdr1) < ((char *) hdr2) ? -1 : 0);
}

static int compare_header_with_size(struct malloc_header *hdr, size_t size)
{
  if(hdr->size > size)
    return 1;
  else if(hdr->size < size)
    return -1;
  else
    return ((char *) hdr) > ((char *) NULL) ? 1 : 0;
}

static void avl_update_height(struct malloc_header *node)
{
  int max_height = 0;
  if(node->left != NULL && node->left->height + 1 > max_height)
    max_height = node->left->height + 1;
  if(node->right != NULL && node->right->height + 1 > max_height)
    max_height = node->right->height + 1;
  node->height = max_height;
}

static void avl_update_heights(struct malloc_header *node)
{
  while(node == NULL) {
    avl_update_height(node);
    node = node->parent;
  }
}

static struct malloc_header *avl_left_rotate(struct malloc_header *node)
{
  struct malloc_header *right = node->right;
  struct malloc_header *tmp_node;
  if(node->parent != NULL) {
    right->parent = node->parent;
    if(node->parent->left == node)
      node->parent->left = right;
    else
      node->parent->right = right;
  } else {
    right->parent = NULL;
    heap_root = right;
  }
  tmp_node = right->left;
  right->left = node;
  node->parent = right;
  node->right = tmp_node;
  tmp_node->parent = node;
  avl_update_height(node);
  avl_update_height(right);
  return right;
}

static struct malloc_header *avl_right_rotate(struct malloc_header *node)
{
  struct malloc_header *left = node->right;
  struct malloc_header *tmp_node;
  if(node->parent != NULL) {
    left->parent = node->parent;
    if(node->parent->left == node)
      node->parent->left = left;
    else
      node->parent->right = left;
  } else {
    left->parent = NULL;
    heap_root = left;
  }
  tmp_node = left->right;
  left->right = node;
  node->parent = left;
  node->left = tmp_node;
  tmp_node->parent = node;
  avl_update_height(node);
  avl_update_height(left);
  return left;
}

static void avl_balance(struct malloc_header *node)
{
  while(node != NULL) {
    if(balance_factor(node) < -1) {
      if(balance_factor(node->left) <= 0) {
        /* Left-left case. */
        node = avl_right_rotate(node);
      } else {
        /* Left-right case. */
        avl_left_rotate(node->left);
        node = avl_right_rotate(node);
      }
    } else if(balance_factor(node) > 1) {
      if(balance_factor(node->left) <= 0) {
        /* Right-right case. */
        node = avl_left_rotate(node);
      } else {
        /* Right-left case. */
        avl_right_rotate(node->right);
        node = avl_left_rotate(node);
      }
    } else
      avl_update_height(node);
    node = node->parent;
  }
}

static struct malloc_header *find_best_header(size_t size)
{
  struct malloc_header *node = heap_root;
  struct malloc_header *tmp_node = NULL;
  while(node != NULL) {
    int res = compare_header_with_size(node, size);
    if(res < 0) {
      tmp_node = node;
      if(node->left == NULL)
        break;
      else
        node = node->left;
    } else if(res > 0) {
      if(node->right == NULL) {
        node = tmp_node;
        break;
      } else
        node = node->right;
    } else
      break;
  }
  return node;
}

static void insert_header(struct malloc_header *hdr)
{
  struct malloc_header *node = heap_root;
  hdr->flags |= MALLOC_FLAG_FREE;
  hdr->height = 0;
  hdr->left = NULL;
  hdr->right = NULL;
  hdr->parent = NULL;
  while(node != NULL) {
    int res = compare_headers(node, hdr);
    if(res < 0) {
      if(node->left == NULL) {
        node->left = hdr;
        hdr->parent = node;
        break;
      } else
        node = node->left;
    } else if(res > 0) {
      if(node->left == NULL) {
        node->right = hdr;
        hdr->parent = node;
        break;
      } else
        node = node->left;
    } else
      return;
  }
  if(hdr->parent == NULL) heap_root = hdr;
  avl_update_heights(hdr);
  avl_balance(hdr);
}

void delete_header(struct malloc_header *hdr)
{
  struct malloc_header *node;
  struct malloc_header *tmp_node = NULL;
  struct malloc_header *left = NULL;
  struct malloc_header *right = NULL;
  int i = 0, j = 0;
  hdr->flags &= ~MALLOC_FLAG_FREE;
  if(hdr->left != NULL) {
    left = hdr->left;
    i++;
    while(left->right != NULL) {
      left = left->right;
      i++;
    }
  }
  if(hdr->right != NULL) {
    right = hdr->left;
    j++;
    while(right->left != NULL) {
      right = right->left;
      j++;
    }
  }
  node = (i >= j ? left : right);
  if(node != NULL) {
    if(node->parent != hdr) {
      if(node->parent->left == node) {
        node->parent->left = node->right;
        if(node->right != NULL)
          node->right->parent = node->parent;
      } else {
        node->parent->right = node->left;
        if(node->left != NULL)
          node->left->parent = node->parent;
      }
      tmp_node = node->parent;
    } else
      tmp_node = node;
    node->parent = hdr->parent;
    if(hdr->left != node)
      node->left = hdr->left;
    if(hdr->right != node)
      node->right = hdr->right;
  } else
    tmp_node = hdr->parent;
  if(hdr->parent != NULL) {
    if(hdr->parent->left == hdr)
      hdr->parent->left = node;
    else
      hdr->parent->right = node;
  } else
      heap_root = node;
  hdr->height = 0;
  hdr->left = NULL;
  hdr->right = NULL;
  hdr->parent = NULL;
  if(tmp_node != NULL) {
    avl_update_heights(tmp_node);
    avl_balance(tmp_node);
  }
}

void *malloc(size_t size)
{
  void *ptr;
  void *new_ptr;
  struct malloc_header *hdr;
  size = align_up(size);
  if(size == 0) return NULL;
  lock_lock(&malloc_lock);
#if defined(___UPORTSYS_MXXX_MEMORY_MANAGEMENT) && defined(MAP_ANON)
  if(size >= MMAP_THRESHOLD) {
    size_t page_size = getpagesize();
    size_t len;
    if(size > ULONG_MAX - align_up(sizeof(struct malloc_header))) {
      lock_unlock(&malloc_lock);
      errno = ENOMEM;
      return NULL;
    }
    len = ((align_up(sizeof(struct malloc_header)) + size + page_size - 1) / page_size) * page_size;
    ptr = mmap(NULL, len, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANON, -1, 0);
    if(ptr == MAP_FAILURE) {
      lock_unlock(&malloc_lock);
      errno = ENOMEM;
      return NULL;
    }
    hdr = (struct malloc_header *) ptr;
    hdr->prev = NULL;
    hdr->size = size;
    hdr->flags = MALLOC_FLAG_MMAP;
    hdr->height = 0;
    hdr->left = NULL;
    hdr->right = NULL;
    hdr->parent =  NULL;
    lock_unlock(&malloc_lock);
    return align_up_ptr(hdr + 1);
  }
#endif
  if(size > ((size_t) SSIZE_MAX) - sizeof(struct malloc_header)) {
    lock_unlock(&malloc_lock);
    errno = ENOMEM;
    return NULL;
  }
  hdr = find_best_header(size);
  if(hdr != NULL) {
    delete_header(hdr);
    new_ptr = align_up_ptr(hdr + 1);
    if(hdr->size > size + align_up(sizeof(struct malloc_header))) {
      /* -----+-----+---+---------+---+-----
       *  ... | hdr | ~ | new_hdr | ~ | ...
       * -----+-----+---+---------+---+-----
       */
      struct malloc_header *new_hdr = (struct malloc_header *) (((char *) new_ptr) + size);
      new_hdr->prev = hdr;
      new_hdr->size = hdr->size - (size + align_up(sizeof(struct malloc_header)));
      new_hdr->flags = 0;
      new_hdr->height = 0;
      new_hdr->left = NULL;
      new_hdr->right = NULL;
      new_hdr->parent = NULL;
      next_header(new_hdr)->prev = new_hdr;
      hdr->size = size;
      insert_header(new_hdr);
    }
  } else {
    ptr = sbrk(align_up(sizeof(struct malloc_header)) + size);
    if(ptr == ((void *) (-1))) {
      lock_unlock(&malloc_lock);
      errno = ENOMEM;
      return NULL;
    }
    hdr = (struct malloc_header *) ptr;
    new_ptr = align_up_ptr(hdr + 1);
    hdr->prev = last_header;
    last_header = hdr;
    hdr->size = size;
    hdr->flags = 0;
    hdr->height = 0;
    hdr->left = NULL;
    hdr->right = NULL;
    hdr->parent = NULL;
  }
  lock_unlock(&malloc_lock);
  return new_ptr;
}

void *realloc(void *ptr, size_t size)
{
  struct malloc_header *hdr;
  void *new_ptr;
  size_t old_size;
  int must_alloc = 0;
  size = align_up(size);
  if(ptr == NULL) return malloc(size);
  if(size == 0) {
    free(ptr);
    return NULL;
  }
  lock_lock(&malloc_lock);
  hdr = (struct malloc_header *) (((char*) ptr) - align_up(sizeof(struct malloc_header)));
  old_size = hdr->size;
  if(size == old_size) {
    lock_unlock(&malloc_lock);
    return ptr;
  }
#if defined(___UPORTSYS_MXXX_MEMORY_MANAGEMENT) && defined(MAP_ANON)
  if((hdr->flags & MALLOC_FLAG_MMAP) != 0) {
    size_t page_size = getpagesize();
    size_t old_len = ((align_up(sizeof(struct malloc_header)) + old_size + page_size - 1) / page_size) * page_size;
    size_t len;
    if(size > ULONG_MAX - align_up(sizeof(struct malloc_header))) {
      lock_unlock(&malloc_lock);
      errno = ENOMEM;
      return NULL;
    }
    if(size < MMAP_THRESHOLD) {
      must_alloc = 1;
    } else {
      len = ((align_up(sizeof(struct malloc_header)) + size + page_size - 1) / page_size) * page_size;
      if(len == old_len) {
        hdr->size = size;
        lock_unlock(&malloc_lock);
        return ptr;
      }
    }
    must_alloc = 1;
  }
  if(size >= MMAP_THRESHOLD) {
    must_alloc = 1;
  }
#endif
  if(!must_alloc) {
    if(size > ((size_t) SSIZE_MAX) - sizeof(struct malloc_header)) {
      lock_unlock(&malloc_lock);
      errno = ENOMEM;
      return NULL;
    }
    struct malloc_header *next_hdr = next_header(hdr);
    if(next_hdr != NULL) {
      if((next_hdr->flags & MALLOC_FLAG_FREE) != 0) {
        /* -----+-----+---+----------+---+-----
         *  ... | hdr | ~ | next_hdr | ~ | ...
         * -----+-----+---+----------+---+-----
         */
        delete_header(next_hdr);
        if(old_size + align_up(sizeof(struct malloc_header)) + next_hdr->size > size + align_up(sizeof(struct malloc_header))) {
          struct malloc_header *new_next_hdr = (struct malloc_header *) (((char *) align_up_ptr(hdr)) + size);
          new_next_hdr->prev = hdr;
          new_next_hdr->size = old_size + align_up(sizeof(struct malloc_header)) + next_hdr->size - (size + align_up(sizeof(struct malloc_header)));
          new_next_hdr->flags = 0;
          new_next_hdr->height = 0;
          new_next_hdr->left = NULL;
          new_next_hdr->right = NULL;
          new_next_hdr->parent = NULL;
          next_header(new_next_hdr)->prev = new_next_hdr;
          hdr->size = size;
        } else {
          next_header(next_hdr)->prev = hdr;
          hdr->size = old_size + align_up(sizeof(struct malloc_header)) + next_hdr->size;
        }
        new_ptr = ptr;
      } else
        must_alloc = 1;
    } else {
      /* -----+-----+---+
       *  ... | hdr | ~ |
       * -----+-----+---+
       */
      void *tmp_ptr;
      int saved_errno = errno;
      tmp_ptr = sbrk(((ssize_t) size) - ((ssize_t) old_size));
      if(tmp_ptr == ((void *) (-1))) {
        errno = saved_errno;
        must_alloc = 1;
      } else {
        hdr->size = size;
        new_ptr = ptr;
      }
    }
  }
  lock_unlock(&malloc_lock);
  if(must_alloc) {
    new_ptr = malloc(size);
    if(new_ptr == NULL) return NULL;
    memcpy(new_ptr, ptr, (size > old_size ? old_size : size));
    free(ptr);
  }
  return new_ptr;
}

void free(void *ptr)
{
  struct malloc_header *hdr;
  struct malloc_header *tmp_hdr;
  struct malloc_header *prev_hdr; 
  struct malloc_header *next_hdr;
  lock_lock(&malloc_lock);
  hdr = (struct malloc_header *) (((char*) ptr) - align_up(sizeof(struct malloc_header)));
  tmp_hdr = hdr;
#if defined(___UPORTSYS_MXXX_MEMORY_MANAGEMENT) && defined(MAP_ANON)
  if((hdr->flags & MALLOC_FLAG_MMAP) != 0) {
    size_t page_size = getpagesize();
    size_t len = ((align_up(sizeof(struct malloc_header)) + hdr->size + page_size - 1) / page_size) * page_size;
    munmap(hdr, len);
    lock_unlock(&malloc_lock);
    return;
  }
#endif
  prev_hdr = hdr->prev;
  if(prev_hdr != NULL && (prev_hdr->flags & MALLOC_FLAG_FREE) != 0) {
    /* -----+----------+---+-----+---+-----
     *  ... | prev_hdr | ~ | hdr | ~ | ...
     * -----+----------+---+-----+---+-----
     */
    delete_header(prev_hdr);
    tmp_hdr = prev_hdr;
    tmp_hdr->size += align_up(sizeof(struct malloc_header)) + hdr->size;
  }
  next_hdr = next_header(hdr);
  if(next_hdr != NULL && (next_hdr->flags & MALLOC_FLAG_FREE) != 0) {
    /* -----+-----+---+----------+---+-----
     *  ... | hdr | ~ | next_hdr | ~ | ...
     * -----+-----+---+----------+---+-----
     */
    delete_header(next_hdr);
    tmp_hdr->size += align_up(sizeof(struct malloc_header)) + next_hdr->size;
  }
  if(next_hdr != NULL) {
    insert_header(tmp_hdr);
    next_header(tmp_hdr)->prev = tmp_hdr;
  } else {
    last_header = tmp_hdr->prev;
    sbrk(-(align_up(sizeof(struct malloc_header)) + tmp_hdr->size));
  }
  lock_unlock(&malloc_lock);
}
