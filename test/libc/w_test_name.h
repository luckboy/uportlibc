/*
 * Copyright (c) 2016-2017 Łukasz Szpakowski
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
#ifndef __W_UNDEF
#ifdef __W
#if __W == 'c'
#define __W_L(x)                                x
#define __W_TEST_NAME(prefix, suffix, x)        test_##prefix##suffix##x
#define __W_TEST_STR_NAME(suffix, x)            test_str##suffix##x
#define __W_TEST_MEM_NAME(suffix, x)            test_mem##suffix##x
#define __W_TEST_STRTOK_R_NAME(x)               test_strtok_r##x
#define __W_TEST_SNPRINTF_NAME(x)               test_snprintf##x
#define __W_TEST_STRING(prefix, suffix, str)    #prefix #suffix str
#define __W_TEST_STR_STRING(suffix, str)        "str" #suffix str
#define __W_TEST_MEM_STRING(suffix, str)        "mem" #suffix str
#define __W_TEST_STRTOK_R_STRING(str)           "strtok_r" str
#define __W_TEST_SNPRINTF_STRING(str)           "snprintf" str
#define __W_INITIALIZE_NAME(name)               initialize_c_##name
#define __W_CLEANUP_NAME(name)                  cleanup_c_##name
#define __W_ADD_SUITE_NAME(name)                add_c_##name##_suite
#define __W_SUITE_STRING(name)                  "c_" #name " suite"
#define __W_NATIVE_MEMCMP                       memcmp
#define __W_NATIVE_MEMCPY                       memcpy
#define __W_NATIVE_MEMSET                       memset
#define __W_NATIVE_STRCPY                       strcpy
#else
#if __W == 'w'
#define __W_L(x)                                L##x
#define __W_TEST_NAME(prefix, suffix, x)        test_##prefix##w##suffix##x
#define __W_TEST_STR_NAME(suffix, x)            test_wcs##suffix##x
#define __W_TEST_MEM_NAME(suffix, x)            test_wmem##suffix##x
#define __W_TEST_STRTOK_R_NAME(x)               test_wcstok##x
#define __W_TEST_SNPRINTF_NAME(x)               test_wsprintf##x
#define __W_TEST_STRING(prefix, suffix, str)    #prefix "w" #suffix str
#define __W_TEST_STR_STRING(suffix, str)        "wcs" #suffix str
#define __W_TEST_MEM_STRING(suffix, str)        "wmem" #suffix str
#define __W_TEST_STRTOK_R_STRING(str)           "wcstok" str
#define __W_TEST_SNPRINTF_STRING(str)           "wsprintf" str
#define __W_INITIALIZE_NAME(name)               initialize_w_##name
#define __W_CLEANUP_NAME(name)                  cleanup_w_##name
#define __W_ADD_SUITE_NAME(name)                add_w_##name##_suite
#define __W_SUITE_STRING(name)                  "w_" #name " suite"
#define __W_NATIVE_MEMCMP                       wmemcmp
#define __W_NATIVE_MEMCPY                       wmemcpy
#define __W_NATIVE_MEMSET                       wmemset
#define __W_NATIVE_STRCPY                       wcscpy
#else
#error "Incorrect macro __W."
#endif
#endif
#else
#error "Undefined macro __W."
#endif
#else
#undef __W_
#undef __W_TEST_NAME
#undef __W_TEST_STR_NAME
#undef __W_TEST_MEM_NAME
#undef __W_TEST_STRTOK_R_NAME
#undef __W_TEST_STRING
#undef __W_TEST_STR_STRING
#undef __W_TEST_MEM_STRING
#undef __W_TEST_STRTOK_R_STRING
#undef __W_INITIALIZE_NAME
#undef __W_CLEANUP_NAME
#undef __W_ADD_SUITE_NAME
#undef __W_SUITE_STRING
#undef __W_NATIVE_MEMCMP
#undef __W_NATIVE_MEMCPY
#undef __W_NATIVE_MEMSET
#undef __W_NATIVE_STRCPY
#endif
