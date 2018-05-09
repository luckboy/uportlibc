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
#include <sys/mman.h>
#include <sys/types.h>
#include <errno.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <CUnit/Basic.h>
#include "malloc_vars.h"
#include "sys_mock.h"
#include "sys_mock_priv.h"
#include "uportlibc.h"
#include "malloc_priv.h"

unsigned test_seed(void);

extern struct malloc_header *heap_root;
extern struct malloc_header *last_header;

struct malloc_header *find_best_header(size_t size);
void insert_header(struct malloc_header *hdr);
void delete_header(struct malloc_header *hdr);

int initialize_malloc(void)
{
  init_sys_mock();
  save_malloc_vars();
  srand(test_seed());
  return 0;
}

int cleanup_malloc(void)
{
  restore_malloc_vars();
  final_sys_mock();
  return 0; 
}

static inline size_t align_up(size_t size)
{ return (size + 7) & ~7UL; }

static inline void *align_up_ptr(void *ptr)
{ return (void *) (align_up((size_t) ptr)); }

static inline struct malloc_header *ptr_to_header(void *ptr)
{ return (struct malloc_header *) (((char*) ptr) - align_up(sizeof(struct malloc_header))); }

static struct malloc_header *prev_node = NULL;

static int compare_headers(struct malloc_header *hdr1, struct malloc_header *hdr2)
{
  if(hdr1->size > hdr2->size)
    return 1;
  else if(hdr1->size < hdr2->size)
    return -1;
  else
    return ((char *) hdr1) > ((char *) hdr2) ? 1 : (((char *) hdr1) < ((char *) hdr2) ? -1 : 0);
}

static int is_integrated_heap_node(struct malloc_header *node, int *height)
{
  int tmp_height = 1;
  if(node->left != NULL) {
    int left_height;
    if(node->left->parent != node)
      return 0;
    if(!is_integrated_heap_node(node->left, &left_height))
      return 0;
    if(tmp_height < left_height + 1) tmp_height = left_height + 1; 
  }
  if(prev_node != NULL) {
    if(compare_headers(prev_node, node) >= 0)
      return 0;
  }
  if(node->flags != MALLOC_FLAG_FREE)
    return 0;
  prev_node = node;
  if(node->right != NULL) {
    int right_height;
    if(node->right->parent != node)
      return 0;
    if(!is_integrated_heap_node(node->right, &right_height))
      return 0;
    if(tmp_height < right_height + 1) tmp_height = right_height + 1; 
  }
  if(tmp_height != node->height)
    return 0;
  if(height != NULL) *height = tmp_height;
  return 1;
}

int is_integrated_heap_tree(void)
{
  prev_node = NULL;
  return heap_root != NULL ? is_integrated_heap_node(heap_root, NULL) : 1;
}

size_t calc_heap_node_count(struct malloc_header *node)
{
  size_t count = 1;
  if(node->left != NULL) count += calc_heap_node_count(node->left);
  if(node->right != NULL) count += calc_heap_node_count(node->right);
  return count;
}

size_t calc_heap_tree_count(void)
{ return heap_root != NULL ? calc_heap_node_count(heap_root) : 0; }

int is_integrated_list(void)
{
  struct malloc_header *prev_hdr = NULL;
  struct malloc_header *hdr = (struct malloc_header *) sys_mock_data_seg;
  while(((char *) hdr) != sys_mock_break) {
    char *ptr;
    if(hdr->prev != prev_hdr) return 0;
    prev_hdr = hdr;
    ptr = ((char *) align_up_ptr(hdr + 1)) + hdr->size;
    hdr = (struct malloc_header *) ptr;
  }
  if(prev_hdr != last_header) return 0;
  return 1;
}

size_t calc_list_count(void)
{
  struct malloc_header *hdr = (struct malloc_header *) sys_mock_data_seg;
  size_t count = 0;
  while(((char *) hdr) != sys_mock_break) {
    count++;
    char *ptr;
    ptr = ((char *) align_up_ptr(hdr + 1)) + hdr->size;
    hdr = (struct malloc_header *) ptr;
  }
  return count;
}

int repeatly_equal_char(void *str, int c, size_t count)
{
  unsigned char *ptr = (unsigned char *) str;
  size_t i;
  for(i = 0; i < count; i++) {
    if(ptr[i] != ((unsigned char) c)) return 0;
  }
  return 1;
}

void test_insert_header_inserts_one_header(void)
{
  static struct malloc_header hdr;
  restore_malloc_vars();
  hdr.prev = NULL;
  hdr.size = 10;
  hdr.flags = 0;
  hdr.height = 0;
  hdr.left = NULL;
  hdr.right = NULL;
  hdr.parent = NULL;
  insert_header(&hdr);
  CU_ASSERT_PTR_EQUAL(heap_root, &hdr);
  CU_ASSERT_EQUAL(hdr.flags, MALLOC_FLAG_FREE);
  CU_ASSERT_EQUAL(hdr.height, 1);
  CU_ASSERT_PTR_NULL(hdr.left);
  CU_ASSERT_PTR_NULL(hdr.right);
  CU_ASSERT_PTR_NULL(hdr.parent);
}

void test_insert_header_inserts_seven_headers(void)
{
  static struct malloc_header hdrs[7];
  int i;
  restore_malloc_vars();
  for(i = 0; i < 7; i++) {
    hdrs[i].prev = NULL;
    hdrs[i].flags = 0;
    hdrs[i].height = 0;
    hdrs[i].left = NULL;
    hdrs[i].right = NULL;
    hdrs[i].parent = NULL;
  }
  hdrs[0].size = 100;
  hdrs[1].size = 50;
  hdrs[2].size = 150;
  hdrs[3].size = 25;
  hdrs[4].size = 75;
  hdrs[5].size = 125;
  hdrs[6].size = 175;
  for(i = 0; i < 7; i++) {
    insert_header(hdrs + i);
  }
  CU_ASSERT_PTR_EQUAL(heap_root, hdrs + 0);
  CU_ASSERT_PTR_EQUAL(heap_root->left, hdrs + 1);
  CU_ASSERT_PTR_EQUAL(heap_root->right, hdrs + 2);
  CU_ASSERT_PTR_EQUAL(heap_root->left->left, hdrs + 3);
  CU_ASSERT_PTR_EQUAL(heap_root->left->right, hdrs + 4);
  CU_ASSERT_PTR_EQUAL(heap_root->right->left, hdrs + 5);
  CU_ASSERT_PTR_EQUAL(heap_root->right->right, hdrs + 6);
  CU_ASSERT_PTR_NULL(heap_root->left->left->left);
  CU_ASSERT_PTR_NULL(heap_root->left->left->right);
  CU_ASSERT_PTR_NULL(heap_root->left->right->left);
  CU_ASSERT_PTR_NULL(heap_root->left->right->right);
  CU_ASSERT_PTR_NULL(heap_root->right->left->left);
  CU_ASSERT_PTR_NULL(heap_root->right->left->right);
  CU_ASSERT_PTR_NULL(heap_root->right->right->left);
  CU_ASSERT_PTR_NULL(heap_root->right->right->right);
  CU_ASSERT(is_integrated_heap_tree());
}

void test_insert_header_inserts_for_left_left_case(void)
{
  static struct malloc_header hdrs[10];
  int i;
  restore_malloc_vars();
  for(i = 0; i < 10; i++) {
    hdrs[i].prev = NULL;
    hdrs[i].flags = 0;
    hdrs[i].height = 0;
    hdrs[i].left = NULL;
    hdrs[i].right = NULL;
    hdrs[i].parent = NULL;
  }
  hdrs[0].size = 200;
  hdrs[1].size = 100;
  hdrs[2].size = 300;
  hdrs[3].size = 50;
  hdrs[4].size = 150;
  hdrs[5].size = 250;
  hdrs[6].size = 350;
  hdrs[7].size = 25;
  hdrs[8].size = 75;
  hdrs[9].size = 10;
  for(i = 0; i < 10; i++) {
    insert_header(hdrs + i);
  }
  CU_ASSERT_PTR_EQUAL(heap_root, hdrs + 0);
  CU_ASSERT_PTR_EQUAL(heap_root->left, hdrs + 3);
  CU_ASSERT_PTR_EQUAL(heap_root->right, hdrs + 2);
  CU_ASSERT_PTR_EQUAL(heap_root->left->left, hdrs + 7);
  CU_ASSERT_PTR_EQUAL(heap_root->left->right, hdrs + 1);
  CU_ASSERT_PTR_EQUAL(heap_root->right->left, hdrs + 5);
  CU_ASSERT_PTR_EQUAL(heap_root->right->right, hdrs + 6);
  CU_ASSERT_PTR_EQUAL(heap_root->left->left->left, hdrs + 9);
  CU_ASSERT_PTR_NULL(heap_root->left->left->right);
  CU_ASSERT_PTR_EQUAL(heap_root->left->right->left, hdrs + 8);
  CU_ASSERT_PTR_EQUAL(heap_root->left->right->right, hdrs + 4);
  CU_ASSERT_PTR_NULL(heap_root->right->left->left);
  CU_ASSERT_PTR_NULL(heap_root->right->left->right);
  CU_ASSERT_PTR_NULL(heap_root->right->right->left);
  CU_ASSERT_PTR_NULL(heap_root->right->right->right);
  CU_ASSERT_PTR_NULL(heap_root->left->left->left->left);
  CU_ASSERT_PTR_NULL(heap_root->left->left->left->right);
  CU_ASSERT_PTR_NULL(heap_root->left->right->left->left);
  CU_ASSERT_PTR_NULL(heap_root->left->right->left->right);
  CU_ASSERT_PTR_NULL(heap_root->left->right->right->left);
  CU_ASSERT_PTR_NULL(heap_root->left->right->right->right);
  CU_ASSERT(is_integrated_heap_tree());
}

void test_insert_header_inserts_for_left_right_case(void)
{
  static struct malloc_header hdrs[6];
  int i;
  restore_malloc_vars();
  for(i = 0; i < 6; i++) {
    hdrs[i].prev = NULL;
    hdrs[i].flags = 0;
    hdrs[i].height = 0;
    hdrs[i].left = NULL;
    hdrs[i].right = NULL;
    hdrs[i].parent = NULL;
  }
  hdrs[0].size = 100;
  hdrs[1].size = 50;
  hdrs[2].size = 150;
  hdrs[3].size = 25;
  hdrs[4].size = 75;
  hdrs[5].size = 80;
  for(i = 0; i < 6; i++) {
    insert_header(hdrs + i);
  }
  CU_ASSERT_PTR_EQUAL(heap_root, hdrs + 4);
  CU_ASSERT_PTR_EQUAL(heap_root->left, hdrs + 1);
  CU_ASSERT_PTR_EQUAL(heap_root->right, hdrs + 0);
  CU_ASSERT_PTR_EQUAL(heap_root->left->left, hdrs + 3);
  CU_ASSERT_PTR_NULL(heap_root->left->right);
  CU_ASSERT_PTR_EQUAL(heap_root->right->left, hdrs + 5);
  CU_ASSERT_PTR_EQUAL(heap_root->right->right, hdrs + 2);
  CU_ASSERT_PTR_NULL(heap_root->left->left->left);
  CU_ASSERT_PTR_NULL(heap_root->left->left->right);
  CU_ASSERT_PTR_NULL(heap_root->right->left->left);
  CU_ASSERT_PTR_NULL(heap_root->right->left->right);
  CU_ASSERT_PTR_NULL(heap_root->right->right->left);
  CU_ASSERT_PTR_NULL(heap_root->right->right->right);
  CU_ASSERT(is_integrated_heap_tree());
}

void test_insert_header_inserts_for_right_right_case(void)
{
  static struct malloc_header hdrs[6];
  int i;
  restore_malloc_vars();
  for(i = 0; i < 6; i++) {
    hdrs[i].prev = NULL;
    hdrs[i].flags = 0;
    hdrs[i].height = 0;
    hdrs[i].left = NULL;
    hdrs[i].right = NULL;
    hdrs[i].parent = NULL;
  }
  hdrs[0].size = 100;
  hdrs[1].size = 50;
  hdrs[2].size = 150;
  hdrs[3].size = 125;
  hdrs[4].size = 175;
  hdrs[5].size = 170;
  for(i = 0; i < 6; i++) {
    insert_header(hdrs + i);
  }
  CU_ASSERT_PTR_EQUAL(heap_root, hdrs + 2);
  CU_ASSERT_PTR_EQUAL(heap_root->left, hdrs + 0);
  CU_ASSERT_PTR_EQUAL(heap_root->right, hdrs + 4);
  CU_ASSERT_PTR_EQUAL(heap_root->left->left, hdrs + 1);
  CU_ASSERT_PTR_EQUAL(heap_root->left->right, hdrs + 3);
  CU_ASSERT_PTR_EQUAL(heap_root->right->left, hdrs + 5);
  CU_ASSERT_PTR_NULL(heap_root->right->right);
  CU_ASSERT_PTR_NULL(heap_root->left->left->left);
  CU_ASSERT_PTR_NULL(heap_root->left->left->right);
  CU_ASSERT_PTR_NULL(heap_root->left->right->left);
  CU_ASSERT_PTR_NULL(heap_root->left->right->right);
  CU_ASSERT_PTR_NULL(heap_root->right->left->left);
  CU_ASSERT_PTR_NULL(heap_root->right->left->right);
  CU_ASSERT(is_integrated_heap_tree());
}

void test_insert_header_inserts_for_right_left_case(void)
{
  static struct malloc_header hdrs[10];
  int i;
  restore_malloc_vars();
  for(i = 0; i < 10; i++) {
    hdrs[i].prev = NULL;
    hdrs[i].flags = 0;
    hdrs[i].height = 0;
    hdrs[i].left = NULL;
    hdrs[i].right = NULL;
    hdrs[i].parent = NULL;
  }
  hdrs[0].size = 200;
  hdrs[1].size = 100;
  hdrs[2].size = 300;
  hdrs[3].size = 50;
  hdrs[4].size = 150;
  hdrs[5].size = 250;
  hdrs[6].size = 350;
  hdrs[7].size = 325;
  hdrs[8].size = 375;
  hdrs[9].size = 320;
  for(i = 0; i < 10; i++) {
    insert_header(hdrs + i);
  }
  CU_ASSERT_PTR_EQUAL(heap_root, hdrs + 0);
  CU_ASSERT_PTR_EQUAL(heap_root->left, hdrs + 1);
  CU_ASSERT_PTR_EQUAL(heap_root->right, hdrs + 7);
  CU_ASSERT_PTR_EQUAL(heap_root->left->left, hdrs + 3);
  CU_ASSERT_PTR_EQUAL(heap_root->left->right, hdrs + 4); 
  CU_ASSERT_PTR_EQUAL(heap_root->right->left, hdrs + 2);
  CU_ASSERT_PTR_EQUAL(heap_root->right->right, hdrs + 6);
  CU_ASSERT_PTR_NULL(heap_root->left->left->left);
  CU_ASSERT_PTR_NULL(heap_root->left->left->right);
  CU_ASSERT_PTR_NULL(heap_root->left->right->left);
  CU_ASSERT_PTR_NULL(heap_root->left->right->right);
  CU_ASSERT_PTR_EQUAL(heap_root->right->left->left, hdrs + 5);
  CU_ASSERT_PTR_EQUAL(heap_root->right->left->right, hdrs + 9);
  CU_ASSERT_PTR_NULL(heap_root->right->right->left);
  CU_ASSERT_PTR_EQUAL(heap_root->right->right->right, hdrs + 8);
  CU_ASSERT_PTR_NULL(heap_root->right->left->left->left);
  CU_ASSERT_PTR_NULL(heap_root->right->left->left->right);
  CU_ASSERT_PTR_NULL(heap_root->right->left->right->left);
  CU_ASSERT_PTR_NULL(heap_root->right->left->right->right);
  CU_ASSERT_PTR_NULL(heap_root->right->right->right->left);
  CU_ASSERT_PTR_NULL(heap_root->right->right->right->right);
  CU_ASSERT(is_integrated_heap_tree());
}

void test_insert_header_inserts_for_two_same_sizes(void)
{
  static struct malloc_header hdrs[3];
  int i;
  restore_malloc_vars();
  for(i = 0; i < 3; i++) {
    hdrs[i].prev = NULL;
    hdrs[i].flags = 0;
    hdrs[i].height = 0;
    hdrs[i].left = NULL;
    hdrs[i].right = NULL;
    hdrs[i].parent = NULL;
  }
  hdrs[0].size = 100;
  hdrs[1].size = 50;
  hdrs[2].size = 100;
  for(i = 0; i < 3; i++) {
    insert_header(hdrs + i);
  }
  CU_ASSERT_PTR_EQUAL(heap_root, hdrs + 0);
  CU_ASSERT_PTR_EQUAL(heap_root->left, hdrs + 1);
  CU_ASSERT_PTR_EQUAL(heap_root->right, hdrs + 2);
  CU_ASSERT_PTR_NULL(heap_root->left->left);
  CU_ASSERT_PTR_NULL(heap_root->left->right);
  CU_ASSERT_PTR_NULL(heap_root->right->left);
  CU_ASSERT_PTR_NULL(heap_root->right->right);
  CU_ASSERT(is_integrated_heap_tree());
}

void test_insert_header_inserts_random_headers(void)
{
  static struct malloc_header hdrs[100];
  int i, len = floor((rand() / (RAND_MAX * 1.0)) * 50) + 50;
  restore_malloc_vars();
  for(i = 0; i < len; i++) {
    hdrs[i].prev = NULL;
    hdrs[i].size = floor((rand() / (RAND_MAX * 1.0)) * 1000.0) + 1;
    hdrs[i].flags = 0;
    hdrs[i].height = 0;
    hdrs[i].left = NULL;
    hdrs[i].right = NULL;
    hdrs[i].parent = NULL;
  }
  for(i = 0; i < len; i++) {
    insert_header(hdrs + i);
  }
  CU_ASSERT(is_integrated_heap_tree());
  CU_ASSERT_EQUAL(calc_heap_tree_count(), len);
}

void test_find_best_header_returns_header_for_one_header(void)
{
  static struct malloc_header hdr;
  restore_malloc_vars();
  hdr.prev = NULL;
  hdr.size = 10;
  hdr.flags = 0;
  hdr.height = 0;
  hdr.left = NULL;
  hdr.right = NULL;
  hdr.parent = NULL;
  insert_header(&hdr);
  CU_ASSERT_PTR_EQUAL(heap_root, &hdr);
  CU_ASSERT_EQUAL(hdr.flags, MALLOC_FLAG_FREE);
  CU_ASSERT_EQUAL(hdr.height, 1);
  CU_ASSERT_PTR_NULL(hdr.left);
  CU_ASSERT_PTR_NULL(hdr.right);
  CU_ASSERT_PTR_NULL(hdr.parent);
  CU_ASSERT_PTR_EQUAL(find_best_header(9), &hdr);
  CU_ASSERT_PTR_EQUAL(find_best_header(10), &hdr);
}

void test_find_best_header_returns_null_pointer_for_one_header(void)
{
  static struct malloc_header hdr;
  restore_malloc_vars();
  hdr.prev = NULL;
  hdr.size = 20;
  hdr.flags = 0;
  hdr.height = 0;
  hdr.left = NULL;
  hdr.right = NULL;
  hdr.parent = NULL;
  insert_header(&hdr);
  CU_ASSERT_PTR_EQUAL(heap_root, &hdr);
  CU_ASSERT_EQUAL(hdr.flags, MALLOC_FLAG_FREE);
  CU_ASSERT_EQUAL(hdr.height, 1);
  CU_ASSERT_PTR_NULL(hdr.left);
  CU_ASSERT_PTR_NULL(hdr.right);
  CU_ASSERT_PTR_NULL(hdr.parent);
  CU_ASSERT_PTR_NULL(find_best_header(21));
}

void test_find_best_header_returns_null_pointer_for_empty_heap_tree(void)
{
  restore_malloc_vars();
  CU_ASSERT_PTR_NULL(heap_root);
  CU_ASSERT_PTR_NULL(find_best_header(10));
}

void test_find_best_header_returns_header_for_seven_headers(void)
{
  static struct malloc_header hdrs[7];
  int i;
  restore_malloc_vars();
  for(i = 0; i < 7; i++) {
    hdrs[i].prev = NULL;
    hdrs[i].flags = 0;
    hdrs[i].height = 0;
    hdrs[i].left = NULL;
    hdrs[i].right = NULL;
    hdrs[i].parent = NULL;
  }
  hdrs[0].size = 200;
  hdrs[1].size = 100;
  hdrs[2].size = 300;
  hdrs[3].size = 50;
  hdrs[4].size = 150;
  hdrs[5].size = 250;
  hdrs[6].size = 350;
  for(i = 0; i < 7; i++) {
    insert_header(hdrs + i);
  }
  CU_ASSERT(is_integrated_heap_tree());
  CU_ASSERT_PTR_EQUAL(find_best_header(50), hdrs + 3);
  CU_ASSERT_PTR_EQUAL(find_best_header(150), hdrs + 4);
  CU_ASSERT_PTR_EQUAL(find_best_header(100), hdrs + 1);
  CU_ASSERT_PTR_EQUAL(find_best_header(200), hdrs + 0);
  CU_ASSERT_PTR_EQUAL(find_best_header(225), hdrs + 5);
  CU_ASSERT_PTR_EQUAL(find_best_header(325), hdrs + 6);
  CU_ASSERT_PTR_EQUAL(find_best_header(275), hdrs + 2);
  CU_ASSERT_PTR_EQUAL(find_best_header(175), hdrs + 0);
}

void test_find_best_header_returns_null_pointer_for_seven_headers(void)
{
  static struct malloc_header hdrs[7];
  int i;
  restore_malloc_vars();
  for(i = 0; i < 7; i++) {
    hdrs[i].prev = NULL;
    hdrs[i].flags = 0;
    hdrs[i].height = 0;
    hdrs[i].left = NULL;
    hdrs[i].right = NULL;
    hdrs[i].parent = NULL;
  }
  hdrs[0].size = 200;
  hdrs[1].size = 100;
  hdrs[2].size = 300;
  hdrs[3].size = 50;
  hdrs[4].size = 150;
  hdrs[5].size = 250;
  hdrs[6].size = 350;
  for(i = 0; i < 7; i++) {
    insert_header(hdrs + i);
  }
  CU_ASSERT(is_integrated_heap_tree());
  CU_ASSERT_PTR_NULL(find_best_header(375));
  CU_ASSERT_PTR_NULL(find_best_header(380));
}

