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
#include <limits.h>
#include <math.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <CUnit/Basic.h>
#include "uportlibc.h"

unsigned test_seed(void);

void heapsort(void *elems, size_t elem_count, size_t elem_size, int (*cmp)(const void *, const void *));
void introsort(void *elems, size_t l, size_t r, size_t elem_size, int (*cmp)(const void *, const void *), unsigned depth);

int initialize_qsort(void) { srand(test_seed()); return 0; }

int cleanup_qsort(void) { return 0; }

static int compare(const void *x, const void *y)
{ return *((const int *) x) - *((const int *) y); }

static void set_rand_qsort_arrays(int *in_array, int *out_array, size_t len)
{
  size_t i;
  for(i = 0; i < len; i++) {
    in_array[i] = floor((rand() / (RAND_MAX * 1.0)) * 1000000.0);
  }
  memcpy(out_array, in_array, len * sizeof(int));
  qsort(out_array, len, sizeof(int), compare);
}

static const int fixture_in_array1[8] = { 4, 5, 1, 7, 8, 9, 10, 5 };
static const int fixture_out_array1[8] = { 1, 4, 5, 5, 7, 8, 9, 10 };
static const int fixture_in_array2[10] = { 101, 14, 10, 50, 2, 1, 25, 10, 2, 8 };
static const int fixture_out_array2[10] = { 1, 2, 2, 8, 10, 10, 14, 25, 50, 101 };

static const int fixture_array1[5] = { 10, 8, 6, 4, 2 };
static const int fixture_array2[7] = { 1, 2, 1, 2, 1, 2, 1 };

static const int fixture_in_one_elem_array1[1] = { 1 };
static const int fixture_out_one_elem_array1[1] = { 1 };
static const int fixture_in_one_elem_array2[1] = { 10 };
static const int fixture_out_one_elem_array2[1] = { 10 };

static const int fixture_in_sorted_array1[9] = { 1, 4, 6, 7, 7, 8, 10, 15, 20 };
static const int fixture_out_sorted_array1[9] = { 1, 4, 6, 7, 7, 8, 10, 15, 20 };
static const int fixture_in_sorted_array2[12] = { 1, 2, 4, 10, 16, 19, 20, 22, 25, 30, 32, 36 };
static const int fixture_out_sorted_array2[12] = { 1, 2, 4, 10, 16, 19, 20, 22, 25, 30, 32, 36 };

static const int fixture_in_rev_sorted_array1[8] = { 20, 15, 14, 10, 9, 8, 4, 2 };
static const int fixture_out_rev_sorted_array1[8] = { 2, 4, 8, 9, 10, 14, 15, 20 };
static const int fixture_in_rev_sorted_array2[10] = { 100, 90, 80, 50, 50, 40, 30, 20, 20, 10 };
static const int fixture_out_rev_sorted_array2[10] = { 10, 20, 20, 30, 40, 50, 50, 80, 90, 100 };

void test_heapsort_sorts_array(void)
{
  static int array1[8], array2[10];
  memcpy(array1, fixture_in_array1, sizeof(fixture_in_array1));
  memcpy(array2, fixture_in_array2, sizeof(fixture_in_array2));
  heapsort(array1, sizeof(array1) / sizeof(int), sizeof(int), compare);
  CU_ASSERT(memcmp(array1, fixture_out_array1, sizeof(array1)) == 0);
  heapsort(array2, sizeof(array2) / sizeof(int), sizeof(int), compare);
  CU_ASSERT(memcmp(array2, fixture_out_array2, sizeof(array2)) == 0);
}

void test_heapsort_does_not_swap_elements_for_number_of_elements_that_is_zero(void)
{
  static int array1[5], array2[7];
  memcpy(array1, fixture_array1, sizeof(fixture_array1));
  memcpy(array2, fixture_array2, sizeof(fixture_array2));
  heapsort(array1 + 2, 0, sizeof(int), compare);
  CU_ASSERT(memcmp(array1, fixture_array1, sizeof(array1)) == 0);
  heapsort(array2 + 4, 0, sizeof(int), compare);
  CU_ASSERT(memcmp(array2, fixture_array2, sizeof(array2)) == 0);
}

void test_heapsort_sorts_array_of_one_element(void)
{
  static int array1[1], array2[1];
  memcpy(array1, fixture_in_one_elem_array1, sizeof(fixture_in_one_elem_array1));
  memcpy(array2, fixture_in_one_elem_array2, sizeof(fixture_in_one_elem_array2));
  heapsort(array1, sizeof(array1) / sizeof(int), sizeof(int), compare);
  CU_ASSERT(memcmp(array1, fixture_out_one_elem_array1, sizeof(array1)) == 0);
  heapsort(array2, sizeof(array2) / sizeof(int), sizeof(int), compare);
  CU_ASSERT(memcmp(array2, fixture_out_one_elem_array2, sizeof(array2)) == 0);
}

