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
#include <limits.h>
#include <wchar.h>
#include "stdio_priv.h"

wint_t btowc(int c)
{ return (c >= 0 && c <= 0x7f) ? c : WEOF; }

int fwide(FILE *stream, int mode)
{
  int res;
  lock_lock(&(stream->lock));
  if(mode != 0) {
    if(mode < 0 && stream->wide_mode == 0) stream->wide_mode = -1;
    if(mode > 0 && stream->wide_mode == 0) stream->wide_mode = 1;
  }
  res = stream->wide_mode;
  lock_unlock(&(stream->lock));
  return res;
}

size_t mbrlen(const char *str, size_t count, mbstate_t *state)
{ return mbrtowc(NULL, str, count, state); }

size_t mbrtowc(wchar_t *wc, const char *str, size_t count, mbstate_t *state)
{
  static mbstate_t static_state = { 0, 0 };
  const char *end = str + count;
  size_t len = 0;
  if(str == NULL) str = "";
  if(state == NULL) state = &static_state;
  if(state->count >= MB_LEN_MAX || state->wc < 0 || state->wc > (0x10ffff >> (state->count * 6))) {
    errno = EINVAL;
    return (size_t) (-1);
  }
  if(state->count == 0) {
    if((*str & 0x80) == 0x00) {
      state->wc = *str & 0x7f;
    } else if((*str & 0xe0) == 0xc0) {
      state->count = 1;
      state->wc = *str & 0x1f;
    } else if((*str & 0xf0) == 0xe0) {
      state->count = 2;
      state->wc = *str & 0x0f;
    } else if((*str & 0xf8) == 0xf0) {
      state->count = 3;
      state->wc = *str & 0x07;
    } else {
      errno = EILSEQ;
      return (size_t) (-1);
    }
    if(state->wc == 0 && state->count > 1) {
      errno = EILSEQ;
      return (size_t) (-1);
    }
    str++;
    len++;
  }
  for(; str != end && *str != 0 && state->count != 0; str++, len++) {
    if((*str & 0xc0) != 0x80) {
      errno = EILSEQ;
      return (size_t) (-1);
    }
    state->wc = (state->wc << 6) | (*str & 0x3f);
    state->count--;
  }
  if(state->count == 0) {
    if(wc != NULL) *wc = state->wc;
    state->wc = 0;
    return state->wc != 0 ? len : 0;
  } else
    return (size_t) (-2);
}

int mbsinit(const mbstate_t *state)
{ return state == NULL || (state->count == 0 && state->wc == 0); }

size_t mbsrtowcs(wchar_t *wcs, const char **str, size_t count, mbstate_t *state)
{
  const char *ptr = *str;
  size_t i;
  for(i = 0; i < count; i++) {
    size_t res = mbrtowc((wcs != NULL ? wcs + i : NULL), ptr, ULONG_MAX, state);
    if(res == ((size_t) (-1))) return (size_t) (-1);
    if(*ptr == 0) break;
    ptr += res;
  }
  if(wcs != NULL) *str = (*ptr != 0 ? ptr : NULL);
  return i;
}

size_t wcrtomb(char *str, wchar_t wc, mbstate_t *state)
{
  if(wc >=0 && wc <= 0x7f) {
    if(str != NULL) str[0] = wc;
    return 1;
  } else if(wc >= 0x80 && wc <= 0x7ff) {
    if(str != NULL) {
      str[0] = (wc >> 6) | 0xc0;
      str[1] = ((wc >> 0) & 0x3f) | 0x80;
    }
    return 2;
  } else if(wc >= 0x800 && wc <= 0xffff) {
    if(str != NULL) {
      str[0] = (wc >> 12) | 0xe0;
      str[1] = ((wc >> 6) & 0x3f) | 0x80;
      str[2] = ((wc >> 0) & 0x3f) | 0x80;
    }
    return 3;
  } else if(wc >= 0x10000 && wc <= 0x10ffff) {
    if(str != NULL) {
      str[0] = (wc >> 18) | 0xf0;
      str[1] = ((wc >> 12) & 0x3f) | 0x80;
      str[2] = ((wc >> 6) & 0x3f) | 0x80;
      str[3] = ((wc >> 0) & 0x3f) | 0x80;
    }
    return 4;
  } else {
    errno = EILSEQ;
    return (size_t) (-1);
  }
}

size_t wcsrtombs(char *str, const wchar_t **wcs, size_t count, mbstate_t *state)
{
  const wchar_t *ptr = *wcs;
  size_t i;
  for(i = 0; i < count; ptr++) {
    size_t len = wcrtomb(NULL, *ptr, state);
    if(len == ((size_t) (-1))) return (size_t) (-1);
    if(i + len > count) break;
    if(str != NULL) wcrtomb(str + i, *ptr, state);
    if(*ptr == 0) break;
    i += len;
  }
  if(str != NULL) *wcs = (*ptr != 0 ? ptr : NULL);
  return i;
}