void test_find_best_header_returns_header_for_random_headers(void)
{
  static struct malloc_header hdrs[100];
  int i, len = floor((rand() / (RAND_MAX * 1.0)) * 50) + 50;
  int hdr_idx = floor((rand() / (RAND_MAX * 1.0)) * (len - 1)); 
  restore_malloc_vars();
  for(i = 0; i < len; i++) {
    hdrs[i].prev = NULL;
    hdrs[i].size = floor((rand() / (RAND_MAX * 1.0)) * 1000.0) + 1;
    hdrs[i].flags = 0;
    hdrs[i].height = 0;
    hdrs[i].left = NULL;
    hdrs[i].right = NULL;
    hdrs[i].parent = NULL;
  }
  for(i = 0; i < len; i++) {
    if(hdrs[i].size == hdrs[hdr_idx].size) break;
  }
  hdr_idx = i;
  for(i = 0; i < len; i++) {
    insert_header(hdrs + i);
  }
  CU_ASSERT(is_integrated_heap_tree());
  CU_ASSERT_EQUAL(calc_heap_tree_count(), len);
  CU_ASSERT_PTR_EQUAL(find_best_header(hdrs[hdr_idx].size), hdrs + hdr_idx);
}

void test_find_best_header_returns_null_pointer_for_random_headers(void)
{
  static struct malloc_header hdrs[100];
  int i, len = floor((rand() / (RAND_MAX * 1.0)) * 50) + 50;
  int size = 0;
  restore_malloc_vars();
  for(i = 0; i < len; i++) {
    hdrs[i].prev = NULL;
    hdrs[i].size = floor((rand() / (RAND_MAX * 1.0)) * 1000.0) + 1;
    hdrs[i].flags = 0;
    hdrs[i].height = 0;
    hdrs[i].left = NULL;
    hdrs[i].right = NULL;
    hdrs[i].parent = NULL;
  }
  for(i = 0; i < len; i++) {
    if(hdrs[i].size > size) size = hdrs[i].size;
  }
  size += floor((rand() / (RAND_MAX * 1.0)) * 1000) + 1;
  for(i = 0; i < len; i++) {
    insert_header(hdrs + i);
  }
  CU_ASSERT(is_integrated_heap_tree());
  CU_ASSERT_EQUAL(calc_heap_tree_count(), len);
  CU_ASSERT_PTR_NULL(find_best_header(size));
}

void test_delete_header_deletes_one_header_for_one_header(void)
{
  static struct malloc_header hdr;
  restore_malloc_vars();
  hdr.prev = NULL;
  hdr.size = 20;
  hdr.flags = 0;
  hdr.height = 0;
  hdr.left = NULL;
  hdr.right = NULL;
  hdr.parent = NULL;
  insert_header(&hdr);
  delete_header(&hdr);
  CU_ASSERT_PTR_NULL(heap_root);
  CU_ASSERT_EQUAL(hdr.flags, 0);
  CU_ASSERT_EQUAL(hdr.height, 0);
  CU_ASSERT_PTR_NULL(hdr.left);
  CU_ASSERT_PTR_NULL(hdr.right);
  CU_ASSERT_PTR_NULL(hdr.parent);
}

void test_delete_header_deletes_one_header_for_seven_headers(void)
{
  static struct malloc_header hdrs[7];
  int i;
  restore_malloc_vars();
  for(i = 0; i < 7; i++) {
    hdrs[i].prev = NULL;
    hdrs[i].flags = 0;
    hdrs[i].height = 0;
    hdrs[i].left = NULL;
    hdrs[i].right = NULL;
    hdrs[i].parent = NULL;
  }
  hdrs[0].size = 100;
  hdrs[1].size = 50;
  hdrs[2].size = 150;
  hdrs[3].size = 25;
  hdrs[4].size = 75;
  hdrs[5].size = 125;
  hdrs[6].size = 175;
  for(i = 0; i < 7; i++) {
    insert_header(hdrs + i);
  }
  delete_header(hdrs + 4);
  CU_ASSERT_PTR_EQUAL(heap_root, hdrs + 0);
  CU_ASSERT_PTR_EQUAL(heap_root->left, hdrs + 1);
  CU_ASSERT_PTR_EQUAL(heap_root->right, hdrs + 2);
  CU_ASSERT_PTR_EQUAL(heap_root->left->left, hdrs + 3);
  CU_ASSERT_PTR_NULL(heap_root->left->right);
  CU_ASSERT_PTR_EQUAL(heap_root->right->left, hdrs + 5);
  CU_ASSERT_PTR_EQUAL(heap_root->right->right, hdrs + 6);
  CU_ASSERT_PTR_NULL(heap_root->left->left->left);
  CU_ASSERT_PTR_NULL(heap_root->left->left->right);
  CU_ASSERT_PTR_NULL(heap_root->right->left->left);
  CU_ASSERT_PTR_NULL(heap_root->right->left->right);
  CU_ASSERT_PTR_NULL(heap_root->right->right->left);
  CU_ASSERT_PTR_NULL(heap_root->right->right->right);
  CU_ASSERT(is_integrated_heap_tree());
  CU_ASSERT_EQUAL(hdrs[4].flags, 0);
  CU_ASSERT_EQUAL(hdrs[4].height, 0);
  CU_ASSERT_PTR_NULL(hdrs[4].left);
  CU_ASSERT_PTR_NULL(hdrs[4].right);
  CU_ASSERT_PTR_NULL(hdrs[4].parent);
}

void test_delete_header_deletes_for_left_left_case(void)
{
  static struct malloc_header hdrs[8];
  int i;
  restore_malloc_vars();
  for(i = 0; i < 8; i++) {
    hdrs[i].prev = NULL;
    hdrs[i].flags = 0;
    hdrs[i].height = 0;
    hdrs[i].left = NULL;
    hdrs[i].right = NULL;
    hdrs[i].parent = NULL;
  }
  hdrs[0].size = 200;
  hdrs[1].size = 100;
  hdrs[2].size = 300;
  hdrs[3].size = 50;
  hdrs[4].size = 150;
  hdrs[5].size = 350;
  hdrs[6].size = 25;
  hdrs[7].size = 75;
  for(i = 0; i < 8; i++) {
    insert_header(hdrs + i);
  }
  delete_header(hdrs + 5);
  CU_ASSERT_PTR_EQUAL(heap_root, hdrs + 1);
  CU_ASSERT_PTR_EQUAL(heap_root->left, hdrs + 3);
  CU_ASSERT_PTR_EQUAL(heap_root->right, hdrs + 0);
  CU_ASSERT_PTR_EQUAL(heap_root->left->left, hdrs + 6);
  CU_ASSERT_PTR_EQUAL(heap_root->left->right, hdrs + 7);
  CU_ASSERT_PTR_EQUAL(heap_root->right->left, hdrs + 4);
  CU_ASSERT_PTR_EQUAL(heap_root->right->right, hdrs + 2);
  CU_ASSERT_PTR_NULL(heap_root->left->left->left);
  CU_ASSERT_PTR_NULL(heap_root->left->left->right);
  CU_ASSERT_PTR_NULL(heap_root->left->right->left);
  CU_ASSERT_PTR_NULL(heap_root->left->right->right);
  CU_ASSERT_PTR_NULL(heap_root->right->left->left);
  CU_ASSERT_PTR_NULL(heap_root->right->left->right);
  CU_ASSERT_PTR_NULL(heap_root->right->right->left);
  CU_ASSERT_PTR_NULL(heap_root->right->right->right);
  CU_ASSERT(is_integrated_heap_tree());
  CU_ASSERT_EQUAL(hdrs[5].flags, 0);
  CU_ASSERT_EQUAL(hdrs[5].height, 0);
  CU_ASSERT_PTR_NULL(hdrs[5].left);
  CU_ASSERT_PTR_NULL(hdrs[5].right);
  CU_ASSERT_PTR_NULL(hdrs[5].parent);
}

void test_delete_header_deletes_for_left_right_case(void)
{
  static struct malloc_header hdrs[16];
  int i;
  restore_malloc_vars();
  for(i = 0; i < 16; i++) {
    hdrs[i].prev = NULL;
    hdrs[i].flags = 0;
    hdrs[i].height = 0;
    hdrs[i].left = NULL;
    hdrs[i].right = NULL;
    hdrs[i].parent = NULL;
  }
  hdrs[0].size = 400;
  hdrs[1].size = 200;
  hdrs[2].size = 600;
  hdrs[3].size = 100;
  hdrs[4].size = 300;
  hdrs[5].size = 500;
  hdrs[6].size = 700;
  hdrs[7].size = 50;
  hdrs[8].size = 150;
  hdrs[9].size = 250;
  hdrs[10].size = 350;
  hdrs[11].size = 450;
  hdrs[12].size = 550;
  hdrs[13].size = 750;
  hdrs[14].size = 525;
  hdrs[15].size = 575;
  for(i = 0; i < 16; i++) {
    insert_header(hdrs + i);
  }
  delete_header(hdrs + 13);
  CU_ASSERT_PTR_EQUAL(heap_root, hdrs + 0);
  CU_ASSERT_PTR_EQUAL(heap_root->left, hdrs + 1);
  CU_ASSERT_PTR_EQUAL(heap_root->right, hdrs + 12);
  CU_ASSERT_PTR_EQUAL(heap_root->left->left, hdrs + 3);
  CU_ASSERT_PTR_EQUAL(heap_root->left->right, hdrs + 4);
  CU_ASSERT_PTR_EQUAL(heap_root->right->left, hdrs + 5);
  CU_ASSERT_PTR_EQUAL(heap_root->right->right, hdrs + 2);
  CU_ASSERT_PTR_EQUAL(heap_root->left->left->left, hdrs + 7);
  CU_ASSERT_PTR_EQUAL(heap_root->left->left->right, hdrs + 8);
  CU_ASSERT_PTR_EQUAL(heap_root->left->right->left, hdrs + 9);
  CU_ASSERT_PTR_EQUAL(heap_root->left->right->right, hdrs + 10);
  CU_ASSERT_PTR_EQUAL(heap_root->right->left->left, hdrs + 11);
  CU_ASSERT_PTR_EQUAL(heap_root->right->left->right, hdrs + 14);
  CU_ASSERT_PTR_EQUAL(heap_root->right->right->left, hdrs + 15);
  CU_ASSERT_PTR_EQUAL(heap_root->right->right->right, hdrs + 6);
  CU_ASSERT_PTR_NULL(heap_root->left->left->left->left);
  CU_ASSERT_PTR_NULL(heap_root->left->left->left->right);
  CU_ASSERT_PTR_NULL(heap_root->left->left->right->left);
  CU_ASSERT_PTR_NULL(heap_root->left->left->right->right);
  CU_ASSERT_PTR_NULL(heap_root->left->right->left->left);
  CU_ASSERT_PTR_NULL(heap_root->left->right->left->right);
  CU_ASSERT_PTR_NULL(heap_root->left->right->right->left);
  CU_ASSERT_PTR_NULL(heap_root->left->right->right->right);
  CU_ASSERT_PTR_NULL(heap_root->right->left->left->left);
  CU_ASSERT_PTR_NULL(heap_root->right->left->left->right);
  CU_ASSERT_PTR_NULL(heap_root->right->left->right->left);
  CU_ASSERT_PTR_NULL(heap_root->right->left->right->right);
  CU_ASSERT_PTR_NULL(heap_root->right->right->left->left);
  CU_ASSERT_PTR_NULL(heap_root->right->right->left->right);
  CU_ASSERT_PTR_NULL(heap_root->right->right->right->left);
  CU_ASSERT_PTR_NULL(heap_root->right->right->right->right);
  CU_ASSERT(is_integrated_heap_tree());
  CU_ASSERT_EQUAL(hdrs[13].flags, 0);
  CU_ASSERT_EQUAL(hdrs[13].height, 0);
  CU_ASSERT_PTR_NULL(hdrs[13].left);
  CU_ASSERT_PTR_NULL(hdrs[13].right);
  CU_ASSERT_PTR_NULL(hdrs[13].parent);
}

void test_delete_header_deletes_for_right_right_case(void)
{
  static struct malloc_header hdrs[16];
  int i;
  restore_malloc_vars();
  for(i = 0; i < 16; i++) {
    hdrs[i].prev = NULL;
    hdrs[i].flags = 0;
    hdrs[i].height = 0;
    hdrs[i].left = NULL;
    hdrs[i].right = NULL;
    hdrs[i].parent = NULL;
  }
  hdrs[0].size = 400;
  hdrs[1].size = 200;
  hdrs[2].size = 600;
  hdrs[3].size = 100;
  hdrs[4].size = 300;
  hdrs[5].size = 500;
  hdrs[6].size = 700;
  hdrs[7].size = 50;
  hdrs[8].size = 250;
  hdrs[9].size = 350;
  hdrs[10].size = 450;
  hdrs[11].size = 550;
  hdrs[12].size = 650;
  hdrs[13].size = 750;
  hdrs[14].size = 325;
  hdrs[15].size = 375;
  for(i = 0; i < 16; i++) {
    insert_header(hdrs + i);
  }
  delete_header(hdrs + 7);
  CU_ASSERT_PTR_EQUAL(heap_root, hdrs + 0);
  CU_ASSERT_PTR_EQUAL(heap_root->left, hdrs + 4);
  CU_ASSERT_PTR_EQUAL(heap_root->right, hdrs + 2);
  CU_ASSERT_PTR_EQUAL(heap_root->left->left, hdrs + 1);
  CU_ASSERT_PTR_EQUAL(heap_root->left->right, hdrs + 9);
  CU_ASSERT_PTR_EQUAL(heap_root->right->left, hdrs + 5);
  CU_ASSERT_PTR_EQUAL(heap_root->right->right, hdrs + 6);
  CU_ASSERT_PTR_EQUAL(heap_root->left->left->left, hdrs + 3);
  CU_ASSERT_PTR_EQUAL(heap_root->left->left->right, hdrs + 8);
  CU_ASSERT_PTR_EQUAL(heap_root->left->right->left, hdrs + 14);
  CU_ASSERT_PTR_EQUAL(heap_root->left->right->right, hdrs + 15);
  CU_ASSERT_PTR_EQUAL(heap_root->right->left->left, hdrs + 10);
  CU_ASSERT_PTR_EQUAL(heap_root->right->left->right, hdrs + 11);
  CU_ASSERT_PTR_EQUAL(heap_root->right->right->left, hdrs + 12);
  CU_ASSERT_PTR_EQUAL(heap_root->right->right->right, hdrs + 13);
  CU_ASSERT_PTR_NULL(heap_root->left->left->left->left);
  CU_ASSERT_PTR_NULL(heap_root->left->left->left->right);
  CU_ASSERT_PTR_NULL(heap_root->left->left->right->left);
  CU_ASSERT_PTR_NULL(heap_root->left->left->right->right);
  CU_ASSERT_PTR_NULL(heap_root->left->right->left->left);
  CU_ASSERT_PTR_NULL(heap_root->left->right->left->right);
  CU_ASSERT_PTR_NULL(heap_root->left->right->right->left);
  CU_ASSERT_PTR_NULL(heap_root->left->right->right->right);
  CU_ASSERT_PTR_NULL(heap_root->right->left->left->left);
  CU_ASSERT_PTR_NULL(heap_root->right->left->left->right);
  CU_ASSERT_PTR_NULL(heap_root->right->left->right->left);
  CU_ASSERT_PTR_NULL(heap_root->right->left->right->right);
  CU_ASSERT_PTR_NULL(heap_root->right->right->left->left);
  CU_ASSERT_PTR_NULL(heap_root->right->right->left->right);
  CU_ASSERT_PTR_NULL(heap_root->right->right->right->left);
  CU_ASSERT_PTR_NULL(heap_root->right->right->right->right);
  CU_ASSERT(is_integrated_heap_tree());
  CU_ASSERT_EQUAL(hdrs[7].flags, 0);
  CU_ASSERT_EQUAL(hdrs[7].height, 0);
  CU_ASSERT_PTR_NULL(hdrs[7].left);
  CU_ASSERT_PTR_NULL(hdrs[7].right);
  CU_ASSERT_PTR_NULL(hdrs[7].parent);
}

void test_delete_header_deletes_for_right_left_case(void)
{
  static struct malloc_header hdrs[8];
  int i;
  restore_malloc_vars();
  for(i = 0; i < 8; i++) {
    hdrs[i].prev = NULL;
    hdrs[i].flags = 0;
    hdrs[i].height = 0;
    hdrs[i].left = NULL;
    hdrs[i].right = NULL;
    hdrs[i].parent = NULL;
  }
  hdrs[0].size = 200;
  hdrs[1].size = 100;
  hdrs[2].size = 300;
  hdrs[3].size = 150;
  hdrs[4].size = 250;
  hdrs[5].size = 350;
  hdrs[6].size = 225;
  hdrs[7].size = 275;
  for(i = 0; i < 8; i++) {
    insert_header(hdrs + i);
  }
  delete_header(hdrs + 3);
  CU_ASSERT_PTR_EQUAL(heap_root, hdrs + 4);
  CU_ASSERT_PTR_EQUAL(heap_root->left, hdrs + 0);
  CU_ASSERT_PTR_EQUAL(heap_root->right, hdrs + 2);
  CU_ASSERT_PTR_EQUAL(heap_root->left->left, hdrs + 1);
  CU_ASSERT_PTR_EQUAL(heap_root->left->right, hdrs + 6);
  CU_ASSERT_PTR_EQUAL(heap_root->right->left, hdrs + 7);
  CU_ASSERT_PTR_EQUAL(heap_root->right->right, hdrs + 5);
  CU_ASSERT_PTR_NULL(heap_root->left->left->left);
  CU_ASSERT_PTR_NULL(heap_root->left->left->right);
  CU_ASSERT_PTR_NULL(heap_root->left->right->left);
  CU_ASSERT_PTR_NULL(heap_root->left->right->right);
  CU_ASSERT_PTR_NULL(heap_root->right->left->left);
  CU_ASSERT_PTR_NULL(heap_root->right->left->right);
  CU_ASSERT_PTR_NULL(heap_root->right->right->left);
  CU_ASSERT_PTR_NULL(heap_root->right->right->right);
  CU_ASSERT(is_integrated_heap_tree());
  CU_ASSERT_EQUAL(hdrs[3].flags, 0);
  CU_ASSERT_EQUAL(hdrs[3].height, 0);
  CU_ASSERT_PTR_NULL(hdrs[3].left);
  CU_ASSERT_PTR_NULL(hdrs[3].right);
  CU_ASSERT_PTR_NULL(hdrs[3].parent);
}

void test_delete_header_replaces_header_for_left_right_node_with_branch(void)
{
  static struct malloc_header hdrs[7];
  int i;
  restore_malloc_vars();
  for(i = 0; i < 7; i++) {
    hdrs[i].prev = NULL;
    hdrs[i].flags = 0;
    hdrs[i].height = 0;
    hdrs[i].left = NULL;
    hdrs[i].right = NULL;
    hdrs[i].parent = NULL;
  }
  hdrs[0].size = 200;
  hdrs[1].size = 100;
  hdrs[2].size = 300;
  hdrs[3].size = 50;
  hdrs[4].size = 150;
  hdrs[5].size = 350;
  hdrs[6].size = 125;
  for(i = 0; i < 7; i++) {
    insert_header(hdrs + i);
  }
  delete_header(hdrs + 0);
  CU_ASSERT_PTR_EQUAL(heap_root, hdrs + 4);
  CU_ASSERT_PTR_EQUAL(heap_root->left, hdrs + 1);
  CU_ASSERT_PTR_EQUAL(heap_root->right, hdrs + 2);
  CU_ASSERT_PTR_EQUAL(heap_root->left->left, hdrs + 3);
  CU_ASSERT_PTR_EQUAL(heap_root->left->right, hdrs + 6);
  CU_ASSERT_PTR_NULL(heap_root->right->left);
  CU_ASSERT_PTR_EQUAL(heap_root->right->right, hdrs + 5);
  CU_ASSERT_PTR_NULL(heap_root->left->left->left);
  CU_ASSERT_PTR_NULL(heap_root->left->left->right);
  CU_ASSERT_PTR_NULL(heap_root->left->right->left);
  CU_ASSERT_PTR_NULL(heap_root->left->right->right);
  CU_ASSERT_PTR_NULL(heap_root->right->right->left);
  CU_ASSERT_PTR_NULL(heap_root->right->right->right);
  CU_ASSERT(is_integrated_heap_tree());
  CU_ASSERT_EQUAL(hdrs[0].flags, 0);
  CU_ASSERT_EQUAL(hdrs[0].height, 0);
  CU_ASSERT_PTR_NULL(hdrs[0].left);
  CU_ASSERT_PTR_NULL(hdrs[0].right);
  CU_ASSERT_PTR_NULL(hdrs[0].parent);
}

void test_delete_header_replaces_header_for_left_right_node_without_branch(void)
{
  static struct malloc_header hdrs[6];
  int i;
  restore_malloc_vars();
  for(i = 0; i < 6; i++) {
    hdrs[i].prev = NULL;
    hdrs[i].flags = 0;
    hdrs[i].height = 0;
    hdrs[i].left = NULL;
    hdrs[i].right = NULL;
    hdrs[i].parent = NULL;
  }
  hdrs[0].size = 100;
  hdrs[1].size = 50;
  hdrs[2].size = 150;
  hdrs[3].size = 25;
  hdrs[4].size = 75;
  hdrs[5].size = 175;
  for(i = 0; i < 6; i++) {
    insert_header(hdrs + i);
  }
  delete_header(hdrs + 0);
  CU_ASSERT_PTR_EQUAL(heap_root, hdrs + 4);
  CU_ASSERT_PTR_EQUAL(heap_root->left, hdrs + 1);
  CU_ASSERT_PTR_EQUAL(heap_root->right, hdrs + 2);
  CU_ASSERT_PTR_EQUAL(heap_root->left->left, hdrs + 3);
  CU_ASSERT_PTR_NULL(heap_root->left->right);
  CU_ASSERT_PTR_NULL(heap_root->right->left);
  CU_ASSERT_PTR_EQUAL(heap_root->right->right, hdrs + 5);
  CU_ASSERT_PTR_NULL(heap_root->left->left->left);
  CU_ASSERT_PTR_NULL(heap_root->left->left->right);
  CU_ASSERT_PTR_NULL(heap_root->right->right->left);
  CU_ASSERT_PTR_NULL(heap_root->right->right->right);
  CU_ASSERT(is_integrated_heap_tree());
  CU_ASSERT_EQUAL(hdrs[0].flags, 0);
  CU_ASSERT_EQUAL(hdrs[0].height, 0);
  CU_ASSERT_PTR_NULL(hdrs[0].left);
  CU_ASSERT_PTR_NULL(hdrs[0].right);
  CU_ASSERT_PTR_NULL(hdrs[0].parent);
}