void test_heapsort_sorts_sorted_array(void)
{
  static int array1[9], array2[12];
  memcpy(array1, fixture_in_sorted_array1, sizeof(fixture_in_sorted_array1));
  memcpy(array2, fixture_in_sorted_array2, sizeof(fixture_in_sorted_array2));
  heapsort(array1, sizeof(array1) / sizeof(int), sizeof(int), compare);
  CU_ASSERT(memcmp(array1, fixture_out_sorted_array1, sizeof(array1)) == 0);
  heapsort(array2, sizeof(array2) / sizeof(int), sizeof(int), compare);
  CU_ASSERT(memcmp(array2, fixture_out_sorted_array2, sizeof(array2)) == 0);
}

void test_heapsort_sorts_reversely_sorted_array(void)
{
  static int array1[8], array2[10];
  memcpy(array1, fixture_in_rev_sorted_array1, sizeof(fixture_in_rev_sorted_array1));
  memcpy(array2, fixture_in_rev_sorted_array2, sizeof(fixture_in_rev_sorted_array2));
  heapsort(array1, sizeof(array1) / sizeof(int), sizeof(int), compare);
  CU_ASSERT(memcmp(array1, fixture_out_rev_sorted_array1, sizeof(array1)) == 0);
  heapsort(array2, sizeof(array2) / sizeof(int), sizeof(int), compare);
  CU_ASSERT(memcmp(array2, fixture_out_rev_sorted_array2, sizeof(array2)) == 0);
}

void test_heapsort_sorts_random_array(void)
{
  static int array1[1000], expected_array1[1000];
  static int array2[1000], expected_array2[1000];
  size_t len1 = floor(rand() / (RAND_MAX * 1.0) * 900.0) + 100;
  size_t len2 = floor(rand() / (RAND_MAX * 1.0) * 900.0) + 100;
  set_rand_qsort_arrays(array1, expected_array1, len1);
  set_rand_qsort_arrays(array2, expected_array2, len2);
  heapsort(array1, len1, sizeof(int), compare);
  CU_ASSERT(memcmp(array1, expected_array1, len1 * sizeof(int)) == 0);
  heapsort(array2, len2, sizeof(int), compare);
  CU_ASSERT(memcmp(array2, expected_array2, len2 * sizeof(int)) == 0);
}

void test_introsort_sorts_array_for_depth_that_is_uint_max(void)
{
  static int array1[8], array2[10];
  size_t i;
  memcpy(array1, fixture_in_array1, sizeof(fixture_in_array1));
  memcpy(array2, fixture_in_array2, sizeof(fixture_in_array2));
  introsort(array1, 0, sizeof(array1) / sizeof(int) - 1, sizeof(int), compare, UINT_MAX);
  CU_ASSERT(memcmp(array1, fixture_out_array1, sizeof(array1)) == 0);
  introsort(array2, 0, sizeof(array2) / sizeof(int) - 1, sizeof(int), compare, UINT_MAX);
  CU_ASSERT(memcmp(array2, fixture_out_array2, sizeof(array2)) == 0);
}

void test_introsort_sorts_array_of_one_element_for_depth_that_is_uint_max(void)
{
  static int array1[1], array2[1];
  memcpy(array1, fixture_in_one_elem_array1, sizeof(fixture_in_one_elem_array1));
  memcpy(array2, fixture_in_one_elem_array2, sizeof(fixture_in_one_elem_array2));
  introsort(array1, 0, sizeof(array1) / sizeof(int) - 1, sizeof(int), compare, UINT_MAX);
  CU_ASSERT(memcmp(array1, fixture_out_one_elem_array1, sizeof(array1)) == 0);
  introsort(array2, 0, sizeof(array2) / sizeof(int) - 1, sizeof(int), compare, UINT_MAX);
  CU_ASSERT(memcmp(array2, fixture_out_one_elem_array2, sizeof(array2)) == 0);
}

void test_introsort_sorts_sorted_array_for_depth_that_is_uint_max(void)
{
  static int array1[9], array2[12];
  memcpy(array1, fixture_in_sorted_array1, sizeof(fixture_in_sorted_array1));
  memcpy(array2, fixture_in_sorted_array2, sizeof(fixture_in_sorted_array2));
  introsort(array1, 0, sizeof(array1) / sizeof(int) - 1, sizeof(int), compare, UINT_MAX);
  CU_ASSERT(memcmp(array1, fixture_out_sorted_array1, sizeof(array1)) == 0);
  introsort(array2, 0, sizeof(array2) / sizeof(int) - 1, sizeof(int), compare, UINT_MAX);
  CU_ASSERT(memcmp(array2, fixture_out_sorted_array2, sizeof(array2)) == 0);
}

