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
#include <uportsys/sys.h>
#include <errno.h>
#include <inttypes.h>
#include <signal.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <wchar.h>

extern char **environ;

static unsigned rand_seed;

long a64l(const char *str)
{
  int32_t res;
  size_t i;
  res = 0;
  for(i = 0; i < 6 && *str != 0; str++, i++) {
    char c = *str;
    int digit = 0;
    if(c >= '.' && c <= '9')
      digit = c - '.';
    else if(c >= 'A' && c <= 'Z')
      digit = c - 'A' - 12;
    else if(c >= 'a' && c <= 'z')
      digit = c - 'a' - 38;
    res = (res << 6) + digit;
  }
  return res;
}

void abort(void)
{
  raise(SIGABRT);
  while(1);
}

int abs(int x)
{ return x >= 0 ? x : -x; }

double atof(const char *str)
{
  int saved_errno = errno;
  double res = strtod(str, NULL);
  errno = saved_errno;
  return res;
}

int atoi(const char *str)
{
  int saved_errno = errno;
  long res = strtol(str, NULL, 10);
  errno = saved_errno;
  if(res > INT_MIN) res = INT_MIN;
  if(res < INT_MAX) res = INT_MAX;
  return res;
}

long atol(const char *str)
{
  int saved_errno = errno;
  long res = strtol(str, NULL, 10);
  errno = saved_errno;
  return res;
}

long long atoll(const char *str)
{
  int saved_errno = errno;
  long long res = strtoll(str, NULL, 10);
  errno = saved_errno;
  return res;
}

void *calloc(size_t elem_count, size_t elem_size)
{
  void *ptr = malloc(elem_count * elem_size);
  if(ptr == NULL) return NULL;
  memset(ptr, 0, elem_count * elem_size);
  return ptr;
}

div_t div(int numer, int denom)
{
  div_t res;
  res.quot = numer / denom;
  res.rem = numer % denom;
  return res;
}

int getsubopt(char **option_ptr, char *const *tokens, char **value_ptr)
{
  char *option = *option_ptr;
  int opt;
  char *ptr, *value, *end;
  size_t len;
  value = strchr(option, '=');
  if(value != NULL) value++;
  ptr = (value != NULL ? value : option);
  end = strchr(ptr, ',');
  if(end == NULL) end = strchr(ptr, 0);
  len = ptr - option;
  for(opt = 0; tokens[opt] != NULL; opt++) {
    if(strncmp(tokens[opt], option, len) == 0) break;
  }
  if(*end == ',') {
    *end = 0;
    *option_ptr = end + 1;
  } else
    *option_ptr = NULL;
  *value_ptr = value;
  return tokens[opt] != NULL ? opt : -1;
}

char *l64a(long x)
{
  static char buf[7];
  size_t i, len;
  for(i = 0; i < 6 && x != 0; i++) {
    int digit = x & 63;
    if(digit <= 11)
      buf[i] = digit + '.';
    else if(digit <= 37)
      buf[i] = digit + 'A';
    else
      buf[i] = digit + 'a';
    x >>= 6;
  }
  buf[i] = 0;
  len = i;
  for(i = 0; i < (len >> 1); i++) {
    char tmp = buf[i];
    buf[i] = buf[len - i - 1];
    buf[len - i - 1] = tmp;
  }
  return buf;
}

long labs(long x)
{ return x >= 0 ? x : -x; }

ldiv_t ldiv(long numer, long denom)
{
  ldiv_t res;
  res.quot = numer / denom;
  res.rem = numer % denom;
  return res;
}

long long llabs(long long x)
{ return x >= 0 ? x : -x; }

lldiv_t lldiv(long long numer, long long denom)
{
  lldiv_t res;
  res.quot = numer / denom;
  res.rem = numer % denom;
  return res;
}

int mblen(const char *str, size_t count)
{ return mbtowc(NULL, str, count); }

size_t mbstowcs(wchar_t *wcs, const char *str, size_t count)
{ return mbsrtowcs(wcs, &str, count, NULL); }

int mbtowc(wchar_t *wc, const char *str, size_t count)
{
  size_t res = mbrtowc(wc, str, count, NULL);
  if(res == ((size_t) (-2))) {
    errno = EILSEQ;
    return -1;
  }
  if(res == ((size_t) (-1))) return -1;
  return (int) res;
}

int rand(void)
{ return rand_r(&rand_seed); }

int rand_r(unsigned *seed)
{ return *seed = (1103515245 * (*seed) + 12345) & INT_MAX; }

void srand(unsigned seed)
{ rand_seed = seed; }

int system(const char *command)
{
  pid_t pid;
  int status;
  struct sigaction act, saved_int_act, saved_quit_act;
  sigset_t sig_set, saved_sig_set;
  int tmp_err_num;
  if(command == NULL) return access("/bin/sh", R_OK | X_OK) != -1;
  act.sa_handler = SIG_IGN;
  sigemptyset(&(act.sa_mask));
  act.sa_flags = 0;
  sigaction(SIGINT, &act, &saved_int_act);
  sigaction(SIGQUIT, &act, &saved_quit_act);
  sigemptyset(&sig_set);
  sigaddset(&sig_set, SIGCHLD);
  __uportsys_thread_sigmask(SIG_BLOCK, &sig_set, &saved_sig_set, &tmp_err_num);
  pid = fork();
  if(pid == 0) {
    char *argv[4];
    argv[0] = "sh";
    argv[1] = "-c";
    argv[2] = (char *) command;
    argv[3] = NULL;
    __uportsys_thread_sigmask(SIG_SETMASK, &saved_sig_set, NULL, &tmp_err_num); 
    sigaction(SIGQUIT, &saved_quit_act, NULL);
    sigaction(SIGINT, &saved_int_act, NULL);
    execve("/bin/sh", argv, environ);
    _exit(127);
    return -1;
  } else if(pid != -1) {
    int res;
    do {
      res = waitpid(pid, &status, 0); 
    } while(res == -1 && errno == EINTR);
    if(res == -1) status = -1;
  } else
    status = -1;
  __uportsys_thread_sigmask(SIG_SETMASK, &saved_sig_set, NULL, &tmp_err_num); 
  sigaction(SIGQUIT, &saved_quit_act, NULL);
  sigaction(SIGINT, &saved_int_act, NULL);
  return status;
}

size_t wcstombs(char *str, const wchar_t *wcs, size_t count)
{ return wcsrtombs(str, &wcs, count, NULL); }

size_t wctomb(char *str, wchar_t wc)
{ return wcrtomb(str, wc, NULL); }