void test_delete_header_replaces_header_for_left_right_node_without_branch_and_right_right_case(void)
{
  static struct malloc_header hdrs[8];
  int i;
  restore_malloc_vars();
  for(i = 0; i < 8; i++) {
    hdrs[i].prev = NULL;
    hdrs[i].flags = 0;
    hdrs[i].height = 0;
    hdrs[i].left = NULL;
    hdrs[i].right = NULL;
    hdrs[i].parent = NULL;
  }
  hdrs[0].size = 200;
  hdrs[1].size = 100;
  hdrs[2].size = 300;
  hdrs[3].size = 150;
  hdrs[4].size = 250;
  hdrs[5].size = 350;
  hdrs[6].size = 325;
  hdrs[7].size = 375;
  for(i = 0; i < 8; i++) {
    insert_header(hdrs + i);
  }
  delete_header(hdrs + 0);
  CU_ASSERT_PTR_EQUAL(heap_root, hdrs + 2);
  CU_ASSERT_PTR_EQUAL(heap_root->left, hdrs + 3);
  CU_ASSERT_PTR_EQUAL(heap_root->right, hdrs + 5);
  CU_ASSERT_PTR_EQUAL(heap_root->left->left, hdrs + 1);
  CU_ASSERT_PTR_EQUAL(heap_root->left->right, hdrs + 4);
  CU_ASSERT_PTR_EQUAL(heap_root->right->left, hdrs + 6);
  CU_ASSERT_PTR_EQUAL(heap_root->right->right, hdrs + 7);
  CU_ASSERT_PTR_NULL(heap_root->left->left->left);
  CU_ASSERT_PTR_NULL(heap_root->left->left->right);
  CU_ASSERT_PTR_NULL(heap_root->left->right->left);
  CU_ASSERT_PTR_NULL(heap_root->left->right->right);
  CU_ASSERT_PTR_NULL(heap_root->right->left->left);
  CU_ASSERT_PTR_NULL(heap_root->right->left->right);
  CU_ASSERT_PTR_NULL(heap_root->right->right->left);
  CU_ASSERT_PTR_NULL(heap_root->right->right->right);
  CU_ASSERT(is_integrated_heap_tree());
  CU_ASSERT_EQUAL(hdrs[0].flags, 0);
  CU_ASSERT_EQUAL(hdrs[0].height, 0);
  CU_ASSERT_PTR_NULL(hdrs[0].left);
  CU_ASSERT_PTR_NULL(hdrs[0].right);
  CU_ASSERT_PTR_NULL(hdrs[0].parent);
}

void test_delete_header_replaces_header_for_left_node_with_branch(void)
{
  static struct malloc_header hdrs[8];
  int i;
  restore_malloc_vars();
  for(i = 0; i < 8; i++) {
    hdrs[i].prev = NULL;
    hdrs[i].flags = 0;
    hdrs[i].height = 0;
    hdrs[i].left = NULL;
    hdrs[i].right = NULL;
    hdrs[i].parent = NULL;
  }
  hdrs[0].size = 200;
  hdrs[1].size = 100;
  hdrs[2].size = 300;
  hdrs[3].size = 50;
  hdrs[4].size = 150;
  hdrs[5].size = 250;
  hdrs[6].size = 350;
  hdrs[7].size = 25;
  for(i = 0; i < 8; i++) {
    insert_header(hdrs + i);
  }
  delete_header(hdrs + 1);
  CU_ASSERT_PTR_EQUAL(heap_root, hdrs + 0);
  CU_ASSERT_PTR_EQUAL(heap_root->left, hdrs + 3);
  CU_ASSERT_PTR_EQUAL(heap_root->right, hdrs + 2);
  CU_ASSERT_PTR_EQUAL(heap_root->left->left, hdrs + 7);
  CU_ASSERT_PTR_EQUAL(heap_root->left->right, hdrs + 4);
  CU_ASSERT_PTR_EQUAL(heap_root->right->left, hdrs + 5);
  CU_ASSERT_PTR_EQUAL(heap_root->right->right, hdrs + 6);
  CU_ASSERT_PTR_NULL(heap_root->left->left->left);
  CU_ASSERT_PTR_NULL(heap_root->left->left->right);
  CU_ASSERT_PTR_NULL(heap_root->left->right->left);
  CU_ASSERT_PTR_NULL(heap_root->left->right->right);
  CU_ASSERT_PTR_NULL(heap_root->right->left->left);
  CU_ASSERT_PTR_NULL(heap_root->right->left->right);
  CU_ASSERT_PTR_NULL(heap_root->right->right->left);
  CU_ASSERT_PTR_NULL(heap_root->right->right->right);
  CU_ASSERT(is_integrated_heap_tree());
  CU_ASSERT_EQUAL(hdrs[1].flags, 0);
  CU_ASSERT_EQUAL(hdrs[1].height, 0);
  CU_ASSERT_PTR_NULL(hdrs[1].left);
  CU_ASSERT_PTR_NULL(hdrs[1].right);
  CU_ASSERT_PTR_NULL(hdrs[1].parent);
}

void test_delete_header_replaces_header_for_left_node_without_branch(void)
{
  static struct malloc_header hdrs[6];
  int i;
  restore_malloc_vars();
  for(i = 0; i < 6; i++) {
    hdrs[i].prev = NULL;
    hdrs[i].flags = 0;
    hdrs[i].height = 0;
    hdrs[i].left = NULL;
    hdrs[i].right = NULL;
    hdrs[i].parent = NULL;
  }
  hdrs[0].size = 100;
  hdrs[1].size = 50;
  hdrs[2].size = 150;
  hdrs[3].size = 25;
  hdrs[4].size = 75;
  hdrs[5].size = 125;
  for(i = 0; i < 6; i++) {
    insert_header(hdrs + i);
  }
  delete_header(hdrs + 2);
  CU_ASSERT_PTR_EQUAL(heap_root, hdrs + 0);
  CU_ASSERT_PTR_EQUAL(heap_root->left, hdrs + 1);
  CU_ASSERT_PTR_EQUAL(heap_root->right, hdrs + 5);
  CU_ASSERT_PTR_EQUAL(heap_root->left->left, hdrs + 3);
  CU_ASSERT_PTR_EQUAL(heap_root->left->right, hdrs + 4);
  CU_ASSERT_PTR_NULL(heap_root->right->left);
  CU_ASSERT_PTR_NULL(heap_root->right->right);
  CU_ASSERT_PTR_NULL(heap_root->left->left->left);
  CU_ASSERT_PTR_NULL(heap_root->left->left->right);
  CU_ASSERT_PTR_NULL(heap_root->left->right->left);
  CU_ASSERT_PTR_NULL(heap_root->left->right->right);
  CU_ASSERT(is_integrated_heap_tree());
  CU_ASSERT_EQUAL(hdrs[2].flags, 0);
  CU_ASSERT_EQUAL(hdrs[2].height, 0);
  CU_ASSERT_PTR_NULL(hdrs[2].left);
  CU_ASSERT_PTR_NULL(hdrs[2].right);
  CU_ASSERT_PTR_NULL(hdrs[2].parent);
}

void test_delete_header_replaces_header_for_right_left_node_with_branch(void)
{
  static struct malloc_header hdrs[7];
  int i;
  restore_malloc_vars();
  for(i = 0; i < 7; i++) {
    hdrs[i].prev = NULL;
    hdrs[i].flags = 0;
    hdrs[i].height = 0;
    hdrs[i].left = NULL;
    hdrs[i].right = NULL;
    hdrs[i].parent = NULL;
  }
  hdrs[0].size = 200;
  hdrs[1].size = 100;
  hdrs[2].size = 300;
  hdrs[3].size = 50;
  hdrs[4].size = 250;
  hdrs[5].size = 350;
  hdrs[6].size = 275;
  for(i = 0; i < 7; i++) {
    insert_header(hdrs + i);
  }
  delete_header(hdrs + 0);
  CU_ASSERT_PTR_EQUAL(heap_root, hdrs + 4);
  CU_ASSERT_PTR_EQUAL(heap_root->left, hdrs + 1);
  CU_ASSERT_PTR_EQUAL(heap_root->right, hdrs + 2);
  CU_ASSERT_PTR_EQUAL(heap_root->left->left, hdrs + 3);
  CU_ASSERT_PTR_NULL(heap_root->left->right);
  CU_ASSERT_PTR_EQUAL(heap_root->right->left, hdrs + 6);
  CU_ASSERT_PTR_EQUAL(heap_root->right->right, hdrs + 5);
  CU_ASSERT_PTR_NULL(heap_root->left->left->left);
  CU_ASSERT_PTR_NULL(heap_root->left->left->right);
  CU_ASSERT_PTR_NULL(heap_root->right->left->left);
  CU_ASSERT_PTR_NULL(heap_root->right->left->right);
  CU_ASSERT_PTR_NULL(heap_root->right->right->left);
  CU_ASSERT_PTR_NULL(heap_root->right->right->right);
  CU_ASSERT(is_integrated_heap_tree());
  CU_ASSERT_EQUAL(hdrs[0].flags, 0);
  CU_ASSERT_EQUAL(hdrs[0].height, 0);
  CU_ASSERT_PTR_NULL(hdrs[0].left);
  CU_ASSERT_PTR_NULL(hdrs[0].right);
  CU_ASSERT_PTR_NULL(hdrs[0].parent);
}

void test_delete_header_replaces_header_for_right_left_node_without_branch(void)
{
  static struct malloc_header hdrs[6];
  int i;
  restore_malloc_vars();
  for(i = 0; i < 6; i++) {
    hdrs[i].prev = NULL;
    hdrs[i].flags = 0;
    hdrs[i].height = 0;
    hdrs[i].left = NULL;
    hdrs[i].right = NULL;
    hdrs[i].parent = NULL;
  }
  hdrs[0].size = 100;
  hdrs[1].size = 50;
  hdrs[2].size = 150;
  hdrs[3].size = 25;
  hdrs[4].size = 125;
  hdrs[5].size = 175;
  for(i = 0; i < 6; i++) {
    insert_header(hdrs + i);
  }
  delete_header(hdrs + 0);
  CU_ASSERT_PTR_EQUAL(heap_root, hdrs + 4);
  CU_ASSERT_PTR_EQUAL(heap_root->left, hdrs + 1);
  CU_ASSERT_PTR_EQUAL(heap_root->right, hdrs + 2);
  CU_ASSERT_PTR_EQUAL(heap_root->left->left, hdrs + 3);
  CU_ASSERT_PTR_NULL(heap_root->left->right);
  CU_ASSERT_PTR_NULL(heap_root->right->left);
  CU_ASSERT_PTR_EQUAL(heap_root->right->right, hdrs + 5);
  CU_ASSERT_PTR_NULL(heap_root->left->left->left);
  CU_ASSERT_PTR_NULL(heap_root->left->left->right);
  CU_ASSERT_PTR_NULL(heap_root->right->right->left);
  CU_ASSERT_PTR_NULL(heap_root->right->right->right);
  CU_ASSERT(is_integrated_heap_tree());
  CU_ASSERT_EQUAL(hdrs[0].flags, 0);
  CU_ASSERT_EQUAL(hdrs[0].height, 0);
  CU_ASSERT_PTR_NULL(hdrs[0].left);
  CU_ASSERT_PTR_NULL(hdrs[0].right);
  CU_ASSERT_PTR_NULL(hdrs[0].parent);
}

void test_delete_header_replaces_header_for_right_node_without_branch(void)
{
  static struct malloc_header hdrs[6];
  int i;
  restore_malloc_vars();
  for(i = 0; i < 6; i++) {
    hdrs[i].prev = NULL;
    hdrs[i].flags = 0;
    hdrs[i].height = 0;
    hdrs[i].left = NULL;
    hdrs[i].right = NULL;
    hdrs[i].parent = NULL;
  }
  hdrs[0].size = 100;
  hdrs[1].size = 50;
  hdrs[2].size = 150;
  hdrs[3].size = 75;
  hdrs[4].size = 125;
  hdrs[5].size = 175;
  for(i = 0; i < 6; i++) {
    insert_header(hdrs + i);
  }
  delete_header(hdrs + 1);
  CU_ASSERT_PTR_EQUAL(heap_root, hdrs + 0);
  CU_ASSERT_PTR_EQUAL(heap_root->left, hdrs + 3);
  CU_ASSERT_PTR_EQUAL(heap_root->right, hdrs + 2);
  CU_ASSERT_PTR_NULL(heap_root->left->left);
  CU_ASSERT_PTR_NULL(heap_root->left->right);
  CU_ASSERT_PTR_EQUAL(heap_root->right->left, hdrs + 4);
  CU_ASSERT_PTR_EQUAL(heap_root->right->right, hdrs + 5);
  CU_ASSERT_PTR_NULL(heap_root->right->left->left);
  CU_ASSERT_PTR_NULL(heap_root->right->left->right);
  CU_ASSERT_PTR_NULL(heap_root->right->right->left);
  CU_ASSERT_PTR_NULL(heap_root->right->right->right);
  CU_ASSERT(is_integrated_heap_tree());
  CU_ASSERT_EQUAL(hdrs[1].flags, 0);
  CU_ASSERT_EQUAL(hdrs[1].height, 0);
  CU_ASSERT_PTR_NULL(hdrs[1].left);
  CU_ASSERT_PTR_NULL(hdrs[1].right);
  CU_ASSERT_PTR_NULL(hdrs[1].parent);
}

void test_delete_header_deletes_for_random_headers(void)
{
  static struct malloc_header hdrs[100];
  int i, len = floor((rand() / (RAND_MAX * 1.0)) * 50) + 50;
  int hdr_idx = floor((rand() / (RAND_MAX * 1.0)) * (len - 1)); 
  restore_malloc_vars();
  for(i = 0; i < len; i++) {
    hdrs[i].prev = NULL;
    hdrs[i].size = floor((rand() / (RAND_MAX * 1.0)) * 1000.0) + 1;
    hdrs[i].flags = 0;
    hdrs[i].height = 0;
    hdrs[i].left = NULL;
    hdrs[i].right = NULL;
    hdrs[i].parent = NULL;
  }
  for(i = 0; i < len; i++) {
    insert_header(hdrs + i);
  }
  delete_header(hdrs + hdr_idx);
  CU_ASSERT(is_integrated_heap_tree());
  CU_ASSERT_EQUAL(calc_heap_tree_count(), len - 1);
  CU_ASSERT_EQUAL(hdrs[hdr_idx].flags, 0);
  CU_ASSERT_EQUAL(hdrs[hdr_idx].height, 0);
  CU_ASSERT_PTR_NULL(hdrs[hdr_idx].left);
  CU_ASSERT_PTR_NULL(hdrs[hdr_idx].right);
  CU_ASSERT_PTR_NULL(hdrs[hdr_idx].parent);
}

void test_malloc_allocates_memory(void)
{
  void *ptr;
  clear_sys_mock();
  restore_malloc_vars();
  errno = 0;
  ptr = uportlibc_malloc(10);
  CU_ASSERT_PTR_EQUAL(ptr, sys_mock_data_seg + align_up(sizeof(struct malloc_header)));
  CU_ASSERT_EQUAL(errno, 0);
  CU_ASSERT_EQUAL(sys_mock_op_count, 1);
  CU_ASSERT_STRING_EQUAL(sys_mock_ops[0].fun_name, "sbrk");
  CU_ASSERT_EQUAL(sys_mock_ops[0].args[0].i, align_up(sizeof(struct malloc_header)) + 16);
  CU_ASSERT_PTR_EQUAL(sys_mock_ops[0].res.vp, sys_mock_data_seg);
  CU_ASSERT_EQUAL(sys_mock_ops[0].err_num, 0);
  CU_ASSERT_PTR_EQUAL(sys_mock_break, sys_mock_data_seg + align_up(sizeof(struct malloc_header)) * 1 + 16);
  CU_ASSERT_EQUAL(ptr_to_header(ptr)->size, 16);
  CU_ASSERT(is_integrated_heap_tree());
  CU_ASSERT_EQUAL(calc_heap_tree_count(), 0);
  CU_ASSERT(is_integrated_list());
  CU_ASSERT_EQUAL(calc_list_count(), 1);
}

void test_malloc_allocates_three_memory_areas(void)
{
  void *ptr1;
  void *ptr2;
  void *ptr3;
  clear_sys_mock();
  restore_malloc_vars();
  errno = 0;
  ptr1 = uportlibc_malloc(100);
  CU_ASSERT_PTR_EQUAL(ptr1, sys_mock_data_seg + align_up(sizeof(struct malloc_header)));
  CU_ASSERT_EQUAL(errno, 0);
  CU_ASSERT_EQUAL(sys_mock_op_count, 1);
  CU_ASSERT_STRING_EQUAL(sys_mock_ops[0].fun_name, "sbrk");
  CU_ASSERT_EQUAL(sys_mock_ops[0].args[0].i, align_up(sizeof(struct malloc_header)) + 104);
  CU_ASSERT_PTR_EQUAL(sys_mock_ops[0].res.vp, sys_mock_data_seg);
  CU_ASSERT_EQUAL(sys_mock_ops[0].err_num, 0);
  CU_ASSERT_PTR_EQUAL(sys_mock_break, sys_mock_data_seg + align_up(sizeof(struct malloc_header)) * 1 + 104);
  CU_ASSERT_EQUAL(ptr_to_header(ptr1)->size, 104);
  CU_ASSERT(is_integrated_heap_tree());
  CU_ASSERT_EQUAL(calc_heap_tree_count(), 0);
  CU_ASSERT(is_integrated_list());
  CU_ASSERT_EQUAL(calc_list_count(), 1);
  errno = 0;
  ptr2 = uportlibc_malloc(200);
  CU_ASSERT_PTR_EQUAL(ptr2, sys_mock_data_seg + align_up(sizeof(struct malloc_header)) * 2 + 104);
  CU_ASSERT_EQUAL(errno, 0);
  CU_ASSERT_EQUAL(sys_mock_op_count, 2);
  CU_ASSERT_STRING_EQUAL(sys_mock_ops[1].fun_name, "sbrk");
  CU_ASSERT_EQUAL(sys_mock_ops[1].args[0].i, align_up(sizeof(struct malloc_header)) + 200);
  CU_ASSERT_PTR_EQUAL(sys_mock_ops[1].res.vp, sys_mock_data_seg + align_up(sizeof(struct malloc_header)) * 1 + 104);
  CU_ASSERT_EQUAL(sys_mock_ops[1].err_num, 0);
  CU_ASSERT_PTR_EQUAL(sys_mock_break, sys_mock_data_seg + align_up(sizeof(struct malloc_header)) * 2 + 304);
  CU_ASSERT_EQUAL(ptr_to_header(ptr2)->size, 200);
  CU_ASSERT(is_integrated_heap_tree());
  CU_ASSERT_EQUAL(calc_heap_tree_count(), 0);
  CU_ASSERT(is_integrated_list());                                             
  CU_ASSERT_EQUAL(calc_list_count(), 2);
  errno = 0;
  ptr3 = uportlibc_malloc(300);
  CU_ASSERT_PTR_EQUAL(ptr3, sys_mock_data_seg + align_up(sizeof(struct malloc_header)) * 3 + 304);
  CU_ASSERT_EQUAL(errno, 0);
  CU_ASSERT_EQUAL(sys_mock_op_count, 3);
  CU_ASSERT_STRING_EQUAL(sys_mock_ops[2].fun_name, "sbrk");
  CU_ASSERT_EQUAL(sys_mock_ops[2].args[0].i, align_up(sizeof(struct malloc_header)) + 304);
  CU_ASSERT_PTR_EQUAL(sys_mock_ops[2].res.vp, sys_mock_data_seg + align_up(sizeof(struct malloc_header)) * 2 + 304);
  CU_ASSERT_EQUAL(sys_mock_ops[2].err_num, 0);
  CU_ASSERT_PTR_EQUAL(sys_mock_break, sys_mock_data_seg + align_up(sizeof(struct malloc_header)) * 3 + 608);
  CU_ASSERT_EQUAL(ptr_to_header(ptr3)->size, 304);
  CU_ASSERT(is_integrated_heap_tree());
  CU_ASSERT_EQUAL(calc_heap_tree_count(), 0);
  CU_ASSERT(is_integrated_list());
  CU_ASSERT_EQUAL(calc_list_count(), 3);
}

void test_malloc_returns_null_pointer_for_size_that_is_zero(void)
{
  void *ptr;
  clear_sys_mock();
  restore_malloc_vars();
  errno = 0;
  ptr = uportlibc_malloc(0);
  CU_ASSERT_PTR_NULL(ptr);
  CU_ASSERT_EQUAL(errno, 0);
  CU_ASSERT_EQUAL(sys_mock_op_count, 0);
  CU_ASSERT_PTR_EQUAL(sys_mock_break, sys_mock_data_seg);
  CU_ASSERT(is_integrated_heap_tree());
  CU_ASSERT_EQUAL(calc_heap_tree_count(), 0);
  CU_ASSERT(is_integrated_list());
  CU_ASSERT_EQUAL(calc_list_count(), 0);
}