void test_introsort_sorts_reversely_sorted_array_for_depth_that_is_uint_max(void)
{
  static int array1[8], array2[10];
  memcpy(array1, fixture_in_rev_sorted_array1, sizeof(fixture_in_rev_sorted_array1));
  memcpy(array2, fixture_in_rev_sorted_array2, sizeof(fixture_in_rev_sorted_array2));
  introsort(array1, 0, sizeof(array1) / sizeof(int) - 1, sizeof(int), compare, UINT_MAX);
  CU_ASSERT(memcmp(array1, fixture_out_rev_sorted_array1, sizeof(array1)) == 0);
  introsort(array2, 0, sizeof(array2) / sizeof(int) - 1, sizeof(int), compare, UINT_MAX);
  CU_ASSERT(memcmp(array2, fixture_out_rev_sorted_array2, sizeof(array2)) == 0);
}

void test_introsort_sorts_random_array_for_depth_that_is_uint_max(void)
{
  static int array1[1000], expected_array1[1000];
  static int array2[1000], expected_array2[1000];
  size_t len1 = floor(rand() / (RAND_MAX * 1.0) * 900.0) + 100;
  size_t len2 = floor(rand() / (RAND_MAX * 1.0) * 900.0) + 100;
  set_rand_qsort_arrays(array1, expected_array1, len1);
  set_rand_qsort_arrays(array2, expected_array2, len2);
  introsort(array1, 0, len1 - 1, sizeof(int), compare, UINT_MAX);
  CU_ASSERT(memcmp(array1, expected_array1, len1 * sizeof(int)) == 0);
  introsort(array2, 0, len2 - 1, sizeof(int), compare, UINT_MAX);
  CU_ASSERT(memcmp(array2, expected_array2, len2 * sizeof(int)) == 0);
}

void test_qsort_sorts_array(void)
{
  static int array1[8], array2[10];
  memcpy(array1, fixture_in_array1, sizeof(fixture_in_array1));
  memcpy(array2, fixture_in_array2, sizeof(fixture_in_array2));
  uportlibc_qsort(array1, sizeof(array1) / sizeof(int), sizeof(int), compare);
  CU_ASSERT(memcmp(array1, fixture_out_array1, sizeof(array1)) == 0);
  uportlibc_qsort(array2, sizeof(array2) / sizeof(int), sizeof(int), compare);
  CU_ASSERT(memcmp(array2, fixture_out_array2, sizeof(array2)) == 0);
}

void test_qsort_does_not_swap_elements_for_number_of_elements_that_is_zero(void)
{
  static int array1[5], array2[7];
  memcpy(array1, fixture_array1, sizeof(fixture_array1));
  memcpy(array2, fixture_array2, sizeof(fixture_array2));
  uportlibc_qsort(array1 + 2, 0, sizeof(int), compare);
  CU_ASSERT(memcmp(array1, fixture_array1, sizeof(array1)) == 0);
  uportlibc_qsort(array2 + 4, 0, sizeof(int), compare);
  CU_ASSERT(memcmp(array2, fixture_array2, sizeof(array2)) == 0);
}

void test_qsort_sorts_array_of_one_element(void)
{
  static int array1[1], array2[1];
  memcpy(array1, fixture_in_one_elem_array1, sizeof(fixture_in_one_elem_array1));
  memcpy(array2, fixture_in_one_elem_array2, sizeof(fixture_in_one_elem_array2));
  uportlibc_qsort(array1, sizeof(array1) / sizeof(int), sizeof(int), compare);
  CU_ASSERT(memcmp(array1, fixture_out_one_elem_array1, sizeof(array1)) == 0);
  uportlibc_qsort(array2, sizeof(array2) / sizeof(int), sizeof(int), compare);
  CU_ASSERT(memcmp(array2, fixture_out_one_elem_array2, sizeof(array2)) == 0);
}

void test_qsort_sorts_sorted_array(void)
{
  static int array1[9], array2[12];
  memcpy(array1, fixture_in_sorted_array1, sizeof(fixture_in_sorted_array1));
  memcpy(array2, fixture_in_sorted_array2, sizeof(fixture_in_sorted_array2));
  uportlibc_qsort(array1, sizeof(array1) / sizeof(int), sizeof(int), compare);
  CU_ASSERT(memcmp(array1, fixture_out_sorted_array1, sizeof(array1)) == 0);
  uportlibc_qsort(array2, sizeof(array2) / sizeof(int), sizeof(int), compare);
  CU_ASSERT(memcmp(array2, fixture_out_sorted_array2, sizeof(array2)) == 0);
}

