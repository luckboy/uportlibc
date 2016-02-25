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
#ifndef _STDLIB_H
#define _STDLIB_H

#include <sys/wait.h>
#define __W 'c'
#include <uportlibc/w_stdlib.h>
#include <limits.h>

#define EXIT_FAILURE            1
#define EXIT_SUCCESS            0
#define MB_CUR_MAX              MB_LEN_MAX
#define RAND_MAX                INT_MAX

#ifdef __cplusplus
extern "C" {
#endif

typedef struct
{
  int quot;
  int rem;
} div_t;

typedef struct
{
  long quot;
  long rem;
} ldiv_t;

typedef struct
{
  long long quot;
  long long rem;
} lldiv_t;

/* System functions. */

int grantpt(int fd);
char *ptsname(int fd);
int unlockpt(int fd);

/* Functions from the wchar.h header. */

int mblen(const char *str, size_t count);
int mbstowcs(wchar_t *wcs, const char *str, size_t count);
int mbtowc(wchar_t *wc, const char *str, size_t count);
size_t wcstombs(char *str, const wchar_t *wcs, size_t count);
size_t wctomb(char *str, wchar_t wc);

/* Other functions. */

void abort(void);
int abs(int x);
int atexit(void (*fun)(void));
double atof(const char *str);
int atoi(const char *str);
long atol(const char *str);
long long atoll(const char *str);
void *bsearch(const void *key, const void *elems, size_t elem_count, size_t elem_size, int (*cmp)(const void *, const void *));
void *calloc(size_t elem_count, size_t elem_size);
div_t div(int numer, int demon);
void exit(int status);
char *getenv(const char *name);
int getsubopt(const char **option_ptr, char *const *tokens, char *value_ptr);
void free(void *ptr);
long labs(long x);
ldiv_t ldiv(long numer, long denom);
long long llabs(long long x);
lldiv_t lldiv(long long numer, long long denom);
void *malloc(size_t size);
int rand(void);
int rand_r(unsigned *seed);
void *realloc(void *ptr, size_t size);
char *realpath(const char *path_name, char *resolved_path_name);
void qsort(void *elems, size_t elem_count, size_t elem_size, int (*cmp)(const void *, const void *));
void srand(unsigned seed);
int system(const char *command);

#ifdef __cplusplus
}
#endif

#endif