void test_malloc_allocates_freed_memory_area(void)
{
  void *ptr1;
  void *ptr2;
  void *ptr3;
  void *ptr;
  clear_sys_mock();
  restore_malloc_vars();
  uportlibc_malloc(5);
  ptr1 = uportlibc_malloc(200);
  uportlibc_malloc(5);
  ptr2 = uportlibc_malloc(100);
  uportlibc_malloc(5);
  ptr3 = uportlibc_malloc(300);
  uportlibc_malloc(5);
  uportlibc_free(ptr1);
  uportlibc_free(ptr2);
  uportlibc_free(ptr3);
  CU_ASSERT(is_integrated_heap_tree());
  CU_ASSERT_EQUAL(calc_heap_tree_count(), 3);
  CU_ASSERT(is_integrated_list());
  CU_ASSERT_EQUAL(calc_list_count(), 7);
  errno = 0;
  ptr = uportlibc_malloc(200);
  CU_ASSERT_PTR_EQUAL(ptr, ptr1);
  CU_ASSERT_EQUAL(errno, 0);
  CU_ASSERT_EQUAL(sys_mock_op_count, 13);
  CU_ASSERT_EQUAL(ptr_to_header(ptr1)->size, 200);
  CU_ASSERT_PTR_EQUAL(ptr_to_header(ptr2), heap_root);
  CU_ASSERT_PTR_NULL(heap_root->left);
  CU_ASSERT_PTR_EQUAL(ptr_to_header(ptr3), heap_root->right);
  CU_ASSERT_PTR_NULL(heap_root->right->left);
  CU_ASSERT_PTR_NULL(heap_root->right->right);
  CU_ASSERT(is_integrated_heap_tree());
  CU_ASSERT_EQUAL(calc_heap_tree_count(), 2);
  CU_ASSERT(is_integrated_list());
  CU_ASSERT_EQUAL(calc_list_count(), 7);
}

void test_malloc_allocates_part_of_freed_memory_area(void)
{
  void *ptr1;
  void *ptr2;
  void *ptr3;
  void *ptr;
  clear_sys_mock();
  restore_malloc_vars();
  uportlibc_malloc(5);
  ptr1 = uportlibc_malloc(200);
  uportlibc_malloc(5);
  ptr2 = uportlibc_malloc(100);
  uportlibc_malloc(5);
  ptr3 = uportlibc_malloc(300);
  uportlibc_malloc(5);
  uportlibc_free(ptr1);
  uportlibc_free(ptr2);
  uportlibc_free(ptr3);
  CU_ASSERT(is_integrated_heap_tree());
  CU_ASSERT_EQUAL(calc_heap_tree_count(), 3);
  CU_ASSERT(is_integrated_list());
  CU_ASSERT_EQUAL(calc_list_count(), 7);
  errno = 0;
  ptr = uportlibc_malloc(240);
  CU_ASSERT_PTR_EQUAL(ptr, ptr3);
  CU_ASSERT_EQUAL(errno, 0);
  CU_ASSERT_EQUAL(sys_mock_op_count, 14);
  CU_ASSERT_STRING_EQUAL(sys_mock_ops[13].fun_name, "sbrk");
  CU_ASSERT_EQUAL(sys_mock_ops[13].args[0].i, 0);
  CU_ASSERT_PTR_EQUAL(sys_mock_ops[13].res.vp, sys_mock_data_seg + align_up(sizeof(struct malloc_header)) * 7 + 640);
  CU_ASSERT_EQUAL(sys_mock_ops[13].err_num, 0);
  CU_ASSERT_PTR_EQUAL(sys_mock_break, sys_mock_data_seg + align_up(sizeof(struct malloc_header)) * 7 + 640); 
  CU_ASSERT_EQUAL(ptr_to_header(ptr3)->size, 240);
  CU_ASSERT_PTR_EQUAL(ptr_to_header(ptr2), heap_root);
  CU_ASSERT_PTR_EQUAL(((char *) ptr3) + 240, heap_root->left);
  CU_ASSERT_PTR_EQUAL(ptr_to_header(ptr1), heap_root->right);
  CU_ASSERT_PTR_NULL(heap_root->left->left);
  CU_ASSERT_PTR_NULL(heap_root->left->right);
  CU_ASSERT_PTR_NULL(heap_root->right->left);
  CU_ASSERT_PTR_NULL(heap_root->right->right);
  CU_ASSERT_EQUAL(((struct malloc_header *) (((char *) ptr3) + 240))->size, 16);
  CU_ASSERT(is_integrated_heap_tree());
  CU_ASSERT_EQUAL(calc_heap_tree_count(), 3);
  CU_ASSERT(is_integrated_list());
  CU_ASSERT_EQUAL(calc_list_count(), 8);
}

void test_malloc_allocates_part_of_freed_memory_area_that_is_too_large_for_partition(void)
{
  void *ptr1;
  void *ptr2;
  void *ptr3;
  void *ptr;
  clear_sys_mock();
  restore_malloc_vars();
  uportlibc_malloc(5);
  ptr1 = uportlibc_malloc(200);
  uportlibc_malloc(5);
  ptr2 = uportlibc_malloc(100);
  uportlibc_malloc(5);
  ptr3 = uportlibc_malloc(300);
  uportlibc_malloc(5);
  uportlibc_free(ptr1);
  uportlibc_free(ptr2);
  uportlibc_free(ptr3);
  CU_ASSERT(is_integrated_heap_tree());
  CU_ASSERT_EQUAL(calc_heap_tree_count(), 3);
  CU_ASSERT(is_integrated_list());
  CU_ASSERT_EQUAL(calc_list_count(), 7);
  errno = 0;
  ptr = uportlibc_malloc(104 - align_up(sizeof(struct malloc_header)));
  CU_ASSERT_PTR_EQUAL(ptr, ptr2);
  CU_ASSERT_EQUAL(errno, 0);
  CU_ASSERT_EQUAL(sys_mock_op_count, 13);
  CU_ASSERT_EQUAL(ptr_to_header(ptr2)->size, 104);
  CU_ASSERT_PTR_EQUAL(ptr_to_header(ptr1), heap_root);
  CU_ASSERT_PTR_NULL(heap_root->left);
  CU_ASSERT_PTR_EQUAL(ptr_to_header(ptr3), heap_root->right);
  CU_ASSERT_PTR_NULL(heap_root->right->left);
  CU_ASSERT_PTR_NULL(heap_root->right->right);
  CU_ASSERT(is_integrated_heap_tree());
  CU_ASSERT_EQUAL(calc_heap_tree_count(), 2);
  CU_ASSERT(is_integrated_list());
  CU_ASSERT_EQUAL(calc_list_count(), 7);
}

void test_malloc_allocates_memory_area_that_is_too_large_for_freed_memory_areas(void)
{
  void *ptr1;
  void *ptr2;
  void *ptr;
  clear_sys_mock();
  restore_malloc_vars();
  uportlibc_malloc(5);
  ptr1 = uportlibc_malloc(200);
  uportlibc_malloc(5);
  ptr2 = uportlibc_malloc(100);
  uportlibc_malloc(5);
  uportlibc_free(ptr1);
  uportlibc_free(ptr2);
  CU_ASSERT(is_integrated_heap_tree());
  CU_ASSERT_EQUAL(calc_heap_tree_count(), 2);
  CU_ASSERT(is_integrated_list());
  CU_ASSERT_EQUAL(calc_list_count(), 5);
  errno = 0;
  ptr = uportlibc_malloc(400);
  CU_ASSERT_PTR_EQUAL(ptr, sys_mock_data_seg + align_up(sizeof(struct malloc_header)) * 6 + 328);
  CU_ASSERT_EQUAL(errno, 0);
  CU_ASSERT_EQUAL(sys_mock_op_count, 10);
  CU_ASSERT_STRING_EQUAL(sys_mock_ops[9].fun_name, "sbrk");
  CU_ASSERT_EQUAL(sys_mock_ops[9].args[0].i, align_up(sizeof(struct malloc_header)) + 400);
  CU_ASSERT_PTR_EQUAL(sys_mock_ops[9].res.vp, sys_mock_data_seg + align_up(sizeof(struct malloc_header)) * 5 + 328);
  CU_ASSERT_EQUAL(sys_mock_ops[9].err_num, 0);
  CU_ASSERT_PTR_EQUAL(sys_mock_break, sys_mock_data_seg + align_up(sizeof(struct malloc_header)) * 6 + 728);
  CU_ASSERT_EQUAL(ptr_to_header(ptr)->size, 400);
  CU_ASSERT_PTR_EQUAL(ptr_to_header(ptr1), heap_root);
  CU_ASSERT_PTR_EQUAL(ptr_to_header(ptr2), heap_root->left);
  CU_ASSERT_PTR_NULL(heap_root->right);
  CU_ASSERT_PTR_NULL(heap_root->left->left);
  CU_ASSERT_PTR_NULL(heap_root->left->right);
  CU_ASSERT(is_integrated_heap_tree());
  CU_ASSERT_EQUAL(calc_heap_tree_count(), 2);
  CU_ASSERT(is_integrated_list());
  CU_ASSERT_EQUAL(calc_list_count(), 6);
}

void test_malloc_allocates_freed_memory_area_for_random_freed_memory_areas(void)
{
  static size_t sizes[100];
  static void *ptrs[100];
  void *ptr;
  size_t i, area_idx, len = floor((rand() / (RAND_MAX * 1.0)) * 50) + 50;
  clear_sys_mock();
  restore_malloc_vars();
  for(i = 0; i < len; i++) {
    sizes[i] = floor((rand() / (RAND_MAX * 1.0)) * 1000.0) + 1;
  }
  area_idx = floor((rand() / (RAND_MAX * 1.0)) * (len - 1));
  for(i = 0; i < len; i++) {
    if(align_up(sizes[i]) == align_up(sizes[area_idx])) break;
  }
  area_idx = i;
  for(i = 0; i < len; i++) {
    uportlibc_malloc(5);
    ptrs[i] = uportlibc_malloc(sizes[i]);
  }
  uportlibc_malloc(5);
  for(i = 0; i < len; i++) {
    uportlibc_free(ptrs[i]);
  }
  CU_ASSERT(is_integrated_heap_tree());
  CU_ASSERT_EQUAL(calc_heap_tree_count(), len);
  CU_ASSERT(is_integrated_list());
  CU_ASSERT_EQUAL(calc_list_count(), len * 2 + 1);
  errno = 0;
  ptr = uportlibc_malloc(sizes[area_idx]);
  CU_ASSERT_PTR_EQUAL(ptr, ptrs[area_idx]);
  CU_ASSERT_EQUAL(errno, 0);
  CU_ASSERT_EQUAL(sys_mock_op_count, len * 4 + 1);
  CU_ASSERT_EQUAL(ptr_to_header(ptrs[area_idx])->size, align_up(sizes[area_idx]));
  CU_ASSERT(is_integrated_heap_tree());
  CU_ASSERT_EQUAL(calc_heap_tree_count(), len - 1);
  CU_ASSERT(is_integrated_list());
  CU_ASSERT_EQUAL(calc_list_count(), len * 2 + 1);
}

void test_malloc_maps_memory_area(void)
{
  void *ptr;
  int page_size = getpagesize();
  clear_sys_mock();
  restore_malloc_vars();
  errno = 0;
  ptr = uportlibc_malloc(2 * 1024 * 1024);
  CU_ASSERT_EQUAL(((size_t) ptr_to_header(ptr)) % page_size, 0);
  CU_ASSERT_EQUAL(errno, 0);
  CU_ASSERT_EQUAL(sys_mock_op_count, 2);
  CU_ASSERT_STRING_EQUAL(sys_mock_ops[0].fun_name, "getpagesize");
  CU_ASSERT_EQUAL(sys_mock_ops[0].res.i, page_size);
  CU_ASSERT_EQUAL(sys_mock_ops[0].err_num, 0);
  CU_ASSERT_STRING_EQUAL(sys_mock_ops[1].fun_name, "mmap");
  CU_ASSERT_PTR_NULL(sys_mock_ops[1].args[0].vp);
  CU_ASSERT_EQUAL(sys_mock_ops[1].args[1].i, (((2 * 1024 * 1024) + align_up(sizeof(struct malloc_header)) + page_size - 1) / page_size) * page_size);
  CU_ASSERT_EQUAL(sys_mock_ops[1].args[2].i, PROT_READ | PROT_WRITE);
  CU_ASSERT_EQUAL(sys_mock_ops[1].args[3].i, SYS_MOCK_MAP_PRIVATE | SYS_MOCK_MAP_ANON);
  CU_ASSERT_EQUAL(sys_mock_ops[1].args[4].i, -1);
  CU_ASSERT_EQUAL(sys_mock_ops[1].args[5].o, 0);
  CU_ASSERT_EQUAL(((size_t) (sys_mock_ops[1].res.vp)) % page_size, 0);
  CU_ASSERT_EQUAL(sys_mock_ops[1].err_num, 0);
  CU_ASSERT_EQUAL(ptr_to_header(ptr)->size, 2 * 1024 * 1024);
  CU_ASSERT_EQUAL(ptr_to_header(ptr)->flags, MALLOC_FLAG_MMAP);
  CU_ASSERT(is_integrated_heap_tree());
  CU_ASSERT_EQUAL(calc_heap_tree_count(), 0);
  CU_ASSERT(is_integrated_list());
  CU_ASSERT_EQUAL(calc_list_count(), 0);
}

void test_malloc_returns_null_pointer_and_sets_errno_when_error_occurs_for_sbrk(void)
{
  void *ptr;
  clear_sys_mock();
  restore_malloc_vars();
  errno = 0;
  sys_mock_sbrk_res = (void *) (-1);
  sys_mock_sbrk_err_num = ENOMEM;
  ptr = uportlibc_malloc(20);
  CU_ASSERT_PTR_NULL(ptr);
  CU_ASSERT_EQUAL(errno, ENOMEM);
  CU_ASSERT_EQUAL(sys_mock_op_count, 1);
  CU_ASSERT_STRING_EQUAL(sys_mock_ops[0].fun_name, "sbrk");
  CU_ASSERT_EQUAL(sys_mock_ops[0].args[0].i, align_up(sizeof(struct malloc_header)) + 24);
  CU_ASSERT_PTR_EQUAL(sys_mock_ops[0].res.vp, (void *) (-1));
  CU_ASSERT_EQUAL(sys_mock_ops[0].err_num, ENOMEM);
  CU_ASSERT_PTR_EQUAL(sys_mock_break, sys_mock_data_seg);
  CU_ASSERT(is_integrated_heap_tree());
  CU_ASSERT_EQUAL(calc_heap_tree_count(), 0);
  CU_ASSERT(is_integrated_list());
  CU_ASSERT_EQUAL(calc_list_count(), 0);
}

void test_malloc_returns_null_pointer_and_sets_errno_when_error_occurs_for_mmap(void)
{
  void *ptr;
  int page_size = getpagesize();
  clear_sys_mock();
  restore_malloc_vars();
  errno = 0;
  sys_mock_mmap_res = MAP_FAILED;
  sys_mock_mmap_err_num = ENOMEM;
  ptr = uportlibc_malloc(3 * 1024 * 1024);
  CU_ASSERT_PTR_NULL(ptr);
  CU_ASSERT_EQUAL(errno, ENOMEM);
  CU_ASSERT_EQUAL(sys_mock_op_count, 2);
  CU_ASSERT_STRING_EQUAL(sys_mock_ops[0].fun_name, "getpagesize");
  CU_ASSERT_EQUAL(sys_mock_ops[0].res.i, page_size);
  CU_ASSERT_STRING_EQUAL(sys_mock_ops[1].fun_name, "mmap");
  CU_ASSERT_PTR_NULL(sys_mock_ops[1].args[0].vp);
  CU_ASSERT_EQUAL(sys_mock_ops[1].args[1].i, (((3 * 1024 * 1024) + align_up(sizeof(struct malloc_header)) + page_size - 1) / page_size) * page_size);
  CU_ASSERT_EQUAL(sys_mock_ops[1].args[2].i, PROT_READ | PROT_WRITE);
  CU_ASSERT_EQUAL(sys_mock_ops[1].args[3].i, SYS_MOCK_MAP_PRIVATE | SYS_MOCK_MAP_ANON);
  CU_ASSERT_EQUAL(sys_mock_ops[1].args[4].i, -1);
  CU_ASSERT_EQUAL(sys_mock_ops[1].args[5].o, 0);
  CU_ASSERT_PTR_EQUAL(sys_mock_ops[1].res.vp, MAP_FAILED);
  CU_ASSERT_EQUAL(sys_mock_ops[1].err_num, ENOMEM);
  CU_ASSERT(is_integrated_heap_tree());
  CU_ASSERT_EQUAL(calc_heap_tree_count(), 0);
  CU_ASSERT(is_integrated_list());
  CU_ASSERT_EQUAL(calc_list_count(), 0);
}

void test_realloc_reallocates_memory(void)
{
  void *ptr;
  void *ptr2;
  clear_sys_mock();
  restore_malloc_vars();
  ptr = uportlibc_malloc(10);
  errno = 0;
  ptr2 = uportlibc_realloc(ptr, 20);
  CU_ASSERT_EQUAL(ptr2, ptr);
  CU_ASSERT_EQUAL(errno, 0);
  CU_ASSERT_EQUAL(sys_mock_op_count, 3);
  CU_ASSERT_STRING_EQUAL(sys_mock_ops[1].fun_name, "sbrk");
  CU_ASSERT_EQUAL(sys_mock_ops[1].args[0].i, 0);
  CU_ASSERT_PTR_EQUAL(sys_mock_ops[1].res.vp, sys_mock_data_seg + align_up(sizeof(struct malloc_header)) * 1 + 16);
  CU_ASSERT_EQUAL(sys_mock_ops[1].err_num, 0);
  CU_ASSERT_STRING_EQUAL(sys_mock_ops[2].fun_name, "sbrk");
  CU_ASSERT_EQUAL(sys_mock_ops[2].args[0].i, 8);
  CU_ASSERT_PTR_EQUAL(sys_mock_ops[2].res.vp, sys_mock_data_seg + align_up(sizeof(struct malloc_header)) * 1 + 16);
  CU_ASSERT_EQUAL(sys_mock_ops[2].err_num, 0);
  CU_ASSERT_PTR_EQUAL(sys_mock_break, sys_mock_data_seg + align_up(sizeof(struct malloc_header)) * 1 + 24);
  CU_ASSERT(is_integrated_heap_tree());
  CU_ASSERT_EQUAL(calc_heap_tree_count(), 0);
  CU_ASSERT(is_integrated_list());
  CU_ASSERT_EQUAL(calc_list_count(), 1);
}

void test_realloc_reallocates_three_memory_areas(void)
{
  void *ptr1;
  void *ptr2;
  void *ptr3;
  void *ptr4;
  void *ptr5;
  void *ptr6;
  clear_sys_mock();
  restore_malloc_vars();
  ptr1 = uportlibc_malloc(100);
  uportlibc_malloc(5);
  ptr2 = uportlibc_malloc(200);
  uportlibc_malloc(5);
  ptr3 = uportlibc_malloc(300);
  uportlibc_free(ptr1);
  CU_ASSERT_PTR_EQUAL(uportlibc_malloc(20), ptr1);
  uportlibc_free(ptr2);
  CU_ASSERT_PTR_EQUAL(uportlibc_malloc(120), ptr2);
  errno =  0;
  ptr4 = uportlibc_realloc(ptr1, 40);
  CU_ASSERT_PTR_EQUAL(ptr4, ptr1);
  CU_ASSERT_EQUAL(errno, 0);
  CU_ASSERT_EQUAL(sys_mock_op_count, 13);
  CU_ASSERT_STRING_EQUAL(sys_mock_ops[11].fun_name, "sbrk");
  CU_ASSERT_EQUAL(sys_mock_ops[11].args[0].i, 0);
  CU_ASSERT_PTR_EQUAL(sys_mock_ops[11].res.vp, sys_mock_data_seg + align_up(sizeof(struct malloc_header)) * 5 + 624);
  CU_ASSERT_EQUAL(sys_mock_ops[11].err_num, 0);
  CU_ASSERT_STRING_EQUAL(sys_mock_ops[12].fun_name, "sbrk");
  CU_ASSERT_EQUAL(sys_mock_ops[12].args[0].i, 0);
  CU_ASSERT_PTR_EQUAL(sys_mock_ops[12].res.vp, sys_mock_data_seg + align_up(sizeof(struct malloc_header)) * 5 + 624);
  CU_ASSERT_EQUAL(sys_mock_ops[12].err_num, 0);
  CU_ASSERT_PTR_EQUAL(sys_mock_break, sys_mock_data_seg + align_up(sizeof(struct malloc_header)) * 5 + 624);
  CU_ASSERT_EQUAL(ptr_to_header(ptr1)->size, 40);
  CU_ASSERT_EQUAL(((struct malloc_header *) (((char *) ptr1) + 40))->size, 64 - align_up(sizeof(struct malloc_header)));
  CU_ASSERT(is_integrated_heap_tree());
  CU_ASSERT_EQUAL(calc_heap_tree_count(), 2);
  CU_ASSERT(is_integrated_list());
  CU_ASSERT_EQUAL(calc_list_count(), 7);
  errno =  0;
  ptr5 = uportlibc_realloc(ptr2, 200);
  CU_ASSERT_PTR_EQUAL(ptr5, ptr2);
  CU_ASSERT_EQUAL(errno, 0);
  CU_ASSERT_EQUAL(sys_mock_op_count, 15);
  CU_ASSERT_STRING_EQUAL(sys_mock_ops[13].fun_name, "sbrk");
  CU_ASSERT_EQUAL(sys_mock_ops[13].args[0].i, 0);
  CU_ASSERT_PTR_EQUAL(sys_mock_ops[13].res.vp, sys_mock_data_seg + align_up(sizeof(struct malloc_header)) * 5 + 624);
  CU_ASSERT_EQUAL(sys_mock_ops[13].err_num, 0);
  CU_ASSERT_STRING_EQUAL(sys_mock_ops[14].fun_name, "sbrk");
  CU_ASSERT_EQUAL(sys_mock_ops[14].args[0].i, 0);
  CU_ASSERT_PTR_EQUAL(sys_mock_ops[14].res.vp, sys_mock_data_seg + align_up(sizeof(struct malloc_header)) * 5 + 624);
  CU_ASSERT_EQUAL(sys_mock_ops[14].err_num, 0);
  CU_ASSERT_PTR_EQUAL(sys_mock_break, sys_mock_data_seg + align_up(sizeof(struct malloc_header)) * 5 + 624);
  CU_ASSERT_EQUAL(ptr_to_header(ptr2)->size, 200);
  CU_ASSERT(is_integrated_heap_tree());
  CU_ASSERT_EQUAL(calc_heap_tree_count(), 1);
  CU_ASSERT(is_integrated_list());
  CU_ASSERT_EQUAL(calc_list_count(), 6);
  errno =  0;
  ptr6 = uportlibc_realloc(ptr3, 400);
  CU_ASSERT_PTR_EQUAL(ptr6, ptr3);
  CU_ASSERT_EQUAL(errno, 0);
  CU_ASSERT_EQUAL(sys_mock_op_count, 17);
  CU_ASSERT_STRING_EQUAL(sys_mock_ops[15].fun_name, "sbrk");
  CU_ASSERT_EQUAL(sys_mock_ops[15].args[0].i, 0);
  CU_ASSERT_PTR_EQUAL(sys_mock_ops[15].res.vp, sys_mock_data_seg + align_up(sizeof(struct malloc_header)) * 5 + 624);
  CU_ASSERT_EQUAL(sys_mock_ops[15].err_num, 0);
  CU_ASSERT_STRING_EQUAL(sys_mock_ops[16].fun_name, "sbrk");
  CU_ASSERT_EQUAL(sys_mock_ops[16].args[0].i, 96);
  CU_ASSERT_PTR_EQUAL(sys_mock_ops[16].res.vp, sys_mock_data_seg + align_up(sizeof(struct malloc_header)) * 5 + 624);
  CU_ASSERT_EQUAL(sys_mock_ops[16].err_num, 0);
  CU_ASSERT_PTR_EQUAL(sys_mock_break, sys_mock_data_seg + align_up(sizeof(struct malloc_header)) * 5 + 720);
  CU_ASSERT(is_integrated_heap_tree());
  CU_ASSERT_EQUAL(calc_heap_tree_count(), 1);
  CU_ASSERT(is_integrated_list());
  CU_ASSERT_EQUAL(calc_list_count(), 6);
}