void test_qsort_sorts_reversely_sorted_array(void)
{
  static int array1[8], array2[10];
  memcpy(array1, fixture_in_rev_sorted_array1, sizeof(fixture_in_rev_sorted_array1));
  memcpy(array2, fixture_in_rev_sorted_array2, sizeof(fixture_in_rev_sorted_array2));
  uportlibc_qsort(array1, sizeof(array1) / sizeof(int), sizeof(int), compare);
  CU_ASSERT(memcmp(array1, fixture_out_rev_sorted_array1, sizeof(array1)) == 0);
  uportlibc_qsort(array2, sizeof(array2) / sizeof(int), sizeof(int), compare);
  CU_ASSERT(memcmp(array2, fixture_out_rev_sorted_array2, sizeof(array2)) == 0);
}

void test_qsort_sorts_random_array(void)
{
  static int array1[1000], expected_array1[1000];
  static int array2[1000], expected_array2[1000];
  size_t len1 = floor(rand() / (RAND_MAX * 1.0) * 900.0) + 100;
  size_t len2 = floor(rand() / (RAND_MAX * 1.0) * 900.0) + 100;
  set_rand_qsort_arrays(array1, expected_array1, len1);
  set_rand_qsort_arrays(array2, expected_array2, len2);
  uportlibc_qsort(array1, len1, sizeof(int), compare);
  CU_ASSERT(memcmp(array1, expected_array1, len1 * sizeof(int)) == 0);
  uportlibc_qsort(array2, len2, sizeof(int), compare);
  CU_ASSERT(memcmp(array2, expected_array2, len2 * sizeof(int)) == 0);
}

int add_qsort_suite(void)
{
  CU_pSuite suite;
  suite = CU_add_suite("qsort suite", initialize_qsort, cleanup_qsort);
  if(suite == NULL) return -1;
  if(CU_add_test(suite,
    "heapsort sorts array",
    test_heapsort_sorts_array) == NULL) return -1;
  if(CU_add_test(suite,
    "heapsort does not swap elements for number of elements that is zero",
    test_heapsort_does_not_swap_elements_for_number_of_elements_that_is_zero) == NULL) return -1;
  if(CU_add_test(suite,
    "heapsort sorts array of one element",
    test_heapsort_sorts_array_of_one_element) == NULL) return -1;
  if(CU_add_test(suite,
    "heapsort sorts sorted array",
    test_heapsort_sorts_sorted_array) == NULL) return -1;
  if(CU_add_test(suite,
    "heapsort sorts reversely sorted array",
    test_heapsort_sorts_reversely_sorted_array) == NULL) return -1;
  if(CU_add_test(suite,
    "heapsort sorts random array",
    test_heapsort_sorts_random_array) == NULL) return -1;
  if(CU_add_test(suite,
    "introsort sorts array for depth that is UINT_MAX",
    test_introsort_sorts_array_for_depth_that_is_uint_max) == NULL) return -1;
  if(CU_add_test(suite,
    "introsort sorts array of one element for depth that is UINT_MAX",
    test_introsort_sorts_array_of_one_element_for_depth_that_is_uint_max) == NULL) return -1;
  if(CU_add_test(suite,
    "introsort sorts sorted array for depth that is UINT_MAX",
    test_introsort_sorts_sorted_array_for_depth_that_is_uint_max) == NULL) return -1;
  if(CU_add_test(suite,
    "introsort sorts reversely sorted array for depth that is UINT_MAX",
    test_introsort_sorts_reversely_sorted_array_for_depth_that_is_uint_max) == NULL) return -1;
  if(CU_add_test(suite,
    "introsort sorts random array for depth that is UINT_MAX",
    test_introsort_sorts_random_array_for_depth_that_is_uint_max) == NULL) return -1;
  if(CU_add_test(suite,
    "qsort sorts array",
    test_qsort_sorts_array) == NULL) return -1;
  if(CU_add_test(suite,
    "qsort does not swap elements for number of elements that is zero",
    test_qsort_does_not_swap_elements_for_number_of_elements_that_is_zero) == NULL) return -1;
  if(CU_add_test(suite,
    "qsort sorts array of one element",
    test_qsort_sorts_array_of_one_element) == NULL) return -1;
  if(CU_add_test(suite,
    "qsort sorts sorted array",
    test_qsort_sorts_sorted_array) == NULL) return -1;
  if(CU_add_test(suite,
    "qsort sorts reversely sorted array",
    test_qsort_sorts_reversely_sorted_array) == NULL) return -1;
  if(CU_add_test(suite,
    "qsort sorts random array",
    test_qsort_sorts_random_array) == NULL) return -1;
  return 0;
}
