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
#include <locale.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <CUnit/Basic.h>

static unsigned static_test_seed;

unsigned test_seed(void) { return static_test_seed; }

int add_bsearch_suite(void);
int add_qsort_suite(void);
int add_stdio_suite(void);
int add_c_stdio_suite(void);
int add_w_stdio_suite(void);
int add_c_stdlib_suite(void);
int add_w_stdlib_suite(void);
int add_c_string_suite(void);
int add_w_string_suite(void);
int add_wchar_suite(void);

int main(int argc, char **argv)
{
  static_test_seed = (argc >= 2 ? strtoul(argv[1], NULL, 16) : time(NULL));
  printf("test seed: 0x%x\n", test_seed());
  printf("\n");
  setlocale(LC_ALL, "en_US.UTF-8");
  if(CU_initialize_registry() != CUE_SUCCESS) return CU_get_error();
  if(add_bsearch_suite() == -1) {
    CU_cleanup_registry();
    return CU_get_error();
  }
  if(add_qsort_suite() == -1) {
    CU_cleanup_registry();
    return CU_get_error();
  }
  if(add_stdio_suite() == -1) {
    CU_cleanup_registry();
    return CU_get_error();
  }
  if(add_c_stdio_suite() == -1) {
    CU_cleanup_registry();
    return CU_get_error();
  }
  if(add_w_stdio_suite() == -1) {
    CU_cleanup_registry();
    return CU_get_error();
  }
  if(add_c_stdlib_suite() == -1) {
    CU_cleanup_registry();
    return CU_get_error();
  }
  if(add_w_stdlib_suite() == -1) {
    CU_cleanup_registry();
    return CU_get_error();
  }
  if(add_c_string_suite() == -1) {
    CU_cleanup_registry();
    return CU_get_error();
  }
  if(add_w_string_suite() == -1) {
    CU_cleanup_registry();
    return CU_get_error();
  }
  if(add_wchar_suite() == -1) {
    CU_cleanup_registry();
    return CU_get_error();
  }
  CU_basic_set_mode(CU_BRM_VERBOSE);
  CU_basic_run_tests();
  CU_cleanup_registry();
  printf("\n");
  printf("test seed: 0x%x\n", test_seed());
  return CU_get_error();
}