void test_realloc_allotates_memory_for_pointer_that_is_null_pointer(void)
{
  void *ptr;
  clear_sys_mock();
  restore_malloc_vars();
  errno = 0;
  ptr = uportlibc_realloc(NULL, 20);
  CU_ASSERT_PTR_EQUAL(ptr, sys_mock_data_seg + align_up(sizeof(struct malloc_header)));
  CU_ASSERT_EQUAL(errno, 0);
  CU_ASSERT_EQUAL(sys_mock_op_count, 1);
  CU_ASSERT_STRING_EQUAL(sys_mock_ops[0].fun_name, "sbrk");
  CU_ASSERT_EQUAL(sys_mock_ops[0].args[0].i, align_up(sizeof(struct malloc_header)) + 24);
  CU_ASSERT_PTR_EQUAL(sys_mock_ops[0].res.vp, sys_mock_data_seg);
  CU_ASSERT_EQUAL(sys_mock_ops[0].err_num, 0);
  CU_ASSERT_PTR_EQUAL(sys_mock_break, sys_mock_data_seg + align_up(sizeof(struct malloc_header)) * 1 + 24);
  CU_ASSERT_EQUAL(ptr_to_header(ptr)->size, 24);
  CU_ASSERT(is_integrated_heap_tree());
  CU_ASSERT_EQUAL(calc_heap_tree_count(), 0);
  CU_ASSERT(is_integrated_list());
  CU_ASSERT_EQUAL(calc_list_count(), 1);
}

void test_realloc_returns_null_pointer_for_pointer_that_is_null_pointer_and_size_that_is_zero(void)
{
  void *ptr;
  clear_sys_mock();
  restore_malloc_vars();
  errno = 0;
  ptr = uportlibc_realloc(NULL, 0);
  CU_ASSERT_PTR_NULL(ptr);
  CU_ASSERT_EQUAL(errno, 0);
  CU_ASSERT_EQUAL(sys_mock_op_count, 0);
  CU_ASSERT_PTR_EQUAL(sys_mock_break, sys_mock_data_seg);
  CU_ASSERT(is_integrated_heap_tree());
  CU_ASSERT_EQUAL(calc_heap_tree_count(), 0);
  CU_ASSERT(is_integrated_list());
  CU_ASSERT_EQUAL(calc_list_count(), 0);
}

void test_realloc_frees_memory_for_size_that_is_zero(void)
{
  void *ptr;
  void *ptr2;
  clear_sys_mock();
  restore_malloc_vars();
  ptr = uportlibc_malloc(30);
  errno = 0;
  ptr2 = uportlibc_realloc(ptr, 0);
  CU_ASSERT_PTR_NULL(ptr2);
  CU_ASSERT_EQUAL(errno, 0);
  CU_ASSERT_EQUAL(sys_mock_op_count, 3);
  CU_ASSERT_STRING_EQUAL(sys_mock_ops[1].fun_name, "sbrk");
  CU_ASSERT_EQUAL(sys_mock_ops[1].args[0].i, 0);
  CU_ASSERT_PTR_EQUAL(sys_mock_ops[1].res.vp, sys_mock_data_seg + align_up(sizeof(struct malloc_header)) + 32);
  CU_ASSERT_EQUAL(sys_mock_ops[1].err_num, 0);
  CU_ASSERT_STRING_EQUAL(sys_mock_ops[2].fun_name, "sbrk");
  CU_ASSERT_EQUAL(sys_mock_ops[2].args[0].i, -(((ssize_t) align_up(sizeof(struct malloc_header))) + 32));
  CU_ASSERT_PTR_EQUAL(sys_mock_ops[2].res.vp, sys_mock_data_seg + align_up(sizeof(struct malloc_header)) + 32);
  CU_ASSERT_EQUAL(sys_mock_ops[2].err_num, 0);
  CU_ASSERT_PTR_EQUAL(sys_mock_break, sys_mock_data_seg);
  CU_ASSERT(is_integrated_heap_tree());
  CU_ASSERT_EQUAL(calc_heap_tree_count(), 0);
  CU_ASSERT(is_integrated_list());
  CU_ASSERT_EQUAL(calc_list_count(), 0);
}

void test_realloc_increases_memory_area_for_freed_memory_area(void)
{
  void *ptr;
  void *ptr2;
  clear_sys_mock();
  restore_malloc_vars();
  uportlibc_malloc(5);
  ptr = uportlibc_malloc(200);
  uportlibc_malloc(5);
  uportlibc_free(ptr);
  CU_ASSERT_PTR_EQUAL(uportlibc_malloc(100), ptr);
  errno = 0;
  ptr2 = uportlibc_realloc(ptr, 140);
  CU_ASSERT_PTR_EQUAL(ptr2, ptr);
  CU_ASSERT_EQUAL(errno, 0);
  CU_ASSERT_EQUAL(sys_mock_op_count, 8);
  CU_ASSERT_STRING_EQUAL(sys_mock_ops[6].fun_name, "sbrk");
  CU_ASSERT_EQUAL(sys_mock_ops[6].args[0].i, 0);
  CU_ASSERT_PTR_EQUAL(sys_mock_ops[6].res.vp, sys_mock_data_seg + align_up(sizeof(struct malloc_header)) * 3 + 216);
  CU_ASSERT_EQUAL(sys_mock_ops[6].err_num, 0);
  CU_ASSERT_STRING_EQUAL(sys_mock_ops[7].fun_name, "sbrk");
  CU_ASSERT_EQUAL(sys_mock_ops[7].args[0].i, 0);
  CU_ASSERT_PTR_EQUAL(sys_mock_ops[7].res.vp, sys_mock_data_seg + align_up(sizeof(struct malloc_header)) * 3 + 216);
  CU_ASSERT_EQUAL(sys_mock_ops[7].err_num, 0);
  CU_ASSERT_PTR_EQUAL(sys_mock_break, sys_mock_data_seg + align_up(sizeof(struct malloc_header)) * 3 + 216);
  CU_ASSERT_EQUAL(ptr_to_header(ptr)->size, 144);
  CU_ASSERT_EQUAL(((struct malloc_header *) (((char *) ptr) + 144))->size, 56 - align_up(sizeof(struct malloc_header)));
  CU_ASSERT(is_integrated_heap_tree());
  CU_ASSERT_EQUAL(calc_heap_tree_count(), 1);
  CU_ASSERT(is_integrated_list());
  CU_ASSERT_EQUAL(calc_list_count(), 4);
}

void test_realloc_increases_memory_area_to_maximum_for_freed_memory_area(void)
{
  void *ptr;
  void *ptr2;
  clear_sys_mock();
  restore_malloc_vars();
  uportlibc_malloc(5);
  ptr = uportlibc_malloc(200);
  uportlibc_malloc(5);
  uportlibc_free(ptr);
  CU_ASSERT_PTR_EQUAL(uportlibc_malloc(100), ptr);
  errno = 0;
  ptr2 = uportlibc_realloc(ptr, 200);
  CU_ASSERT_PTR_EQUAL(ptr2, ptr);
  CU_ASSERT_EQUAL(errno, 0);
  CU_ASSERT_EQUAL(sys_mock_op_count, 8);
  CU_ASSERT_STRING_EQUAL(sys_mock_ops[6].fun_name, "sbrk");
  CU_ASSERT_EQUAL(sys_mock_ops[6].args[0].i, 0);
  CU_ASSERT_EQUAL(sys_mock_ops[6].err_num, 0);
  CU_ASSERT_PTR_EQUAL(sys_mock_ops[6].res.vp, sys_mock_data_seg + align_up(sizeof(struct malloc_header)) * 3 + 216);
  CU_ASSERT_STRING_EQUAL(sys_mock_ops[7].fun_name, "sbrk");
  CU_ASSERT_EQUAL(sys_mock_ops[7].args[0].i, 0);
  CU_ASSERT_PTR_EQUAL(sys_mock_ops[7].res.i, sys_mock_data_seg + align_up(sizeof(struct malloc_header)) * 3 + 216);
  CU_ASSERT_EQUAL(sys_mock_ops[7].err_num, 0);
  CU_ASSERT_PTR_EQUAL(sys_mock_break, sys_mock_data_seg + align_up(sizeof(struct malloc_header)) * 3 + 216);
  CU_ASSERT_EQUAL(ptr_to_header(ptr)->size, 200);
  CU_ASSERT(is_integrated_heap_tree());
  CU_ASSERT_EQUAL(calc_heap_tree_count(), 0);
  CU_ASSERT(is_integrated_list());
  CU_ASSERT_EQUAL(calc_list_count(), 3);
}

void test_realloc_increases_memory_area_that_will_be_too_large_for_partition_and_freed_memory_area(void)
{
  void *ptr;
  void *ptr2;
  clear_sys_mock();
  restore_malloc_vars();
  uportlibc_malloc(5);
  ptr = uportlibc_malloc(300);
  uportlibc_malloc(5);
  uportlibc_free(ptr);
  CU_ASSERT_PTR_EQUAL(uportlibc_malloc(100), ptr);
  errno = 0;
  ptr2 = uportlibc_realloc(ptr, 304 - align_up(sizeof(struct malloc_header)));
  CU_ASSERT_PTR_EQUAL(ptr2, ptr);
  CU_ASSERT_EQUAL(errno, 0);
  CU_ASSERT_EQUAL(sys_mock_op_count, 8);
  CU_ASSERT_STRING_EQUAL(sys_mock_ops[6].fun_name, "sbrk");
  CU_ASSERT_EQUAL(sys_mock_ops[6].args[0].i, 0);
  CU_ASSERT_PTR_EQUAL(sys_mock_ops[6].res.vp, sys_mock_data_seg + align_up(sizeof(struct malloc_header)) * 3 + 320);
  CU_ASSERT_EQUAL(sys_mock_ops[6].err_num, 0);
  CU_ASSERT_STRING_EQUAL(sys_mock_ops[7].fun_name, "sbrk");
  CU_ASSERT_EQUAL(sys_mock_ops[7].args[0].i, 0);
  CU_ASSERT_PTR_EQUAL(sys_mock_ops[7].res.i, sys_mock_data_seg + align_up(sizeof(struct malloc_header)) * 3 + 320);
  CU_ASSERT_EQUAL(sys_mock_ops[7].err_num, 0);
  CU_ASSERT_PTR_EQUAL(sys_mock_break, sys_mock_data_seg + align_up(sizeof(struct malloc_header)) * 3 + 320);
  CU_ASSERT_EQUAL(ptr_to_header(ptr)->size, 304);
  CU_ASSERT(is_integrated_heap_tree());
  CU_ASSERT_EQUAL(calc_heap_tree_count(), 0);
  CU_ASSERT(is_integrated_list());
  CU_ASSERT_EQUAL(calc_list_count(), 3);
}

void test_realloc_moves_memory_area_for_freed_memory_area(void)
{
  void *ptr;
  void *ptr2;
  clear_sys_mock();
  restore_malloc_vars();
  uportlibc_malloc(5);
  ptr = uportlibc_malloc(80);
  uportlibc_malloc(5);
  uportlibc_free(ptr);
  CU_ASSERT_PTR_EQUAL(uportlibc_malloc(16), ptr);
  memcpy(ptr, "1234567890123456", 16);
  errno = 0;
  ptr2 = uportlibc_realloc(ptr, 128);
  CU_ASSERT_PTR_EQUAL(ptr2, sys_mock_data_seg + align_up(sizeof(struct malloc_header)) * 4 + 96);
  CU_ASSERT_EQUAL(errno, 0);
  CU_ASSERT_EQUAL(sys_mock_op_count, 10);
  CU_ASSERT_STRING_EQUAL(sys_mock_ops[6].fun_name, "sbrk");
  CU_ASSERT_EQUAL(sys_mock_ops[6].args[0].i, 0);
  CU_ASSERT_PTR_EQUAL(sys_mock_ops[6].res.vp, sys_mock_data_seg + align_up(sizeof(struct malloc_header)) * 3 + 96);
  CU_ASSERT_EQUAL(sys_mock_ops[6].err_num, 0);
  CU_ASSERT_STRING_EQUAL(sys_mock_ops[7].fun_name, "sbrk");
  CU_ASSERT_EQUAL(sys_mock_ops[7].args[0].i, align_up(sizeof(struct malloc_header)) + 128);
  CU_ASSERT_PTR_EQUAL(sys_mock_ops[7].res.vp, sys_mock_data_seg + align_up(sizeof(struct malloc_header)) * 3 + 96);
  CU_ASSERT_EQUAL(sys_mock_ops[7].err_num, 0);
  CU_ASSERT_STRING_EQUAL(sys_mock_ops[8].fun_name, "sbrk");
  CU_ASSERT_EQUAL(sys_mock_ops[8].args[0].i, 0);
  CU_ASSERT_PTR_EQUAL(sys_mock_ops[8].res.i, sys_mock_data_seg + align_up(sizeof(struct malloc_header)) * 4 + 224);
  CU_ASSERT_EQUAL(sys_mock_ops[8].err_num, 0);
  CU_ASSERT_STRING_EQUAL(sys_mock_ops[9].fun_name, "sbrk");
  CU_ASSERT_EQUAL(sys_mock_ops[9].args[0].i, 0);
  CU_ASSERT_PTR_EQUAL(sys_mock_ops[9].res.i, sys_mock_data_seg + align_up(sizeof(struct malloc_header)) * 4 + 224);
  CU_ASSERT_EQUAL(sys_mock_ops[9].err_num, 0);
  CU_ASSERT_PTR_EQUAL(sys_mock_break, sys_mock_data_seg + align_up(sizeof(struct malloc_header)) * 4 + 224);
  CU_ASSERT_EQUAL(ptr_to_header(ptr2)->size, 128);
  CU_ASSERT_EQUAL(ptr_to_header(ptr)->size, 80);
  CU_ASSERT_NSTRING_EQUAL(((char *) ptr2), "1234567890123456", 16);
  CU_ASSERT(is_integrated_heap_tree());
  CU_ASSERT_EQUAL(calc_heap_tree_count(), 1);
  CU_ASSERT(is_integrated_list());
  CU_ASSERT_EQUAL(calc_list_count(), 4);
}

void test_realloc_moves_memory_area(void)
{
  void *ptr;
  void *ptr2;
  clear_sys_mock();
  restore_malloc_vars();
  uportlibc_malloc(5);
  ptr = uportlibc_malloc(24);
  uportlibc_malloc(5);
  memcpy(ptr, "abcdefghABCDEFGH1234568", 24);
  errno = 0;
  ptr2 = uportlibc_realloc(ptr, 32);
  CU_ASSERT_PTR_EQUAL(ptr2, sys_mock_data_seg + align_up(sizeof(struct malloc_header)) * 4 + 40);
  CU_ASSERT_EQUAL(errno, 0);
  CU_ASSERT_EQUAL(sys_mock_op_count, 7);
  CU_ASSERT_STRING_EQUAL(sys_mock_ops[3].fun_name, "sbrk");
  CU_ASSERT_EQUAL(sys_mock_ops[3].args[0].i, 0);
  CU_ASSERT_PTR_EQUAL(sys_mock_ops[3].res.vp, sys_mock_data_seg + align_up(sizeof(struct malloc_header)) * 3 + 40);
  CU_ASSERT_EQUAL(sys_mock_ops[3].err_num, 0);
  CU_ASSERT_STRING_EQUAL(sys_mock_ops[4].fun_name, "sbrk");
  CU_ASSERT_EQUAL(sys_mock_ops[4].args[0].i, align_up(sizeof(struct malloc_header)) + 32);
  CU_ASSERT_PTR_EQUAL(sys_mock_ops[4].res.vp, sys_mock_data_seg + align_up(sizeof(struct malloc_header)) * 3 + 40);
  CU_ASSERT_EQUAL(sys_mock_ops[4].err_num, 0);
  CU_ASSERT_STRING_EQUAL(sys_mock_ops[5].fun_name, "sbrk");
  CU_ASSERT_EQUAL(sys_mock_ops[5].args[0].i, 0);
  CU_ASSERT_PTR_EQUAL(sys_mock_ops[5].res.i, sys_mock_data_seg + align_up(sizeof(struct malloc_header)) * 4 + 72);
  CU_ASSERT_EQUAL(sys_mock_ops[5].err_num, 0);
  CU_ASSERT_STRING_EQUAL(sys_mock_ops[6].fun_name, "sbrk");
  CU_ASSERT_EQUAL(sys_mock_ops[6].args[0].i, 0);
  CU_ASSERT_PTR_EQUAL(sys_mock_ops[6].res.i, sys_mock_data_seg + align_up(sizeof(struct malloc_header)) * 4 + 72);
  CU_ASSERT_EQUAL(sys_mock_ops[6].err_num, 0);
  CU_ASSERT_PTR_EQUAL(sys_mock_break, sys_mock_data_seg + align_up(sizeof(struct malloc_header)) * 4 + 72);
  CU_ASSERT_EQUAL(ptr_to_header(ptr2)->size, 32);
  CU_ASSERT_EQUAL(ptr_to_header(ptr)->size, 24);
  CU_ASSERT_NSTRING_EQUAL(((char *) ptr2), "abcdefghABCDEFGH1234568", 24);
  CU_ASSERT(is_integrated_heap_tree());
  CU_ASSERT_EQUAL(calc_heap_tree_count(), 1);
  CU_ASSERT(is_integrated_list());
  CU_ASSERT_EQUAL(calc_list_count(), 4);
}

void test_realloc_increases_memory_area_for_sbrk(void)
{
  void *ptr;
  void *ptr2;
  clear_sys_mock();
  restore_malloc_vars();
  ptr = uportlibc_malloc(100);
  errno = 0;
  ptr2 = uportlibc_realloc(ptr, 200);
  CU_ASSERT_PTR_EQUAL(ptr2, ptr);
  CU_ASSERT_EQUAL(errno, 0);
  CU_ASSERT_EQUAL(sys_mock_op_count, 3);
  CU_ASSERT_STRING_EQUAL(sys_mock_ops[1].fun_name, "sbrk");
  CU_ASSERT_EQUAL(sys_mock_ops[1].args[0].i, 0);
  CU_ASSERT_PTR_EQUAL(sys_mock_ops[1].res.vp, sys_mock_data_seg + align_up(sizeof(struct malloc_header)) * 1 + 104);
  CU_ASSERT_EQUAL(sys_mock_ops[1].err_num, 0);
  CU_ASSERT_STRING_EQUAL(sys_mock_ops[2].fun_name, "sbrk");
  CU_ASSERT_EQUAL(sys_mock_ops[2].args[0].i, 96);
  CU_ASSERT_PTR_EQUAL(sys_mock_ops[2].res.vp, sys_mock_data_seg + align_up(sizeof(struct malloc_header)) * 1 + 104);
  CU_ASSERT_EQUAL(sys_mock_ops[2].err_num, 0);
  CU_ASSERT_PTR_EQUAL(sys_mock_break, sys_mock_data_seg + align_up(sizeof(struct malloc_header)) * 1 + 200);
  CU_ASSERT_EQUAL(ptr_to_header(ptr)->size, 200);
  CU_ASSERT(is_integrated_heap_tree());
  CU_ASSERT_EQUAL(calc_heap_tree_count(), 0);
  CU_ASSERT(is_integrated_list());
  CU_ASSERT_EQUAL(calc_list_count(), 1);
}

void test_realloc_returns_pointer_for_old_size_that_is_eqaul_to_size(void)
{
  void *ptr;
  void *ptr2;
  clear_sys_mock();
  restore_malloc_vars();
  ptr = uportlibc_malloc(100);
  errno = 0;
  ptr2 = uportlibc_realloc(ptr, 100);
  CU_ASSERT_PTR_EQUAL(ptr2, ptr);
  CU_ASSERT_EQUAL(errno, 0);
  CU_ASSERT_EQUAL(sys_mock_op_count, 1);
  CU_ASSERT(is_integrated_heap_tree());
  CU_ASSERT_EQUAL(calc_heap_tree_count(), 0);
  CU_ASSERT(is_integrated_list());
  CU_ASSERT_EQUAL(calc_list_count(), 1);
}

