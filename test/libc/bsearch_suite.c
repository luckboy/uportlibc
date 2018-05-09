/*
 * Copyright (c) 2016, 2018 Łukasz Szpakowski
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
#include <math.h>
#include <stddef.h>
#include <stdlib.h>
#include <CUnit/Basic.h>
#include "uportlibc.h"

unsigned test_seed(void);

int initialize_bsearch(void) { srand(test_seed()); return 0; }

int cleanup_bsearch(void) { return 0; }

static int compare(const void *x, const void *y)
{ return *((const int *) x) - *((const int *) y); }

static void set_rand_bsearch_array(int *array, size_t len)
{
  size_t i;
  int value = 0;
  for(i = 0; i < len; i++) {
    value += floor((rand() / (RAND_MAX * 1.0)) * 100.0) + 1;
    array[i] = value;
  }
}

static void set_rand_bsearch_array_without_key(int *array, size_t len, size_t key_idx, int *key)
{
  size_t i;
  int value = 0;
  for(i = 0; i < len; i++) {
    if(key_idx != i || key_idx == 0) {
      value += floor((rand() / (RAND_MAX * 1.0)) * 100.0) + 1;
    } else {
      int inc = floor((rand() / (RAND_MAX * 1.0)) * 99.0) + 2; 
      *key = value + floor((rand() / (RAND_MAX * 1.0)) * (inc - 2)) + 1;
      value += inc;
    }
    array[i] = value;
  }
  if(key_idx == 0)
    *key = array[0] - floor((rand() / (RAND_MAX * 1.0)) * 100.0) - 1;
  if(key_idx == len)
    *key = array[len - 1] + floor((rand() / (RAND_MAX * 1.0)) * 100.0) + 1;
}

void test_bsearch_returns_pointer_to_found_element(void)
{
  static int array1[4] = { 1, 4, 6, 9 };
  int key1 = 4;
  static int array2[7] = { 100, 200, 300, 400, 500, 600, 700 };
  int key2 = 500;
  void *res1, *res2;
  res1 = uportlibc_bsearch(&key1, array1, sizeof(array1) / sizeof(int), sizeof(int), compare);
  CU_ASSERT_PTR_EQUAL(res1, (void *) (&(array1[1])));
  res2 = uportlibc_bsearch(&key2, array2, sizeof(array2) / sizeof(int), sizeof(int), compare);
  CU_ASSERT_PTR_EQUAL(res2, (void *) (&(array2[4])));
}

void test_bsearch_returns_null_pointer_for_key_that_does_not_occur(void)
{
  static int array1[6] = { -1, 2, 5, 6, 10, 14 };
  int key1 = 3;
  static int array2[5] = { 0, 10, 20, 30, 40 };
  int key2 = 15;
  void *res1, *res2;
  res1 = uportlibc_bsearch(&key1, array1, sizeof(array1) / sizeof(int), sizeof(int), compare);
  CU_ASSERT_PTR_NULL(res1);
  res2 = uportlibc_bsearch(&key2, array2, sizeof(array2) / sizeof(int), sizeof(int), compare);
  CU_ASSERT_PTR_NULL(res2);
}

void test_bsearch_returns_null_pointer_for_empty_array(void)
{
  static int array[1] = { 1 };
  int key = 1;
  void *res;
  res = uportlibc_bsearch(&key, array, 0, sizeof(int), compare);
  CU_ASSERT_PTR_NULL(res);
}

void test_bsearch_returns_pointer_to_found_element_for_array_of_one_element_with_key(void)
{
  static int array1[1] = { 1 };
  int key1 = 1;
  static int array2[1] = { 2 };
  int key2 = 2;
  void *res1, *res2;
  res1 = uportlibc_bsearch(&key1, array1, sizeof(array1) / sizeof(int), sizeof(int), compare);
  CU_ASSERT_PTR_EQUAL(res1, (void *) (&(array1[0])));
  res2 = uportlibc_bsearch(&key2, array2, sizeof(array2) / sizeof(int), sizeof(int), compare);
  CU_ASSERT_PTR_EQUAL(res2, (void *) (&(array2[0])));
}

void test_bsearch_returns_null_pointer_for_array_of_one_element_without_key(void)
{
  static int array1[1] = { 1 };
  int key1 = 2;
  static int array2[1] = { 2 };
  int key2 = 1;
  void *res1, *res2;
  res1 = uportlibc_bsearch(&key1, array1, sizeof(array1) / sizeof(int), sizeof(int), compare);
  CU_ASSERT_PTR_NULL(res1);
  res2 = uportlibc_bsearch(&key2, array2, sizeof(array2) / sizeof(int), sizeof(int), compare);
  CU_ASSERT_PTR_NULL(res2);
}

void test_bsearch_returns_null_pointer_for_key_less_than_first_element(void)
{
  static int array1[5] = { 2, 5, 6, 7, 8 };
  int key1 = 1;
  static int array2[6] = { -2, -1, 6, 7, 8, 10 };
  int key2 = -10;
  void *res1, *res2;
  res1 = uportlibc_bsearch(&key1, array1, sizeof(array1) / sizeof(int), sizeof(int), compare);
  CU_ASSERT_PTR_NULL(res1);
  res2 = uportlibc_bsearch(&key2, array2, sizeof(array2) / sizeof(int), sizeof(int), compare);
  CU_ASSERT_PTR_NULL(res2);
}

void test_bsearch_returns_null_pointer_for_key_greater_than_last_element(void)
{
  static int array1[5] = { 1, 5, 6, 7, 9 };
  int key1 = 10;
  static int array2[10] = { 2, 4, 5, 6, 8, 10, 15, 16 };
  int key2 = 20;
  void *res1, *res2;
  res1 = uportlibc_bsearch(&key1, array1, sizeof(array1) / sizeof(int), sizeof(int), compare);
  CU_ASSERT_PTR_NULL(res1);
  res2 = uportlibc_bsearch(&key2, array2, sizeof(array2) / sizeof(int), sizeof(int), compare);
  CU_ASSERT_PTR_NULL(res2);
}

void test_bsearch_returns_pointer_to_found_element_for_random_array_with_key(void)
{
  static int array1[1000], array2[1000];
  int key1, key2;
  void *res1, *res2;
  size_t len1 = floor(rand() / (RAND_MAX * 1.0) * 900) + 100;
  size_t len2 = floor(rand() / (RAND_MAX * 1.0) * 900) + 100;
  size_t i1 = floor((rand() / (RAND_MAX * 1.0)) * (len1 - 1));
  size_t i2 = floor((rand() / (RAND_MAX * 1.0)) * (len2 - 1));
  set_rand_bsearch_array(array1, len1);
  set_rand_bsearch_array(array2, len2);
  key1 = array1[i1];
  key2 = array2[i2];
  res1 = uportlibc_bsearch(&key1, array1, len1, sizeof(int), compare);
  CU_ASSERT_PTR_EQUAL(res1, (void *) (&(array1[i1])));
  res2 = uportlibc_bsearch(&key2, array2, len2, sizeof(int), compare);
  CU_ASSERT_PTR_EQUAL(res2, (void *) (&(array2[i2])));
}

void test_bsearch_returns_null_pointer_for_random_array_without_key(void)
{
  static int array1[1000], array2[1000];
  int key1, key2;
  void *res1, *res2;
  size_t len1 = floor(rand() / (RAND_MAX * 1.0) * 900.0) + 100;
  size_t len2 = floor(rand() / (RAND_MAX * 1.0) * 900.0) + 100;
  size_t i1 = floor((rand() / (RAND_MAX * 1.0)) * len1);
  size_t i2 = floor((rand() / (RAND_MAX * 1.0)) * len2);
  set_rand_bsearch_array_without_key(array1, len1, i1, &key1);
  set_rand_bsearch_array_without_key(array2, len2, i2, &key2);
  res1 = uportlibc_bsearch(&key1, array1, len1, sizeof(int), compare);
  CU_ASSERT_PTR_NULL(res1);
  res2 = uportlibc_bsearch(&key2, array2, len2, sizeof(int), compare);
  CU_ASSERT_PTR_NULL(res2);
}

int add_bsearch_suite(void)
{
  CU_pSuite suite;
  suite = CU_add_suite("bsearch suite", initialize_bsearch, cleanup_bsearch);
  if(suite == NULL) return -1;
  if(CU_add_test(suite,
    "bsearch returns pointer to found element",
    test_bsearch_returns_pointer_to_found_element) == NULL) return -1;
  if(CU_add_test(suite,
    "bsearch returns null pointer for key that does not occur",
    test_bsearch_returns_null_pointer_for_key_that_does_not_occur) == NULL) return -1;
  if(CU_add_test(suite,
    "bsearch returns null pointer for empty array",
    test_bsearch_returns_null_pointer_for_empty_array) == NULL) return -1;
  if(CU_add_test(suite,
    "bsearch returns pointer for array of one element with key",
    test_bsearch_returns_pointer_to_found_element_for_array_of_one_element_with_key) == NULL) return -1;
  if(CU_add_test(suite,
    "bsearch returns null pointer for array of one element without key",
    test_bsearch_returns_null_pointer_for_array_of_one_element_without_key) == NULL) return -1;
  if(CU_add_test(suite,
    "bsearch returns null pointer for key less than first element",
    test_bsearch_returns_null_pointer_for_key_less_than_first_element) == NULL) return -1;
  if(CU_add_test(suite,
    "bsearch returns null pointer for key greater than last element",
    test_bsearch_returns_null_pointer_for_key_greater_than_last_element) == NULL) return -1;
  if(CU_add_test(suite,
    "bsearch returns pointer to found element for random array with key",
    test_bsearch_returns_pointer_to_found_element_for_random_array_with_key) == NULL) return -1;
  if(CU_add_test(suite,
    "bsearch returns null pointer for random array without key",
    test_bsearch_returns_null_pointer_for_random_array_without_key) == NULL) return -1;
  return 0;
}