void test_realloc_decreases_memory_area_for_freed_memory_area(void)
{
  void *ptr;
  void *ptr2;
  clear_sys_mock();
  restore_malloc_vars();
  uportlibc_malloc(5);
  ptr = uportlibc_malloc(200);
  uportlibc_malloc(5);
  uportlibc_free(ptr);
  CU_ASSERT_PTR_EQUAL(uportlibc_malloc(100), ptr);
  errno = 0;
  ptr2 = uportlibc_realloc(ptr, 50);
  CU_ASSERT_PTR_EQUAL(ptr2, ptr);
  CU_ASSERT_EQUAL(errno, 0);
  CU_ASSERT_EQUAL(sys_mock_op_count, 8);
  CU_ASSERT_STRING_EQUAL(sys_mock_ops[6].fun_name, "sbrk");
  CU_ASSERT_EQUAL(sys_mock_ops[6].args[0].i, 0);
  CU_ASSERT_PTR_EQUAL(sys_mock_ops[6].res.vp, sys_mock_data_seg + align_up(sizeof(struct malloc_header)) * 3 + 216);
  CU_ASSERT_EQUAL(sys_mock_ops[6].err_num, 0);
  CU_ASSERT_STRING_EQUAL(sys_mock_ops[7].fun_name, "sbrk");
  CU_ASSERT_EQUAL(sys_mock_ops[7].args[0].i, 0);
  CU_ASSERT_PTR_EQUAL(sys_mock_ops[7].res.vp, sys_mock_data_seg + align_up(sizeof(struct malloc_header)) * 3 + 216);
  CU_ASSERT_EQUAL(sys_mock_ops[7].err_num, 0);
  CU_ASSERT_PTR_EQUAL(sys_mock_break, sys_mock_data_seg + align_up(sizeof(struct malloc_header)) * 3 + 216);
  CU_ASSERT_EQUAL(ptr_to_header(ptr)->size, 56);
  CU_ASSERT_EQUAL(((struct malloc_header *) (((char *) ptr) + 56))->size, 144 - align_up(sizeof(struct malloc_header)));
  CU_ASSERT(is_integrated_heap_tree());
  CU_ASSERT_EQUAL(calc_heap_tree_count(), 1);
  CU_ASSERT(is_integrated_list());
  CU_ASSERT_EQUAL(calc_list_count(), 4);
}

void test_realloc_decreases_memory_area(void)
{
  void *ptr;
  void *ptr2;
  clear_sys_mock();
  restore_malloc_vars();
  uportlibc_malloc(5);
  ptr = uportlibc_malloc(300);
  uportlibc_malloc(5);
  errno = 0;
  ptr2 = uportlibc_realloc(ptr, 100);
  CU_ASSERT_PTR_EQUAL(ptr2, ptr);
  CU_ASSERT_EQUAL(errno, 0);
  CU_ASSERT_EQUAL(sys_mock_op_count, 5);
  CU_ASSERT_STRING_EQUAL(sys_mock_ops[3].fun_name, "sbrk");
  CU_ASSERT_EQUAL(sys_mock_ops[3].args[0].i, 0);
  CU_ASSERT_PTR_EQUAL(sys_mock_ops[3].res.vp, sys_mock_data_seg + align_up(sizeof(struct malloc_header)) * 3 + 320);
  CU_ASSERT_EQUAL(sys_mock_ops[3].err_num, 0);
  CU_ASSERT_STRING_EQUAL(sys_mock_ops[4].fun_name, "sbrk");
  CU_ASSERT_EQUAL(sys_mock_ops[4].args[0].i, 0);
  CU_ASSERT_PTR_EQUAL(sys_mock_ops[4].res.vp, sys_mock_data_seg + align_up(sizeof(struct malloc_header)) * 3 + 320);
  CU_ASSERT_EQUAL(sys_mock_ops[4].err_num, 0);
  CU_ASSERT_PTR_EQUAL(sys_mock_break, sys_mock_data_seg + align_up(sizeof(struct malloc_header)) * 3 + 320);
  CU_ASSERT_EQUAL(ptr_to_header(ptr)->size, 104);
  CU_ASSERT_EQUAL(((struct malloc_header *) (((char *) ptr) + 104))->size, 200 - align_up(sizeof(struct malloc_header)));
  CU_ASSERT(is_integrated_heap_tree());
  CU_ASSERT_EQUAL(calc_heap_tree_count(), 1);
  CU_ASSERT(is_integrated_list());
  CU_ASSERT_EQUAL(calc_list_count(), 4);
}

void test_realloc_decreases_memory_area_that_will_be_too_large_for_partition(void)
{
  void *ptr;
  void *ptr2;
  clear_sys_mock();
  restore_malloc_vars();
  uportlibc_malloc(5);
  ptr = uportlibc_malloc(200);
  uportlibc_malloc(5);
  errno = 0;
  ptr2 = uportlibc_realloc(ptr, 200 - align_up(sizeof(struct malloc_header)));
  CU_ASSERT_PTR_EQUAL(ptr2, ptr);
  CU_ASSERT_EQUAL(errno, 0);
  CU_ASSERT_EQUAL(sys_mock_op_count, 4);
  CU_ASSERT_STRING_EQUAL(sys_mock_ops[3].fun_name, "sbrk");
  CU_ASSERT_EQUAL(sys_mock_ops[3].args[0].i, 0);
  CU_ASSERT_PTR_EQUAL(sys_mock_ops[3].res.vp, sys_mock_data_seg + align_up(sizeof(struct malloc_header)) * 3 + 216);
  CU_ASSERT_EQUAL(sys_mock_ops[3].err_num, 0);
  CU_ASSERT_PTR_EQUAL(sys_mock_break, sys_mock_data_seg + align_up(sizeof(struct malloc_header)) * 3 + 216);
  CU_ASSERT_EQUAL(ptr_to_header(ptr)->size, 200);
  CU_ASSERT(is_integrated_heap_tree());
  CU_ASSERT_EQUAL(calc_heap_tree_count(), 0);
  CU_ASSERT(is_integrated_list());
  CU_ASSERT_EQUAL(calc_list_count(), 3);
}

void test_realloc_decreases_memory_area_for_sbrk(void)
{
  void *ptr;
  void *ptr2;
  clear_sys_mock();
  restore_malloc_vars();
  ptr = uportlibc_malloc(200);
  errno = 0;
  ptr2 = uportlibc_realloc(ptr, 100);
  CU_ASSERT_PTR_EQUAL(ptr2, ptr);
  CU_ASSERT_EQUAL(errno, 0);
  CU_ASSERT_EQUAL(sys_mock_op_count, 3);
  CU_ASSERT_STRING_EQUAL(sys_mock_ops[1].fun_name, "sbrk");
  CU_ASSERT_EQUAL(sys_mock_ops[1].args[0].i, 0);
  CU_ASSERT_PTR_EQUAL(sys_mock_ops[1].res.vp, sys_mock_data_seg + align_up(sizeof(struct malloc_header)) * 1 + 200);
  CU_ASSERT_EQUAL(sys_mock_ops[1].err_num, 0);
  CU_ASSERT_STRING_EQUAL(sys_mock_ops[2].fun_name, "sbrk");
  CU_ASSERT_EQUAL(sys_mock_ops[2].args[0].i, -96);
  CU_ASSERT_PTR_EQUAL(sys_mock_ops[2].res.vp, sys_mock_data_seg + align_up(sizeof(struct malloc_header)) * 1 + 200);
  CU_ASSERT_EQUAL(sys_mock_ops[2].err_num, 0);
  CU_ASSERT_PTR_EQUAL(sys_mock_break, sys_mock_data_seg + align_up(sizeof(struct malloc_header)) * 1 + 104);
  CU_ASSERT_EQUAL(ptr_to_header(ptr)->size, 104);
  CU_ASSERT(is_integrated_heap_tree());
  CU_ASSERT_EQUAL(calc_heap_tree_count(), 0);
  CU_ASSERT(is_integrated_list());
  CU_ASSERT_EQUAL(calc_list_count(), 1);
}

void test_realloc_maps_memory_area_for_increasing_memory_area(void)
{
  void *ptr;
  void *ptr2;
  int page_size = getpagesize();
  clear_sys_mock();
  restore_malloc_vars();
  ptr = uportlibc_malloc(2 * 1024 * 1024);
  memset(ptr, 'a', 2 * 1024 * 1024);
  errno = 0;
  ptr2 = uportlibc_realloc(ptr, 3 * 1024 * 1024);
  CU_ASSERT_EQUAL(((size_t) ptr_to_header(ptr2)) % page_size, 0);
  CU_ASSERT_EQUAL(errno, 0);
  CU_ASSERT_EQUAL(sys_mock_op_count, 7);
  CU_ASSERT_STRING_EQUAL(sys_mock_ops[2].fun_name, "getpagesize");
  CU_ASSERT_EQUAL(sys_mock_ops[2].res.i, page_size);
  CU_ASSERT_EQUAL(sys_mock_ops[2].err_num, 0);
  CU_ASSERT_STRING_EQUAL(sys_mock_ops[3].fun_name, "getpagesize");
  CU_ASSERT_EQUAL(sys_mock_ops[3].res.i, page_size);
  CU_ASSERT_EQUAL(sys_mock_ops[3].err_num, 0);
  CU_ASSERT_STRING_EQUAL(sys_mock_ops[4].fun_name, "mmap");
  CU_ASSERT_PTR_NULL(sys_mock_ops[4].args[0].vp);
  CU_ASSERT_EQUAL(sys_mock_ops[4].args[1].i, (((3 * 1024 * 1024) + align_up(sizeof(struct malloc_header)) + page_size - 1) / page_size) * page_size);
  CU_ASSERT_EQUAL(sys_mock_ops[4].args[2].i, PROT_READ | PROT_WRITE);
  CU_ASSERT_EQUAL(sys_mock_ops[4].args[3].i, SYS_MOCK_MAP_PRIVATE | SYS_MOCK_MAP_ANON);
  CU_ASSERT_EQUAL(sys_mock_ops[4].args[4].i, -1);
  CU_ASSERT_EQUAL(sys_mock_ops[4].args[5].o, 0);
  CU_ASSERT_EQUAL(((size_t) (sys_mock_ops[4].res.vp)) % page_size, 0);
  CU_ASSERT_EQUAL(sys_mock_ops[4].err_num, 0);
  CU_ASSERT_STRING_EQUAL(sys_mock_ops[5].fun_name, "getpagesize");
  CU_ASSERT_EQUAL(sys_mock_ops[5].res.i, page_size);
  CU_ASSERT_EQUAL(sys_mock_ops[5].err_num, 0);
  CU_ASSERT_STRING_EQUAL(sys_mock_ops[6].fun_name, "munmap");
  CU_ASSERT_PTR_EQUAL(sys_mock_ops[6].args[0].vp, ptr_to_header(ptr));
  CU_ASSERT_EQUAL(sys_mock_ops[6].args[1].i, (((2 * 1024 * 1024) + align_up(sizeof(struct malloc_header)) + page_size - 1) / page_size) * page_size);
  CU_ASSERT_EQUAL(sys_mock_ops[6].res.i, 0);
  CU_ASSERT_EQUAL(sys_mock_ops[6].err_num, 0);
  CU_ASSERT_EQUAL(ptr_to_header(ptr2)->size, 3 * 1024 * 1024);
  CU_ASSERT_EQUAL(ptr_to_header(ptr2)->flags, MALLOC_FLAG_MMAP);
  CU_ASSERT(repeatly_equal_char(ptr2, 'a', 2 * 1024 * 1024));
  CU_ASSERT(is_integrated_heap_tree());
  CU_ASSERT_EQUAL(calc_heap_tree_count(), 0);
  CU_ASSERT(is_integrated_list());
  CU_ASSERT_EQUAL(calc_list_count(), 0);
}

void test_realloc_maps_memory_area_for_decreasing_memory_area(void)
{
  void *ptr;
  void *ptr2;
  int page_size = getpagesize();
  clear_sys_mock();
  restore_malloc_vars();
  ptr = uportlibc_malloc(3 * 1024 * 1024);
  memset(ptr, 'b', 3 * 1024 * 1024);
  errno = 0;
  ptr2 = uportlibc_realloc(ptr, 2 * 1024 * 1024);
  CU_ASSERT_EQUAL(((size_t) ptr_to_header(ptr2)) % page_size, 0);
  CU_ASSERT_EQUAL(errno, 0);
  CU_ASSERT_EQUAL(sys_mock_op_count, 7);
  CU_ASSERT_STRING_EQUAL(sys_mock_ops[2].fun_name, "getpagesize");
  CU_ASSERT_EQUAL(sys_mock_ops[2].res.i, page_size);
  CU_ASSERT_EQUAL(sys_mock_ops[2].err_num, 0);
  CU_ASSERT_STRING_EQUAL(sys_mock_ops[3].fun_name, "getpagesize");
  CU_ASSERT_EQUAL(sys_mock_ops[3].res.i, page_size);
  CU_ASSERT_EQUAL(sys_mock_ops[3].err_num, 0);
  CU_ASSERT_STRING_EQUAL(sys_mock_ops[4].fun_name, "mmap");
  CU_ASSERT_PTR_NULL(sys_mock_ops[4].args[0].vp);
  CU_ASSERT_EQUAL(sys_mock_ops[4].args[1].i, (((2 * 1024 * 1024) + align_up(sizeof(struct malloc_header)) + page_size - 1) / page_size) * page_size);
  CU_ASSERT_EQUAL(sys_mock_ops[4].args[2].i, PROT_READ | PROT_WRITE);
  CU_ASSERT_EQUAL(sys_mock_ops[4].args[3].i, SYS_MOCK_MAP_PRIVATE | SYS_MOCK_MAP_ANON);
  CU_ASSERT_EQUAL(sys_mock_ops[4].args[4].i, -1);
  CU_ASSERT_EQUAL(sys_mock_ops[4].args[5].o, 0);
  CU_ASSERT_EQUAL(((size_t) (sys_mock_ops[4].res.vp)) % page_size, 0);
  CU_ASSERT_EQUAL(sys_mock_ops[4].err_num, 0);
  CU_ASSERT_STRING_EQUAL(sys_mock_ops[5].fun_name, "getpagesize");
  CU_ASSERT_EQUAL(sys_mock_ops[5].res.i, page_size);
  CU_ASSERT_EQUAL(sys_mock_ops[5].err_num, 0);
  CU_ASSERT_STRING_EQUAL(sys_mock_ops[6].fun_name, "munmap");
  CU_ASSERT_PTR_EQUAL(sys_mock_ops[6].args[0].vp, ptr_to_header(ptr));
  CU_ASSERT_EQUAL(sys_mock_ops[6].args[1].i, (((3 * 1024 * 1024) + align_up(sizeof(struct malloc_header)) + page_size - 1) / page_size) * page_size);
  CU_ASSERT_EQUAL(sys_mock_ops[6].res.i, 0);
  CU_ASSERT_EQUAL(sys_mock_ops[6].err_num, 0);
  CU_ASSERT_EQUAL(ptr_to_header(ptr2)->size, 2 * 1024 * 1024);
  CU_ASSERT_EQUAL(ptr_to_header(ptr2)->flags, MALLOC_FLAG_MMAP);
  CU_ASSERT(repeatly_equal_char(ptr2, 'b', 2 * 1024 * 1024));
  CU_ASSERT(is_integrated_heap_tree());
  CU_ASSERT_EQUAL(calc_heap_tree_count(), 0);
  CU_ASSERT(is_integrated_list());
  CU_ASSERT_EQUAL(calc_list_count(), 0);
}

void test_realloc_does_not_map_memory_area_for_old_len_equal_to_len(void)
{
  void *ptr;
  void *ptr2;
  int page_size = getpagesize();
  clear_sys_mock();
  restore_malloc_vars();
  ptr = uportlibc_malloc(2 * 1024 * 1024);
  memset(ptr, 'a', 2 * 1024 * 1024);
  errno = 0;
  ptr2 = uportlibc_realloc(ptr, 2 * 1024 * 1024 + 1);
  CU_ASSERT_PTR_EQUAL(ptr2, ptr);
  CU_ASSERT_EQUAL(errno, 0);
  CU_ASSERT_EQUAL(sys_mock_op_count, 3);
  CU_ASSERT_STRING_EQUAL(sys_mock_ops[2].fun_name, "getpagesize");
  CU_ASSERT_EQUAL(sys_mock_ops[2].res.i, page_size);
  CU_ASSERT_EQUAL(sys_mock_ops[2].err_num, 0);
  CU_ASSERT_EQUAL(ptr_to_header(ptr)->size, 2 * 1024 * 1024 + 8);
  CU_ASSERT(is_integrated_heap_tree());
  CU_ASSERT_EQUAL(calc_heap_tree_count(), 0);
  CU_ASSERT(is_integrated_list());
  CU_ASSERT_EQUAL(calc_list_count(), 0);
}

void test_realloc_allocates_memory_area_by_sbrk_for_mapped_memory_area(void)
{
  void *ptr;
  void *ptr2;
  int page_size = getpagesize();
  clear_sys_mock();
  restore_malloc_vars();
  ptr = uportlibc_malloc(2 * 1024 * 1024);
  memcpy(ptr, "ABCDEFGHabcdefgh1234567812345678", 32);
  errno = 0;
  ptr2 = uportlibc_realloc(ptr, 32);
  CU_ASSERT_PTR_EQUAL(ptr2, sys_mock_data_seg + align_up(sizeof(struct malloc_header)) * 1);
  CU_ASSERT_EQUAL(errno, 0);
  CU_ASSERT_EQUAL(sys_mock_op_count, 6);
  CU_ASSERT_STRING_EQUAL(sys_mock_ops[2].fun_name, "getpagesize");
  CU_ASSERT_EQUAL(sys_mock_ops[2].res.i, page_size);
  CU_ASSERT_EQUAL(sys_mock_ops[2].err_num, 0);
  CU_ASSERT_STRING_EQUAL(sys_mock_ops[3].fun_name, "sbrk");
  CU_ASSERT_EQUAL(sys_mock_ops[3].args[0].i, align_up(sizeof(struct malloc_header)) + 32);
  CU_ASSERT_PTR_EQUAL(sys_mock_ops[3].res.vp, sys_mock_data_seg);
  CU_ASSERT_EQUAL(sys_mock_ops[3].err_num, 0);
  CU_ASSERT_STRING_EQUAL(sys_mock_ops[4].fun_name, "getpagesize");
  CU_ASSERT_EQUAL(sys_mock_ops[4].res.i, page_size);
  CU_ASSERT_EQUAL(sys_mock_ops[4].err_num, 0);
  CU_ASSERT_STRING_EQUAL(sys_mock_ops[5].fun_name, "munmap");
  CU_ASSERT_PTR_EQUAL(sys_mock_ops[5].args[0].vp, ptr_to_header(ptr));
  CU_ASSERT_EQUAL(sys_mock_ops[5].args[1].i, (((2 * 1024 * 1024) + align_up(sizeof(struct malloc_header)) + page_size - 1) / page_size) * page_size);
  CU_ASSERT_EQUAL(sys_mock_ops[5].res.i, 0);
  CU_ASSERT_EQUAL(sys_mock_ops[5].err_num, 0);
  CU_ASSERT_PTR_EQUAL(sys_mock_break, sys_mock_data_seg + align_up(sizeof(struct malloc_header)) * 1 + 32);
  CU_ASSERT_EQUAL(ptr_to_header(ptr2)->size, 32);
  CU_ASSERT_EQUAL(ptr_to_header(ptr2)->flags, 0);
  CU_ASSERT_NSTRING_EQUAL((char *) ptr2, "ABCDEFGHabcdefgh1234567812345678", 32);
  CU_ASSERT(is_integrated_heap_tree());
  CU_ASSERT_EQUAL(calc_heap_tree_count(), 0);
  CU_ASSERT(is_integrated_list());
  CU_ASSERT_EQUAL(calc_list_count(), 1);
}

void test_realloc_maps_memory_area_for_allocated_memory_area_by_sbrk(void)
{
  void *ptr;
  void *ptr2;
  int page_size = getpagesize();
  clear_sys_mock();
  restore_malloc_vars();
  ptr = uportlibc_malloc(24);
  memcpy(ptr, "123456781234567812345678", 24);
  errno = 0;
  ptr2 = uportlibc_realloc(ptr, 2 * 1024 * 1024);
  CU_ASSERT_EQUAL(((size_t) ptr_to_header(ptr2)) % page_size, 0);
  CU_ASSERT_EQUAL(errno, 0);
  CU_ASSERT_EQUAL(sys_mock_op_count, 5);
  CU_ASSERT_STRING_EQUAL(sys_mock_ops[1].fun_name, "getpagesize");
  CU_ASSERT_EQUAL(sys_mock_ops[1].res.i, page_size);
  CU_ASSERT_EQUAL(sys_mock_ops[1].err_num, 0);
  CU_ASSERT_STRING_EQUAL(sys_mock_ops[2].fun_name, "mmap");
  CU_ASSERT_PTR_NULL(sys_mock_ops[2].args[0].vp);
  CU_ASSERT_EQUAL(sys_mock_ops[2].args[1].i, (((2 * 1024 * 1024) + align_up(sizeof(struct malloc_header)) + page_size - 1) / page_size) * page_size);
  CU_ASSERT_EQUAL(sys_mock_ops[2].args[2].i, PROT_READ | PROT_WRITE);
  CU_ASSERT_EQUAL(sys_mock_ops[2].args[3].i, SYS_MOCK_MAP_PRIVATE | SYS_MOCK_MAP_ANON);
  CU_ASSERT_EQUAL(sys_mock_ops[2].args[4].i, -1);
  CU_ASSERT_EQUAL(sys_mock_ops[2].args[5].o, 0);
  CU_ASSERT_EQUAL(((size_t) (sys_mock_ops[2].res.vp)) % page_size, 0);
  CU_ASSERT_EQUAL(sys_mock_ops[2].err_num, 0);
  CU_ASSERT_STRING_EQUAL(sys_mock_ops[3].fun_name, "sbrk");
  CU_ASSERT_EQUAL(sys_mock_ops[3].res.vp, sys_mock_data_seg + align_up(sizeof(struct malloc_header)) * 1 + 24);
  CU_ASSERT_EQUAL(sys_mock_ops[3].err_num, 0);
  CU_ASSERT_STRING_EQUAL(sys_mock_ops[4].fun_name, "sbrk");
  CU_ASSERT_EQUAL(sys_mock_ops[4].args[0].i, -(((ssize_t) align_up(sizeof(struct malloc_header))) + 24));
  CU_ASSERT_EQUAL(sys_mock_ops[4].res.i, sys_mock_data_seg + align_up(sizeof(struct malloc_header)) * 1 + 24);
  CU_ASSERT_EQUAL(sys_mock_ops[4].err_num, 0);
  CU_ASSERT_EQUAL(ptr_to_header(ptr2)->size, 2 * 1024 * 1024);
  CU_ASSERT_EQUAL(ptr_to_header(ptr2)->flags, MALLOC_FLAG_MMAP);
  CU_ASSERT_NSTRING_EQUAL((char *) ptr2, "123456781234567812345678", 24);
  CU_ASSERT(is_integrated_heap_tree());
  CU_ASSERT_EQUAL(calc_heap_tree_count(), 0);
  CU_ASSERT(is_integrated_list());
  CU_ASSERT_EQUAL(calc_list_count(), 0);
}

void test_realloc_returns_null_pointer_and_sets_errno_when_error_occurs(void)
{
  void *ptr;
  void *ptr2;
  clear_sys_mock();
  restore_malloc_vars();
  ptr = uportlibc_malloc(100);
  errno = 0;
  sys_mock_sbrk_res = (void *) (-1);
  sys_mock_sbrk_err_num = ENOMEM;
  ptr2 = uportlibc_realloc(ptr, 200);
  CU_ASSERT_PTR_NULL(ptr2);
  CU_ASSERT_EQUAL(errno, ENOMEM);
  CU_ASSERT_EQUAL(sys_mock_op_count, 4);
  CU_ASSERT_STRING_EQUAL(sys_mock_ops[1].fun_name, "sbrk");
  CU_ASSERT_EQUAL(sys_mock_ops[1].args[0].i, 0);
  CU_ASSERT_PTR_EQUAL(sys_mock_ops[1].res.vp, sys_mock_data_seg + align_up(sizeof(struct malloc_header)) * 1 + 104);
  CU_ASSERT_EQUAL(sys_mock_ops[1].err_num, 0);
  CU_ASSERT_STRING_EQUAL(sys_mock_ops[2].fun_name, "sbrk");
  CU_ASSERT_EQUAL(sys_mock_ops[2].args[0].i, 96);
  CU_ASSERT_PTR_EQUAL(sys_mock_ops[2].res.vp, (void *) (-1));
  CU_ASSERT_EQUAL(sys_mock_ops[2].err_num, ENOMEM);
  CU_ASSERT_STRING_EQUAL(sys_mock_ops[3].fun_name, "sbrk");
  CU_ASSERT_EQUAL(sys_mock_ops[3].args[0].i, align_up(sizeof(struct malloc_header)) * 1 + 200);
  CU_ASSERT_PTR_EQUAL(sys_mock_ops[3].res.vp, (void *) (-1));
  CU_ASSERT_EQUAL(sys_mock_ops[3].err_num, ENOMEM);
  CU_ASSERT_PTR_EQUAL(sys_mock_break, sys_mock_data_seg + align_up(sizeof(struct malloc_header)) * 1 + 104);
  CU_ASSERT(is_integrated_heap_tree());
  CU_ASSERT_EQUAL(calc_heap_tree_count(), 0);
  CU_ASSERT(is_integrated_list());
  CU_ASSERT_EQUAL(calc_list_count(), 1);
}

void test_free_frees_memory(void)
{
  void *ptr;
  clear_sys_mock();
  restore_malloc_vars();
  ptr =  uportlibc_malloc(20);
  errno = 0;
  uportlibc_free(ptr);
  CU_ASSERT_EQUAL(errno, 0);
  CU_ASSERT_EQUAL(sys_mock_op_count, 3);
  CU_ASSERT_STRING_EQUAL(sys_mock_ops[1].fun_name, "sbrk");
  CU_ASSERT_EQUAL(sys_mock_ops[1].args[0].i, 0);
  CU_ASSERT_PTR_EQUAL(sys_mock_ops[1].res.vp, sys_mock_data_seg + align_up(sizeof(struct malloc_header)) * 1 + 24);
  CU_ASSERT_EQUAL(sys_mock_ops[1].err_num, 0);
  CU_ASSERT_STRING_EQUAL(sys_mock_ops[2].fun_name, "sbrk");
  CU_ASSERT_EQUAL(sys_mock_ops[2].args[0].i, -(((ssize_t) align_up(sizeof(struct malloc_header))) + 24));
  CU_ASSERT_PTR_EQUAL(sys_mock_ops[2].res.vp, sys_mock_data_seg + align_up(sizeof(struct malloc_header)) * 1 + 24);
  CU_ASSERT_EQUAL(sys_mock_ops[2].err_num, 0);
  CU_ASSERT_PTR_EQUAL(sys_mock_break, sys_mock_data_seg);
  CU_ASSERT(is_integrated_heap_tree());
  CU_ASSERT_EQUAL(calc_heap_tree_count(), 0);
  CU_ASSERT(is_integrated_list());
  CU_ASSERT_EQUAL(calc_list_count(), 0);
}

void test_free_frees_three_memory_areas(void)
{
  void *ptr1;
  void *ptr2;
  void *ptr3;
  clear_sys_mock();
  restore_malloc_vars();
  uportlibc_malloc(5);
  ptr1 = uportlibc_malloc(200);
  uportlibc_malloc(5);
  ptr2 = uportlibc_malloc(100);
  uportlibc_malloc(5);
  ptr3 = uportlibc_malloc(300);
  uportlibc_malloc(5);
  errno = 0;
  uportlibc_free(ptr1);
  CU_ASSERT_EQUAL(errno, 0);
  CU_ASSERT_EQUAL(sys_mock_op_count, 9);
  CU_ASSERT_STRING_EQUAL(sys_mock_ops[7].fun_name, "sbrk");
  CU_ASSERT_EQUAL(sys_mock_ops[7].args[0].i, 0);
  CU_ASSERT_PTR_EQUAL(sys_mock_ops[7].res.vp, sys_mock_data_seg + align_up(sizeof(struct malloc_header)) * 7 + 640);
  CU_ASSERT_EQUAL(sys_mock_ops[7].err_num, 0);
  CU_ASSERT_STRING_EQUAL(sys_mock_ops[8].fun_name, "sbrk");
  CU_ASSERT_EQUAL(sys_mock_ops[8].args[0].i, 0);
  CU_ASSERT_PTR_EQUAL(sys_mock_ops[8].res.vp, sys_mock_data_seg + align_up(sizeof(struct malloc_header)) * 7 + 640);
  CU_ASSERT_EQUAL(sys_mock_ops[8].err_num, 0);
  CU_ASSERT_PTR_EQUAL(sys_mock_break, sys_mock_data_seg + align_up(sizeof(struct malloc_header)) * 7 + 640);
  CU_ASSERT_PTR_EQUAL(ptr_to_header(ptr1), heap_root);
  CU_ASSERT_PTR_NULL(heap_root->left);
  CU_ASSERT_PTR_NULL(heap_root->right);
  CU_ASSERT(is_integrated_heap_tree());
  CU_ASSERT_EQUAL(calc_heap_tree_count(), 1);
  CU_ASSERT(is_integrated_list());
  CU_ASSERT_EQUAL(calc_list_count(), 7);
  errno = 0;
  uportlibc_free(ptr2);
  CU_ASSERT_EQUAL(errno, 0);
  CU_ASSERT_EQUAL(sys_mock_op_count, 11);
  CU_ASSERT_STRING_EQUAL(sys_mock_ops[9].fun_name, "sbrk");
  CU_ASSERT_EQUAL(sys_mock_ops[9].args[0].i, 0);
  CU_ASSERT_EQUAL(sys_mock_ops[9].err_num, 0);
  CU_ASSERT_PTR_EQUAL(sys_mock_ops[9].res.vp, sys_mock_data_seg + align_up(sizeof(struct malloc_header)) * 7 + 640);
  CU_ASSERT_STRING_EQUAL(sys_mock_ops[10].fun_name, "sbrk");
  CU_ASSERT_EQUAL(sys_mock_ops[10].args[0].i, 0);
  CU_ASSERT_PTR_EQUAL(sys_mock_ops[10].res.vp, sys_mock_data_seg + align_up(sizeof(struct malloc_header)) * 7 + 640);
  CU_ASSERT_EQUAL(sys_mock_ops[10].err_num, 0);
  CU_ASSERT_PTR_EQUAL(sys_mock_break, sys_mock_data_seg + align_up(sizeof(struct malloc_header)) * 7 + 640);
  CU_ASSERT_PTR_EQUAL(ptr_to_header(ptr1), heap_root);
  CU_ASSERT_PTR_EQUAL(ptr_to_header(ptr2), heap_root->left);
  CU_ASSERT_PTR_NULL(heap_root->right);
  CU_ASSERT_PTR_NULL(heap_root->left->left);
  CU_ASSERT_PTR_NULL(heap_root->left->right);
  CU_ASSERT(is_integrated_heap_tree());
  CU_ASSERT_EQUAL(calc_heap_tree_count(), 2);
  CU_ASSERT(is_integrated_list());
  CU_ASSERT_EQUAL(calc_list_count(), 7);
  errno = 0;
  uportlibc_free(ptr3);
  CU_ASSERT_EQUAL(errno, 0);
  CU_ASSERT_EQUAL(sys_mock_op_count, 13);
  CU_ASSERT_STRING_EQUAL(sys_mock_ops[11].fun_name, "sbrk");
  CU_ASSERT_EQUAL(sys_mock_ops[11].args[0].i, 0);
  CU_ASSERT_EQUAL(sys_mock_ops[11].err_num, 0);
  CU_ASSERT_PTR_EQUAL(sys_mock_ops[11].res.vp, sys_mock_data_seg + align_up(sizeof(struct malloc_header)) * 7 + 640);
  CU_ASSERT_STRING_EQUAL(sys_mock_ops[12].fun_name, "sbrk");
  CU_ASSERT_EQUAL(sys_mock_ops[12].args[0].i, 0);
  CU_ASSERT_PTR_EQUAL(sys_mock_ops[12].res.vp, sys_mock_data_seg + align_up(sizeof(struct malloc_header)) * 7 + 640);
  CU_ASSERT_EQUAL(sys_mock_ops[12].err_num, 0);
  CU_ASSERT_PTR_EQUAL(sys_mock_break, sys_mock_data_seg + align_up(sizeof(struct malloc_header)) * 7 + 640);
  CU_ASSERT_PTR_EQUAL(ptr_to_header(ptr1), heap_root);
  CU_ASSERT_PTR_EQUAL(ptr_to_header(ptr2), heap_root->left);
  CU_ASSERT_PTR_EQUAL(ptr_to_header(ptr3), heap_root->right);
  CU_ASSERT_PTR_NULL(heap_root->left->left);
  CU_ASSERT_PTR_NULL(heap_root->left->right);
  CU_ASSERT_PTR_NULL(heap_root->right->left);
  CU_ASSERT_PTR_NULL(heap_root->right->right);
  CU_ASSERT(is_integrated_heap_tree());
  CU_ASSERT_EQUAL(calc_heap_tree_count(), 3);
  CU_ASSERT(is_integrated_list());
  CU_ASSERT_EQUAL(calc_list_count(), 7);
}

void test_free_frees_memory_area_for_previous_memory_area_and_next_memory_area(void)
{
  void *ptr;
  clear_sys_mock();
  restore_malloc_vars();
  uportlibc_malloc(5);
  ptr = uportlibc_malloc(100);
  uportlibc_malloc(5);
  errno = 0;
  uportlibc_free(ptr);
  CU_ASSERT_EQUAL(errno, 0);
  CU_ASSERT_EQUAL(sys_mock_op_count, 5);
  CU_ASSERT_STRING_EQUAL(sys_mock_ops[3].fun_name, "sbrk");
  CU_ASSERT_EQUAL(sys_mock_ops[3].args[0].i, 0);
  CU_ASSERT_PTR_EQUAL(sys_mock_ops[3].res.vp, sys_mock_data_seg + align_up(sizeof(struct malloc_header)) * 3 + 120);
  CU_ASSERT_EQUAL(sys_mock_ops[3].err_num, 0);
  CU_ASSERT_STRING_EQUAL(sys_mock_ops[4].fun_name, "sbrk");
  CU_ASSERT_EQUAL(sys_mock_ops[4].args[0].i, 0);
  CU_ASSERT_PTR_EQUAL(sys_mock_ops[4].res.vp, sys_mock_data_seg + align_up(sizeof(struct malloc_header)) * 3 + 120);
  CU_ASSERT_EQUAL(sys_mock_ops[4].err_num, 0);
  CU_ASSERT_PTR_EQUAL(sys_mock_break, sys_mock_data_seg + align_up(sizeof(struct malloc_header)) * 3 + 120);
  CU_ASSERT_EQUAL(ptr_to_header(ptr)->size, 104);
  CU_ASSERT(is_integrated_heap_tree());
  CU_ASSERT_EQUAL(calc_heap_tree_count(), 1);
  CU_ASSERT(is_integrated_list());
  CU_ASSERT_EQUAL(calc_list_count(), 3);
}

void test_free_frees_memory_area_for_previous_freed_memory_area_and_next_memory_area(void)
{
  void *ptr1;
  void *ptr2;
  clear_sys_mock();
  restore_malloc_vars();
  uportlibc_malloc(5);
  ptr1 = uportlibc_malloc(100);
  ptr2 = uportlibc_malloc(200);
  uportlibc_malloc(5);
  uportlibc_free(ptr1);
  errno = 0;
  uportlibc_free(ptr2);
  CU_ASSERT_EQUAL(errno, 0);
  CU_ASSERT_EQUAL(sys_mock_op_count, 8);
  CU_ASSERT_STRING_EQUAL(sys_mock_ops[6].fun_name, "sbrk");
  CU_ASSERT_EQUAL(sys_mock_ops[6].args[0].i, 0);
  CU_ASSERT_PTR_EQUAL(sys_mock_ops[6].res.vp, sys_mock_data_seg + align_up(sizeof(struct malloc_header)) * 4 + 320);
  CU_ASSERT_EQUAL(sys_mock_ops[6].err_num, 0);
  CU_ASSERT_STRING_EQUAL(sys_mock_ops[7].fun_name, "sbrk");
  CU_ASSERT_EQUAL(sys_mock_ops[7].args[0].i, 0);
  CU_ASSERT_PTR_EQUAL(sys_mock_ops[7].res.vp, sys_mock_data_seg + align_up(sizeof(struct malloc_header)) * 4 + 320);
  CU_ASSERT_EQUAL(sys_mock_ops[7].err_num, 0);
  CU_ASSERT_PTR_EQUAL(sys_mock_break, sys_mock_data_seg + align_up(sizeof(struct malloc_header)) * 4 + 320);
  CU_ASSERT_EQUAL(ptr_to_header(ptr1)->size, 104 + align_up(sizeof(struct malloc_header)) + 200);
  CU_ASSERT(is_integrated_heap_tree());
  CU_ASSERT_EQUAL(calc_heap_tree_count(), 1);
  CU_ASSERT(is_integrated_list());
  CU_ASSERT_EQUAL(calc_list_count(), 3);
}

void test_free_frees_memory_area_for_previous_memory_area_and_next_freed_memory_area(void)
{
  void *ptr1;
  void *ptr2;
  clear_sys_mock();
  restore_malloc_vars();
  uportlibc_malloc(5);
  ptr1 = uportlibc_malloc(200);
  ptr2 = uportlibc_malloc(100);
  uportlibc_malloc(5);
  uportlibc_free(ptr2);
  errno = 0;
  uportlibc_free(ptr1);
  CU_ASSERT_EQUAL(errno, 0);
  CU_ASSERT_EQUAL(sys_mock_op_count, 8);
  CU_ASSERT_STRING_EQUAL(sys_mock_ops[6].fun_name, "sbrk");
  CU_ASSERT_EQUAL(sys_mock_ops[6].args[0].i, 0);
  CU_ASSERT_PTR_EQUAL(sys_mock_ops[6].res.vp, sys_mock_data_seg + align_up(sizeof(struct malloc_header)) * 4 + 320);
  CU_ASSERT_EQUAL(sys_mock_ops[6].err_num, 0);
  CU_ASSERT_STRING_EQUAL(sys_mock_ops[7].fun_name, "sbrk");
  CU_ASSERT_EQUAL(sys_mock_ops[7].args[0].i, 0);
  CU_ASSERT_PTR_EQUAL(sys_mock_ops[7].res.vp, sys_mock_data_seg + align_up(sizeof(struct malloc_header)) * 4 + 320);
  CU_ASSERT_EQUAL(sys_mock_ops[7].err_num, 0);
  CU_ASSERT_PTR_EQUAL(sys_mock_break, sys_mock_data_seg + align_up(sizeof(struct malloc_header)) * 4 + 320);
  CU_ASSERT_EQUAL(ptr_to_header(ptr1)->size, 200 + align_up(sizeof(struct malloc_header)) + 104);
  CU_ASSERT(is_integrated_heap_tree());
  CU_ASSERT_EQUAL(calc_heap_tree_count(), 1);
  CU_ASSERT(is_integrated_list());
  CU_ASSERT_EQUAL(calc_list_count(), 3);
}

void test_free_frees_memory_area_for_previous_freed_memory_area_and_next_freed_memory_area(void)
{
  void *ptr1;
  void *ptr2;
  void *ptr3;
  clear_sys_mock();
  restore_malloc_vars();
  uportlibc_malloc(5);
  ptr1 = uportlibc_malloc(100);
  ptr2 = uportlibc_malloc(200);
  ptr3 = uportlibc_malloc(300);
  uportlibc_malloc(5);
  uportlibc_free(ptr1);
  uportlibc_free(ptr3);
  errno = 0;
  uportlibc_free(ptr2);
  CU_ASSERT_EQUAL(errno, 0);
  CU_ASSERT_EQUAL(sys_mock_op_count, 11);
  CU_ASSERT_STRING_EQUAL(sys_mock_ops[9].fun_name, "sbrk");
  CU_ASSERT_EQUAL(sys_mock_ops[9].args[0].i, 0);
  CU_ASSERT_PTR_EQUAL(sys_mock_ops[9].res.vp, sys_mock_data_seg + align_up(sizeof(struct malloc_header)) * 5 + 624);
  CU_ASSERT_EQUAL(sys_mock_ops[9].err_num, 0);
  CU_ASSERT_STRING_EQUAL(sys_mock_ops[10].fun_name, "sbrk");
  CU_ASSERT_EQUAL(sys_mock_ops[10].args[0].i, 0);
  CU_ASSERT_PTR_EQUAL(sys_mock_ops[10].res.vp, sys_mock_data_seg + align_up(sizeof(struct malloc_header)) * 5 + 624);
  CU_ASSERT_EQUAL(sys_mock_ops[10].err_num, 0);
  CU_ASSERT_PTR_EQUAL(sys_mock_break, sys_mock_data_seg + align_up(sizeof(struct malloc_header)) * 5 + 624);
  CU_ASSERT_EQUAL(ptr_to_header(ptr1)->size, align_up(sizeof(struct malloc_header)) * 2 + 608);
  CU_ASSERT(is_integrated_heap_tree());
  CU_ASSERT_EQUAL(calc_heap_tree_count(), 1);
  CU_ASSERT(is_integrated_list());
  CU_ASSERT_EQUAL(calc_list_count(), 3);
}

void test_free_frees_memory_area_for_next_memory_area(void)
{
  void *ptr;
  clear_sys_mock();
  restore_malloc_vars();
  ptr = uportlibc_malloc(100);
  uportlibc_malloc(5);
  errno = 0;
  uportlibc_free(ptr);
  CU_ASSERT_EQUAL(errno, 0);
  CU_ASSERT_EQUAL(sys_mock_op_count, 4);
  CU_ASSERT_STRING_EQUAL(sys_mock_ops[2].fun_name, "sbrk");
  CU_ASSERT_EQUAL(sys_mock_ops[2].args[0].i, 0);
  CU_ASSERT_PTR_EQUAL(sys_mock_ops[2].res.vp, sys_mock_data_seg + align_up(sizeof(struct malloc_header)) * 2 + 112);
  CU_ASSERT_EQUAL(sys_mock_ops[2].err_num, 0);
  CU_ASSERT_STRING_EQUAL(sys_mock_ops[3].fun_name, "sbrk");
  CU_ASSERT_EQUAL(sys_mock_ops[3].args[0].i, 0);
  CU_ASSERT_PTR_EQUAL(sys_mock_ops[3].res.vp, sys_mock_data_seg + align_up(sizeof(struct malloc_header)) * 2 + 112);
  CU_ASSERT_EQUAL(sys_mock_ops[3].err_num, 0);
  CU_ASSERT_PTR_EQUAL(sys_mock_break, sys_mock_data_seg + align_up(sizeof(struct malloc_header)) * 2 + 112);
  CU_ASSERT_EQUAL(ptr_to_header(ptr)->size, 104);
  CU_ASSERT(is_integrated_heap_tree());
  CU_ASSERT_EQUAL(calc_heap_tree_count(), 1);
  CU_ASSERT(is_integrated_list());
  CU_ASSERT_EQUAL(calc_list_count(), 2);
}

void test_free_frees_memory_area_for_next_freed_memory_area(void)
{
  void *ptr1;
  void *ptr2;
  clear_sys_mock();
  restore_malloc_vars();
  ptr1 = uportlibc_malloc(100);
  ptr2 = uportlibc_malloc(200);
  uportlibc_malloc(5);
  uportlibc_free(ptr2);
  errno = 0;
  uportlibc_free(ptr1);
  CU_ASSERT_EQUAL(errno, 0);
  CU_ASSERT_EQUAL(sys_mock_op_count, 7);
  CU_ASSERT_STRING_EQUAL(sys_mock_ops[5].fun_name, "sbrk");
  CU_ASSERT_EQUAL(sys_mock_ops[5].args[0].i, 0);
  CU_ASSERT_PTR_EQUAL(sys_mock_ops[5].res.vp, sys_mock_data_seg + align_up(sizeof(struct malloc_header)) * 3 + 312);
  CU_ASSERT_EQUAL(sys_mock_ops[5].err_num, 0);
  CU_ASSERT_STRING_EQUAL(sys_mock_ops[6].fun_name, "sbrk");
  CU_ASSERT_EQUAL(sys_mock_ops[6].args[0].i, 0);
  CU_ASSERT_PTR_EQUAL(sys_mock_ops[6].res.vp, sys_mock_data_seg + align_up(sizeof(struct malloc_header)) * 3 + 312);
  CU_ASSERT_EQUAL(sys_mock_ops[6].err_num, 0);
  CU_ASSERT_PTR_EQUAL(sys_mock_break, sys_mock_data_seg + align_up(sizeof(struct malloc_header)) * 3 + 312);
  CU_ASSERT_EQUAL(ptr_to_header(ptr1)->size, 104 + align_up(sizeof(struct malloc_header)) + 200);
  CU_ASSERT(is_integrated_heap_tree());
  CU_ASSERT_EQUAL(calc_heap_tree_count(), 1);
  CU_ASSERT(is_integrated_list());
  CU_ASSERT_EQUAL(calc_list_count(), 2);
}

void test_free_frees_memory_area_for_previous_memory_area(void)
{
  void *ptr;
  clear_sys_mock();
  restore_malloc_vars();
  uportlibc_malloc(5);
  ptr = uportlibc_malloc(100);
  errno = 0;
  uportlibc_free(ptr);
  CU_ASSERT_EQUAL(errno, 0);
  CU_ASSERT_EQUAL(sys_mock_op_count, 4);
  CU_ASSERT_STRING_EQUAL(sys_mock_ops[2].fun_name, "sbrk");
  CU_ASSERT_EQUAL(sys_mock_ops[2].args[0].i, 0);
  CU_ASSERT_PTR_EQUAL(sys_mock_ops[2].res.vp, sys_mock_data_seg + align_up(sizeof(struct malloc_header)) * 2 + 112);
  CU_ASSERT_EQUAL(sys_mock_ops[2].err_num, 0);
  CU_ASSERT_STRING_EQUAL(sys_mock_ops[3].fun_name, "sbrk");
  CU_ASSERT_EQUAL(sys_mock_ops[3].args[0].i, -(((ssize_t) align_up(sizeof(struct malloc_header))) + 104));
  CU_ASSERT_PTR_EQUAL(sys_mock_ops[3].res.vp, sys_mock_data_seg + align_up(sizeof(struct malloc_header)) * 2 + 112);
  CU_ASSERT_EQUAL(sys_mock_ops[3].err_num, 0);
  CU_ASSERT_PTR_EQUAL(sys_mock_break, sys_mock_data_seg + align_up(sizeof(struct malloc_header)) * 1 + 8);
  CU_ASSERT(is_integrated_heap_tree());
  CU_ASSERT_EQUAL(calc_heap_tree_count(), 0);
  CU_ASSERT(is_integrated_list());
  CU_ASSERT_EQUAL(calc_list_count(), 1);
}

void test_free_frees_memory_area_for_previous_freed_memory_area(void)
{
  void *ptr1;
  void *ptr2;
  clear_sys_mock();
  restore_malloc_vars();
  uportlibc_malloc(5);
  ptr1 = uportlibc_malloc(100);
  ptr2 = uportlibc_malloc(200);
  uportlibc_free(ptr1);
  errno = 0;
  uportlibc_free(ptr2);
  CU_ASSERT_EQUAL(errno, 0);
  CU_ASSERT_EQUAL(sys_mock_op_count, 7);
  CU_ASSERT_STRING_EQUAL(sys_mock_ops[5].fun_name, "sbrk");
  CU_ASSERT_EQUAL(sys_mock_ops[5].args[0].i, 0);
  CU_ASSERT_PTR_EQUAL(sys_mock_ops[5].res.vp, sys_mock_data_seg + align_up(sizeof(struct malloc_header)) * 3 + 312);
  CU_ASSERT_EQUAL(sys_mock_ops[5].err_num, 0);
  CU_ASSERT_STRING_EQUAL(sys_mock_ops[6].fun_name, "sbrk");
  CU_ASSERT_EQUAL(sys_mock_ops[6].args[0].i, -(((ssize_t) align_up(sizeof(struct malloc_header))) * 2 + 304));
  CU_ASSERT_PTR_EQUAL(sys_mock_ops[6].res.vp, sys_mock_data_seg + align_up(sizeof(struct malloc_header)) * 3 + 312);
  CU_ASSERT_EQUAL(sys_mock_ops[6].err_num, 0);
  CU_ASSERT_PTR_EQUAL(sys_mock_break, sys_mock_data_seg + align_up(sizeof(struct malloc_header)) * 1 + 8);
  CU_ASSERT(is_integrated_heap_tree());
  CU_ASSERT_EQUAL(calc_heap_tree_count(), 0);
  CU_ASSERT(is_integrated_list());
  CU_ASSERT_EQUAL(calc_list_count(), 1);
}

void test_free_frees_memory_area(void)
{
  void *ptr;
  clear_sys_mock();
  restore_malloc_vars();
  ptr =  uportlibc_malloc(200);
  errno = 0;
  uportlibc_free(ptr);
  CU_ASSERT_EQUAL(errno, 0);
  CU_ASSERT_EQUAL(sys_mock_op_count, 3);
  CU_ASSERT_STRING_EQUAL(sys_mock_ops[1].fun_name, "sbrk");
  CU_ASSERT_EQUAL(sys_mock_ops[1].args[0].i, 0);
  CU_ASSERT_PTR_EQUAL(sys_mock_ops[1].res.vp, sys_mock_data_seg + align_up(sizeof(struct malloc_header)) * 1 + 200);
  CU_ASSERT_EQUAL(sys_mock_ops[1].err_num, 0);
  CU_ASSERT_STRING_EQUAL(sys_mock_ops[2].fun_name, "sbrk");
  CU_ASSERT_EQUAL(sys_mock_ops[2].args[0].i, -(((ssize_t) align_up(sizeof(struct malloc_header))) + 200));
  CU_ASSERT_PTR_EQUAL(sys_mock_ops[2].res.vp, sys_mock_data_seg + align_up(sizeof(struct malloc_header)) * 1 + 200);
  CU_ASSERT_EQUAL(sys_mock_ops[2].err_num, 0);
  CU_ASSERT_PTR_EQUAL(sys_mock_break, sys_mock_data_seg);
  CU_ASSERT(is_integrated_heap_tree());
  CU_ASSERT_EQUAL(calc_heap_tree_count(), 0);
  CU_ASSERT(is_integrated_list());
  CU_ASSERT_EQUAL(calc_list_count(), 0);
}

void test_free_frees_random_memory_areas(void)
{
  static size_t sizes[100];
  static void *ptrs[100];
  void *ptr;
  size_t i, len = floor((rand() / (RAND_MAX * 1.0)) * 50) + 50;
  clear_sys_mock();
  restore_malloc_vars();
  for(i = 0; i < len; i++) {
    sizes[i] = floor((rand() / (RAND_MAX * 1.0)) * 1000.0) + 1;
  }
  for(i = 0; i < len; i++) {
    uportlibc_malloc(5);
    ptrs[i] = uportlibc_malloc(sizes[i]);
  }
  uportlibc_malloc(5);
  for(i = 0; i < len; i++) {
    uportlibc_free(ptrs[i]);
  }
  CU_ASSERT_EQUAL(sys_mock_op_count, len * 4 + 1);
  CU_ASSERT(is_integrated_heap_tree());
  CU_ASSERT_EQUAL(calc_heap_tree_count(), len);
  CU_ASSERT(is_integrated_list());
  CU_ASSERT_EQUAL(calc_list_count(), len * 2 + 1);
}

void test_free_unmaps_memory_area(void)
{
  void *ptr;
  int page_size = getpagesize();
  clear_sys_mock();
  restore_malloc_vars();
  ptr = uportlibc_malloc(2 * 1024 * 1024);
  errno = 0;
  uportlibc_free(ptr);
  CU_ASSERT_EQUAL(errno, 0);
  CU_ASSERT_EQUAL(sys_mock_op_count, 4);
  CU_ASSERT_STRING_EQUAL(sys_mock_ops[2].fun_name, "getpagesize");
  CU_ASSERT_EQUAL(sys_mock_ops[2].res.i, page_size);
  CU_ASSERT_EQUAL(sys_mock_ops[2].err_num, 0);
  CU_ASSERT_STRING_EQUAL(sys_mock_ops[3].fun_name, "munmap");
  CU_ASSERT_PTR_EQUAL(sys_mock_ops[3].args[0].vp, ptr_to_header(ptr));
  CU_ASSERT_EQUAL(sys_mock_ops[3].args[1].i, ((align_up(sizeof(struct malloc_header)) + 2 * 1024 * 1024 + page_size - 1) / page_size) * page_size);
  CU_ASSERT_EQUAL(sys_mock_ops[3].res.i, 0);
  CU_ASSERT_EQUAL(sys_mock_ops[3].err_num, 0);
  CU_ASSERT(is_integrated_heap_tree());
  CU_ASSERT_EQUAL(calc_heap_tree_count(), 0);
  CU_ASSERT(is_integrated_list());
  CU_ASSERT_EQUAL(calc_list_count(), 0);
}

int add_malloc_suite(void)
{
  CU_pSuite suite;
  suite = CU_add_suite("malloc suite", initialize_malloc, cleanup_malloc);
  if(suite == NULL) return -1;
  if(CU_add_test(suite,
    "insert_header inserts one header",
    test_insert_header_inserts_one_header) == NULL) return -1;
  if(CU_add_test(suite,
    "insert_header inserts seven headers",
    test_insert_header_inserts_seven_headers) == NULL) return -1;
  if(CU_add_test(suite,
    "insert_header inserts for left-left case",
    test_insert_header_inserts_for_left_left_case) == NULL) return -1;
  if(CU_add_test(suite,
    "insert_header inserts for left-right case",
    test_insert_header_inserts_for_left_right_case) == NULL) return -1;
  if(CU_add_test(suite,
    "insert_header inserts for right-right case",
    test_insert_header_inserts_for_right_right_case) == NULL) return -1;
  if(CU_add_test(suite,
    "insert_header inserts for right-left case",
    test_insert_header_inserts_for_right_left_case) == NULL) return -1;
  if(CU_add_test(suite,
    "insert_header inserts for two same sizes",
    test_insert_header_inserts_for_two_same_sizes) == NULL) return -1;
  if(CU_add_test(suite,
    "insert_header inserts random headers",
    test_insert_header_inserts_random_headers) == NULL) return -1;
  if(CU_add_test(suite,
    "find_best_header returns header for one header",
    test_find_best_header_returns_header_for_one_header) == NULL) return -1;
  if(CU_add_test(suite,
    "find_best_header returns null pointer for one header",
    test_find_best_header_returns_null_pointer_for_one_header) == NULL) return -1;
  if(CU_add_test(suite,
    "find_best_header returns null pointer for empty heap tree",
    test_find_best_header_returns_null_pointer_for_empty_heap_tree) == NULL) return -1;
  if(CU_add_test(suite,
    "find_best_header returns header for seven headers",
    test_find_best_header_returns_header_for_seven_headers) == NULL) return -1;
  if(CU_add_test(suite,
    "find_best_header returns null pointer for seven headers",
    test_find_best_header_returns_null_pointer_for_seven_headers) == NULL) return -1;
  if(CU_add_test(suite,
    "find_best_header returns header for random headers",
    test_find_best_header_returns_header_for_random_headers) == NULL) return -1;
  if(CU_add_test(suite,
    "find_best_header returns null pointer for random headers",
    test_find_best_header_returns_null_pointer_for_random_headers) == NULL) return -1;
  if(CU_add_test(suite,
    "delete_header deletes one header for one header",
    test_delete_header_deletes_one_header_for_one_header) == NULL) return -1;
  if(CU_add_test(suite,
    "delete_header deletes one header for seven headers",
    test_delete_header_deletes_one_header_for_seven_headers) == NULL) return -1;
  if(CU_add_test(suite,
    "delete_header deletes for left-left case",
    test_delete_header_deletes_for_left_left_case) == NULL) return -1;
  if(CU_add_test(suite,
    "delete_header deletes for left-right case",
    test_delete_header_deletes_for_left_right_case) == NULL) return -1;
  if(CU_add_test(suite,
    "delete_header deletes for right-right case",
    test_delete_header_deletes_for_right_right_case) == NULL) return -1;
  if(CU_add_test(suite,
    "delete_header deletes for right-left case",
    test_delete_header_deletes_for_right_left_case) == NULL) return -1;
  if(CU_add_test(suite,
    "delete_header replaces header for left-right node with branch",
    test_delete_header_replaces_header_for_left_right_node_with_branch) == NULL) return -1;
  if(CU_add_test(suite,
    "delete_header replaces header for left-right node without branch",
    test_delete_header_replaces_header_for_left_right_node_without_branch) == NULL) return -1;
  if(CU_add_test(suite,
    "delete_header replaces header for left-right node without branch and right-right case",
    test_delete_header_replaces_header_for_left_right_node_without_branch_and_right_right_case) == NULL) return -1;
  if(CU_add_test(suite,
    "delete_header replaces header for left node with branch",
    test_delete_header_replaces_header_for_left_node_with_branch) == NULL) return -1;
  if(CU_add_test(suite,
    "delete_header replaces header for left node without branch",
    test_delete_header_replaces_header_for_left_node_without_branch) == NULL) return -1;
  if(CU_add_test(suite,
    "delete_header replaces header for right-left node with branch",
    test_delete_header_replaces_header_for_right_left_node_with_branch) == NULL) return -1;
  if(CU_add_test(suite,
    "delete_header replaces header for right-left node without branch",
    test_delete_header_replaces_header_for_right_left_node_without_branch) == NULL) return -1;
  if(CU_add_test(suite,
    "delete_header replaces header for right node without branch",
    test_delete_header_replaces_header_for_right_node_without_branch) == NULL) return -1;
  if(CU_add_test(suite,
    "delete_header deletes for random headers",
    test_delete_header_deletes_for_random_headers) == NULL) return -1;
  if(CU_add_test(suite,
    "malloc allocates memory",
    test_malloc_allocates_memory) == NULL) return -1;
  if(CU_add_test(suite,
    "malloc allocates three memory areas",
    test_malloc_allocates_three_memory_areas) == NULL) return -1;
  if(CU_add_test(suite,
    "malloc returns null pointer for size that is zero",
    test_malloc_returns_null_pointer_for_size_that_is_zero) == NULL) return -1;
  if(CU_add_test(suite,
    "malloc allocates freed memory area",
    test_malloc_allocates_freed_memory_area) == NULL) return -1;
  if(CU_add_test(suite,
    "malloc allocates part of freed memory area",
    test_malloc_allocates_part_of_freed_memory_area) == NULL) return -1;
  if(CU_add_test(suite,
    "malloc allocates part of freed memory area that is too large for partition",
    test_malloc_allocates_part_of_freed_memory_area_that_is_too_large_for_partition) == NULL) return -1;
  if(CU_add_test(suite,
    "malloc allocates memory area that is too large for freed memory areas",
    test_malloc_allocates_memory_area_that_is_too_large_for_freed_memory_areas) == NULL) return -1;
  if(CU_add_test(suite,
    "malloc allocates freed memory area for random freed memory areas",
    test_malloc_allocates_freed_memory_area_for_random_freed_memory_areas) == NULL) return -1;
  if(CU_add_test(suite,
    "malloc maps memory area",
    test_malloc_maps_memory_area) == NULL) return -1;
  if(CU_add_test(suite,
    "malloc returns null pointer and sets errno when error occurs for sbrk",
    test_malloc_returns_null_pointer_and_sets_errno_when_error_occurs_for_sbrk) == NULL) return -1;
  if(CU_add_test(suite,
    "malloc returns null pointer and sets errno when error occurs for mmap",
    test_malloc_returns_null_pointer_and_sets_errno_when_error_occurs_for_mmap) == NULL) return -1;
  if(CU_add_test(suite,
    "realloc reallocates memory",
    test_realloc_reallocates_memory) == NULL) return -1;
  if(CU_add_test(suite,
    "realloc reallocates three memory areas",
    test_realloc_reallocates_three_memory_areas) == NULL) return -1;
  if(CU_add_test(suite,
    "realloc allotates memory for pointer that is null pointer",
    test_realloc_allotates_memory_for_pointer_that_is_null_pointer) == NULL) return -1;
  if(CU_add_test(suite,
    "realloc returns null pointer for pointer that is null pointer and size that is zero",
    test_realloc_returns_null_pointer_for_pointer_that_is_null_pointer_and_size_that_is_zero) == NULL) return -1;
  if(CU_add_test(suite,
    "realloc frees memory for size that is zero",
    test_realloc_frees_memory_for_size_that_is_zero) == NULL) return -1;
  if(CU_add_test(suite,
    "realloc increases memory area for freed memory area",
    test_realloc_increases_memory_area_for_freed_memory_area) == NULL) return -1;
  if(CU_add_test(suite,
    "realloc increases memory area to maximum for freed memory area",
    test_realloc_increases_memory_area_to_maximum_for_freed_memory_area) == NULL) return -1;
  if(CU_add_test(suite,
    "realloc increases memory area that will be too large for partition and freed memory area",
    test_realloc_increases_memory_area_that_will_be_too_large_for_partition_and_freed_memory_area) == NULL) return -1;
  if(CU_add_test(suite,
    "realloc moves memory area for freed memory area",
    test_realloc_moves_memory_area_for_freed_memory_area) == NULL) return -1;
  if(CU_add_test(suite,
    "realloc moves memory area",
    test_realloc_moves_memory_area) == NULL) return -1;
  if(CU_add_test(suite,
    "realloc increases memory area for sbrk",
    test_realloc_increases_memory_area_for_sbrk) == NULL) return -1;
  if(CU_add_test(suite,
    "realloc returns pointer for old_size that is eqaul to size",
    test_realloc_returns_pointer_for_old_size_that_is_eqaul_to_size) == NULL) return -1;
  if(CU_add_test(suite,
    "realloc decreases memory area for freed memory area",
    test_realloc_decreases_memory_area_for_freed_memory_area) == NULL) return -1;
  if(CU_add_test(suite,
    "realloc decreases memory area",
    test_realloc_decreases_memory_area) == NULL) return -1;
  if(CU_add_test(suite,
    "realloc decreases memory area that will be too large for partition",
    test_realloc_decreases_memory_area_that_will_be_too_large_for_partition) == NULL) return -1;
  if(CU_add_test(suite,
    "realloc decreases memory area for sbrk",
    test_realloc_decreases_memory_area_for_sbrk) == NULL) return -1;
  if(CU_add_test(suite,
    "realloc maps memory area for increasing memory area",
    test_realloc_maps_memory_area_for_increasing_memory_area) == NULL) return -1;
  if(CU_add_test(suite,
    "realloc maps memory area for decreasing memory area",
    test_realloc_maps_memory_area_for_decreasing_memory_area) == NULL) return -1;
  if(CU_add_test(suite,
    "realloc does not map memory area for old_len equal to len",
    test_realloc_does_not_map_memory_area_for_old_len_equal_to_len) == NULL) return -1;
  if(CU_add_test(suite,
    "realloc allocates memory area by sbrk for mapped memory area",
    test_realloc_allocates_memory_area_by_sbrk_for_mapped_memory_area) == NULL) return -1;
  if(CU_add_test(suite,
    "realloc maps memory area for allocated memory area by sbrk",
    test_realloc_maps_memory_area_for_allocated_memory_area_by_sbrk) == NULL) return -1;
  if(CU_add_test(suite,
    "realloc returns null pointer and sets errno when error occurs",
    test_realloc_returns_null_pointer_and_sets_errno_when_error_occurs) == NULL) return -1;
  if(CU_add_test(suite,
    "free frees memory",
    test_free_frees_memory) == NULL) return -1;
  if(CU_add_test(suite,
    "free frees three memory areas",
    test_free_frees_three_memory_areas) == NULL) return -1;
  if(CU_add_test(suite,
    "free frees memory area for previous memory area and next memory area",
    test_free_frees_memory_area_for_previous_memory_area_and_next_memory_area) == NULL) return -1;
  if(CU_add_test(suite,
    "free frees memory area for previous freed memory area and next memory area",
    test_free_frees_memory_area_for_previous_freed_memory_area_and_next_memory_area) == NULL) return -1;
  if(CU_add_test(suite,
    "free frees memory area for previous memory area and next freed memory area",
    test_free_frees_memory_area_for_previous_memory_area_and_next_freed_memory_area) == NULL) return -1;
  if(CU_add_test(suite,
    "free frees memory area for previous freed memory area and next freed memory area",
    test_free_frees_memory_area_for_previous_freed_memory_area_and_next_freed_memory_area) == NULL) return -1;
  if(CU_add_test(suite,
    "free frees memory area for next memory area",
    test_free_frees_memory_area_for_next_memory_area) == NULL) return -1;
  if(CU_add_test(suite,
    "free frees memory area for next freed memory area",
    test_free_frees_memory_area_for_next_freed_memory_area) == NULL) return -1;
  if(CU_add_test(suite,
    "free frees memory area for previous memory area",
    test_free_frees_memory_area_for_previous_memory_area) == NULL) return -1;
  if(CU_add_test(suite,
    "free frees memory area for previous freed memory area",
    test_free_frees_memory_area_for_previous_freed_memory_area) == NULL) return -1;
  if(CU_add_test(suite,
    "free frees memory area",
    test_free_frees_memory_area) == NULL) return -1;
  if(CU_add_test(suite,
    "free frees random memory areas",
    test_free_frees_random_memory_areas) == NULL) return -1;
  if(CU_add_test(suite,
    "free unmaps memory area",
    test_free_unmaps_memory_area) == NULL) return -1;
  return 0;
}
