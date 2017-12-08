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
#ifndef TEST
#define __W W
#include <uportlibc/w_ctype.h>
#else
#include <ctype.h>
#include <wctype.h>
#endif
#include <ctype.h>
#include <errno.h>
#include <float.h>
#include <inttypes.h>
#include <limits.h>
#include <math.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include <strings.h>
#include <wchar.h>
#ifndef TEST
#include "conv.h"
#else
#define UPORTLIBC_CONV
#include "uportlibc.h"
#define __W W
#include "w_uportlibc.h"
#endif
#include "float_priv.h"
#define __W W
#include "w_format.h"
#define __W W
#include "w_vxscanf.h"
#ifndef TEST
#define __W W
#include <uportlibc/w_name.h>
#else
#define __W W
#include "w_uportlibc_name.h"
#endif

#ifndef LDBL_MAX
#if __SIZEOF_LONG_DOUBLE__ == __SIZEOF_FLOAT__
#define LDBL_MAX                3.40282346638528859812e38L
#else
#if __SIZEOF_LONG_DOUBLE__ == __SIZEOF_DOUBLE__
#define LDBL_MAX                1.79769313486231570815e308L
#else
#define LDBL_MAX                1.18973149535723176502e4932L
#endif
#endif
#endif

#if __W == 'c'
#define __W_ISSPACE             isspace
#else
#if __W == 'w'
#define __W_ISSPACE             iswspace
#else
#error "Incorrect macro __W."
#endif
#endif

#define LENGTH_CHAR                 1
#define LENGTH_SHORT                2
#define LENGTH_LONG                 3
#define LENGTH_LONG_LONG            4
#define LENGTH_LONG_DOUBLE          5

#define ARG_TYPE_UNDEF              0
#define ARG_TYPE_CHAR_PTR           1
#define ARG_TYPE_SCHAR_PTR          2
#define ARG_TYPE_SHORT_PTR          3
#define ARG_TYPE_INT_PTR            4
#define ARG_TYPE_LONG_PTR           5
#define ARG_TYPE_LONG_LONG_PTR      6
#define ARG_TYPE_UCHAR_PTR          7
#define ARG_TYPE_USHORT_PTR         8
#define ARG_TYPE_UINT_PTR           9
#define ARG_TYPE_ULONG_PTR          10
#define ARG_TYPE_ULONG_LONG_PTR     11
#define ARG_TYPE_FLOAT_PTR          12
#define ARG_TYPE_DOUBLE_PTR         13
#define ARG_TYPE_LONG_DOUBLE_PTR    14
#define ARG_TYPE_WCHAR_T_PTR        15
#define ARG_TYPE_VOID_PTR_PTR       16

struct conv_spec
{
  int arg_idx;
  long max_width;
  unsigned length;
  __W_CONST_CHAR_PTR set;
  size_t set_size;
  __W_CHAR conv_c;
};

union arg_value
{
  char *cp;
  signed char *scp;
  short *sp;
  int *ip;
  long *lp;
  long long *llp;
  unsigned char *ucp;
  unsigned short *usp;
  unsigned *uip;
  unsigned long *ulp;
  unsigned long long *ullp;
  float *fp;
  double *dp;
  long double *ldp;
  wchar_t *wcp;
  void **vpp;
};

static int __W_NAME(, parse_conv_spec)(__W_CONST_CHAR_PTR *format_ptr, struct conv_spec *spec, unsigned char *arg_types, unsigned *curr_arg_idx_ptr, unsigned *arg_count_ptr)
{
  __W_CONST_CHAR_PTR format = *format_ptr;
  unsigned curr_arg_idx = *curr_arg_idx_ptr;
  unsigned arg_count = *arg_count_ptr;
  if(*format == '%') {
    format++;
    if(*format != '%') {
      unsigned new_arg_type;
      int value;
      __W_CONST_CHAR_PTR tmp_format = format;
      __W_CONST_CHAR_PTR arg_pos_format = tmp_format;
      for(; *tmp_format >= '0' && *tmp_format < '9'; tmp_format++);
      if(*tmp_format == '$') format = tmp_format + 1;
      /* Parses a star. */
      if(*format == '*') {
        spec->arg_idx = -1;
        format++;
      } else {
        int arg_idx = __W_UPORTLIBC_NAME(, parse_arg_pos)(&arg_pos_format, &curr_arg_idx, &arg_count);
        if(arg_idx == -1) return -1;
        spec->arg_idx = arg_idx;
      }
      /* Parses a maximum width. */
      if(*format >= '0' && *format <= '9') {
        value = __W_UPORTLIBC_NAME(, parse_conv_spec_num)(&format);
        if(value == -1) return -1;
        spec->max_width = value;
      } else
        spec->max_width = -1;
      /* Parses a length. */
      spec->length = 0;
      switch(*format) {
      case 'h':
        if(format[1] == 'h') {
          spec->length = LENGTH_CHAR;
          format += 2;
        } else {
          spec->length = LENGTH_SHORT;
          format++;
        }
        break;
      case 'l':
        if(format[1] == 'l') {
          spec->length = LENGTH_LONG_LONG;
          format += 2;
        } else {
          spec->length = LENGTH_LONG;
          format++;
        }
        break;
      case 'L':
        spec->length = LENGTH_LONG_DOUBLE;
        format++;
        break;
      }
      /* Parses a conversion character. */
      spec->conv_c = *format;
      spec->set = NULL;
      spec->set_size = 0;
      switch(*format) {
      case 'd':
      case 'i':
        switch(spec->length) {
        case 0:
          new_arg_type = ARG_TYPE_INT_PTR;
          break;
        case LENGTH_CHAR:
          new_arg_type = ARG_TYPE_SCHAR_PTR;
          break;
        case LENGTH_SHORT:
          new_arg_type = ARG_TYPE_SHORT_PTR;
          break;
        case LENGTH_LONG:
          new_arg_type = ARG_TYPE_LONG_PTR;
          break;
        case LENGTH_LONG_LONG:
          new_arg_type = ARG_TYPE_LONG_LONG_PTR;
          break;
        default:
          errno = EINVAL;
          return -1;
        }
      case 'u':
      case 'o':
      case 'X':
      case 'x':
        switch(spec->length) {
        case 0:
          new_arg_type = ARG_TYPE_UINT_PTR;
          break;
        case LENGTH_CHAR:
          new_arg_type = ARG_TYPE_UCHAR_PTR;
          break;
        case LENGTH_SHORT:
          new_arg_type = ARG_TYPE_USHORT_PTR;
          break;
        case LENGTH_LONG:
          new_arg_type = ARG_TYPE_ULONG_PTR;
          break;
        case LENGTH_LONG_LONG:
          new_arg_type = ARG_TYPE_ULONG_LONG_PTR;
          break;
        default:
          errno = EINVAL;
          return -1;
        }
        break;
      case 'F':
      case 'f':
      case 'E':
      case 'e':
      case 'G':
      case 'g':
      case 'A':
      case 'a':
        switch(spec->length) {
        case 0:
          new_arg_type = ARG_TYPE_FLOAT_PTR;
          break;
        case LENGTH_LONG:
          new_arg_type = ARG_TYPE_DOUBLE_PTR;
          break;
        case LENGTH_LONG_DOUBLE:
          new_arg_type = ARG_TYPE_LONG_DOUBLE_PTR;
          break;
        default:
          errno = EINVAL;
          return -1;
        }
        break;
      case 'c':
        switch(spec->length) {
        case 0:
          new_arg_type = ARG_TYPE_CHAR_PTR;
          break;
        case LENGTH_LONG:
          new_arg_type = ARG_TYPE_WCHAR_T_PTR;
          break;
        default:
          errno = EINVAL;
          return -1;
        }
        break;
      case 'C':
        if(spec->length != 0) {
          errno = EINVAL;
          return -1;
        }
        new_arg_type = ARG_TYPE_WCHAR_T_PTR;
        spec->conv_c = 'c';
        spec->length = LENGTH_LONG;
        break;
      case '[':
        switch(spec->length) {
        case 0:
          new_arg_type = ARG_TYPE_CHAR_PTR;
          break;
        case LENGTH_LONG:
          new_arg_type = ARG_TYPE_WCHAR_T_PTR;
          break;
        default:
          errno = EINVAL;
          return -1;
        }
        format++;
        spec->set = format;
        if(*format == '^') {
          spec->set_size++;
          format++;
        }
        if(*format == 0) {
          errno = EINVAL;
          return -1;
        }
        format++;
        spec->set_size++;
        for(; *format != ']'; format++, spec->set_size++) {
          if(*format == 0) {
            errno = EINVAL;
            return -1;
          }
        }
        break;
      case 's':
        switch(spec->length) {
        case 0:
          new_arg_type = ARG_TYPE_CHAR_PTR;
          break;
        case LENGTH_LONG:
          new_arg_type = ARG_TYPE_WCHAR_T_PTR;
          break;
        default:
          errno = EINVAL;
          return -1;
        }
        break;
      case 'S':
        if(spec->length != 0) {
          errno = EINVAL;
          return -1;
        }
        new_arg_type = ARG_TYPE_WCHAR_T_PTR;
        spec->conv_c = 's';
        spec->length = LENGTH_LONG;
        break;
      case 'p':
        if(spec->length != 0) {
          errno = EINVAL;
          return -1;
        }
        new_arg_type = ARG_TYPE_VOID_PTR_PTR;
        break;
      case 'n':
        if(spec->length != 0) {
          errno = EINVAL;
          return -1;
        }
        new_arg_type = ARG_TYPE_INT_PTR;
        break;
      default:
        errno = EINVAL;
        return -1;
      }
      if(arg_types != NULL && spec->arg_idx != -1)
        arg_types[spec->arg_idx] = new_arg_type;
      format++;
      *format_ptr = format;
      *curr_arg_idx_ptr = curr_arg_idx;
      *arg_count_ptr = arg_count;
      return 1;
    }
  }
  *format_ptr = format;
  return 0;
}

static __W_INT __W_NAME(, get_char)(struct __W_NAME(vx, scanf_stream) *stream, int *count)
{ 
  if(stream->pushed_c_count == 0) {
    int saved_errno = errno;
    errno = 0;
    __W_INT c = (stream->get_char)(stream->data);
    if(c == __W_EOF) {
      if(errno != 0)
        stream->has_error = 1;
      else
        errno = saved_errno;
      return __W_EOF;
    }
    errno = saved_errno;
    if(*count < INT_MAX) (*count)++;
    return c;
  } else {
    stream->pushed_c_count--;
    if(*count < INT_MAX) (*count)++;
    return stream->pushed_cs[stream->pushed_c_count];
  }
}

static void __W_NAME(, unget_char)(struct __W_NAME(vx, scanf_stream) *stream, __W_INT c, int *count)
{
  if(c != __W_EOF) {
    stream->pushed_cs[stream->pushed_c_count] = c;
    stream->pushed_c_count++;
    if(*count != 0) (*count)--;
  }
}

static int __W_NAME(, skip_spaces)(struct __W_NAME(vx, scanf_stream) *stream, int *count)
{
  while(1) {
    __W_INT c = __W_NAME(, get_char)(stream, count);
    if(c == __W_EOF) return -1;
    if(!__W_ISSPACE(c)) {
      __W_NAME(, unget_char)(stream, c, count);
      break;
    }
  }
  return 0;
}

static char *__W_NAME(, get_field)(struct __W_NAME(vx, scanf_stream) *stream, char *str, size_t max_len, int *count)
{
  size_t i;
  for(i = 0; i < max_len; i++) {
    __W_INT c = __W_NAME(, get_char)(stream, count);
    if(c == __W_EOF) {
      if(stream->has_error) return NULL;
      break;
    }
    if(__W_ISSPACE(c) || !isascii(c)) {
      __W_NAME(, unget_char)(stream, c, count);
      break;
    }
    str[i] = c;
  }
  str[i] = 0;
  return str;
}

static int __W_NAME(, convert_to_int)(struct __W_NAME(vx, scanf_stream) *stream, const struct conv_spec *spec, const union arg_value *value, int *count)
{
  unsigned long long x, max_mul_x, abs_min_i, max_i, max_u, min_i;
  int base, is_unsigned = 0;
  int is_minus, is_first, is_overflow;
  int i, max_width;
  __W_INT c;
  switch(spec->conv_c) {
  case 'd':
    base = 10;
    break;
  case 'i':
    base = 0;
    break;
  case 'u':
    base = 10;
    is_unsigned = 1;
    break;
  case 'o':
    base = 8;
    is_unsigned = 1;
    break;
  case 'X':
  case 'x':
    base = 16;
    is_unsigned = 1;
    break;
  default:
    errno = EINVAL;
    return -1;
  }
  max_width = (spec->max_width != 0 ? spec->max_width : -1);
  if(__W_NAME(, skip_spaces)(stream, count) == -1) return -1;
  i = 0;
  c = __W_NAME(, get_char)(stream, count);
  if(c == __W_EOF) return -1;
  switch(c) {
  case '+':
    i++;
    is_minus = 0;
    break;
  case '-':
    i++;
    is_minus = 1;
    break;
  default:
    __W_NAME(, unget_char)(stream, c, count);
    is_minus = 0;
    break;
  }
  if(base == 0) {
    c = ((max_width == -1 || i < max_width) ? __W_NAME(, get_char)(stream, count) : __W_EOF);
    if(c == '0') {
      i++;
      c = ((max_width == -1 || i < max_width) ? __W_NAME(, get_char)(stream, count) : __W_EOF);
      if(c == 'X' || c == 'x') {
        i++;
        base = 16;
      } else {
        base = 8;
        if(max_width == -1 || i < max_width) {
          if(c == __W_EOF && stream->has_error) return -1;
          __W_NAME(, unget_char)(stream, c, count);
        }
      }
    } else {
      base = 10;
      if(max_width == -1 || i < max_width) {
        if(c == __W_EOF && stream->has_error) return -1;
        __W_NAME(, unget_char)(stream, c, count);
      }
    }
  } else if(base == 16) {
    c = ((max_width == -1 || i < max_width) ? __W_NAME(, get_char)(stream, count) : __W_EOF);
    if(c == '0') {
      i++;
      if(max_width != -1 && i >= max_width) return -1;
      __W_INT c2 = ((max_width == -1 || i < max_width) ? __W_NAME(, get_char)(stream, count) : __W_EOF);
      if(c2 == 'X' || c2 == 'x') {
        i++;
      } else {
        if(max_width == -1 || i < max_width) {
          if(c2 == __W_EOF && stream->has_error) return -1;
          __W_NAME(, unget_char)(stream, c2, count);
        }
        i--;
        if(max_width == -1 || i < max_width)
          __W_NAME(, unget_char)(stream, c, count);
      }
    } else {
      if(max_width == -1 || i < max_width) {
        if(c == __W_EOF && stream->has_error) return -1;
        __W_NAME(, unget_char)(stream, c, count);
      }
    }
  }
  x = 0;
  is_first = 1;
  is_overflow = 0;
  max_mul_x = ULLONG_MAX / base;
  while(1) {
    unsigned digit;
    if(max_width != -1 && i >= max_width) break;
    c = __W_NAME(, get_char)(stream, count);
    if(c == __W_EOF) {
      if(stream->has_error) return -1;
      break;
    }
    if(c >= '0' && c <= (base > 10 ? '9' : base - 1 + '0')) {
      i++;
      digit = c - '0';
    } else if(base > 10 && (c & ~0x20) >= 'A' && (c & ~0x20) <= base - 11 + 'A') {
      i++;
      digit = (c & ~0x20) - 'A' + 10;
    } else {
      __W_NAME(, unget_char)(stream, c, count);
      break;
    }
    if(!is_overflow) {
      if(x <= max_mul_x) {
        x *= base;
        if(x <= ULLONG_MAX - digit)
          x += digit;
        else
          is_overflow = 1;
      } else
        is_overflow = 1;
    }
    is_first = 0;
  }
  if(is_first) {
    if(spec->conv_c != 'i' || base != 8) return -1;
  }
  switch(spec->length) {
  case 0:
    abs_min_i = ((unsigned long long) (-(INT_MIN + 1))) + 1ULL;
    max_i = INT_MAX;
    max_u = UINT_MAX;
    min_i = (unsigned long long) INT_MIN;
    break;
  case LENGTH_CHAR:
    abs_min_i = ((unsigned long long) (-(SCHAR_MIN + 1))) + 1ULL;
    max_i = SCHAR_MAX;
    max_u = UCHAR_MAX;
    min_i = (unsigned long long) SCHAR_MIN;
    break;
  case LENGTH_SHORT:
    abs_min_i = ((unsigned long long) (-(SHRT_MIN + 1))) + 1ULL;
    max_i = SHRT_MAX;
    max_u = USHRT_MAX;
    min_i = (unsigned long long) SHRT_MIN;
    break;
  case LENGTH_LONG:
    abs_min_i = ((unsigned long long) (-(LONG_MIN + 1))) + 1ULL;
    max_i = LONG_MAX;
    max_u = ULONG_MAX;
    min_i = (unsigned long long) LONG_MIN;
    break;
  case LENGTH_LONG_LONG:
    abs_min_i = ((unsigned long long) (-(LLONG_MIN + 1))) + 1ULL;
    max_i = LLONG_MAX;
    max_u = ULLONG_MAX;
    min_i = (unsigned long long) LLONG_MIN;
    break;
  default:
    errno = EINVAL;
    return -1;
  }
  if(!is_overflow) {
    if(!is_unsigned) {
      if(is_minus) {
        if(x <= abs_min_i) {
          x = (unsigned long long) (-((long long) x));
        } else {
          x = min_i;
          errno = ERANGE;
        }
      } else {
        if(x > max_i) {
          x = max_i;
          errno = ERANGE;
        }
      }
    } else {
      if(x > max_u) {
        x = max_u;
        errno = ERANGE;
      }
    }
  } else {
    if(!is_unsigned)
      x = (is_minus ? min_i : max_i);
    else
      x = max_u;
    errno = ERANGE;
  }
  switch(spec->length) {
  case 0:
    if(!is_unsigned) {
      if(value != NULL) *(value->ip) = (int) x;
    } else {
      if(value != NULL) *(value->uip) = (unsigned) x;
    }
    break;
  case LENGTH_CHAR:
    if(!is_unsigned) {
      if(value != NULL) *(value->scp) = (signed char) x;
    } else {
      if(value != NULL) *(value->ucp) = (unsigned char) x;
    }
    break;
  case LENGTH_SHORT:
    if(!is_unsigned) {
      if(value != NULL) *(value->sp) = (short) x;
    } else {
      if(value != NULL) *(value->usp) = (unsigned short) x;
    }
    break;
  case LENGTH_LONG:
    if(!is_unsigned) {
      if(value != NULL) *(value->lp) = (long) x;
    } else {
      if(value != NULL) *(value->ulp) = (unsigned long) x;
    }
    break;
  case LENGTH_LONG_LONG:
    if(!is_unsigned) {
      if(value != NULL) *(value->llp) = (long long) x;
    } else {
      if(value != NULL) *(value->ullp) = (unsigned long long) x;
    }
    break;
  default:
    errno = EINVAL;
    return -1;
  }
  return 0;
}

static int __W_NAME(, convert_to_float)(struct __W_NAME(vx, scanf_stream) *stream, const struct conv_spec *spec, const union arg_value *value, int *count)
{
  long double x, sign, base;
  int is_hex, is_first, is_dot;
  size_t max_digits, digits, int_digits, significant_digits;
  int can_parse_exp;
  int i, max_width;
  __W_INT c;
  max_width = (spec->max_width != 0 ? spec->max_width : -1);
  if(__W_NAME(, skip_spaces)(stream, count) == -1) return -1;
  i = 0;
  c = __W_NAME(, get_char)(stream, count);
  if(c == __W_EOF) return -1;
  switch(c) {
  case '+':
    sign = 1.0;
    i++;
    break;
  case '-':
    sign = -1.0;
    i++;
    break;
  default:
    sign = 1.0;
    __W_NAME(, unget_char)(stream, c, count);
    break;
  }
  c = ((max_width == -1 || i < max_width) ? __W_NAME(, get_char)(stream, count) : __W_EOF);
  if(c == '0') {
    i++;
    __W_INT c2 = ((max_width == -1 || i < max_width) ? __W_NAME(, get_char)(stream, count) : __W_EOF);
    if(c2 == 'X' || c2 == 'x') {
      i++;
      is_hex = 1;
    } else {
      if(max_width == -1 || i < max_width) {
        if(c2 == __W_EOF && stream->has_error) return -1;
        __W_NAME(, unget_char)(stream, c2, count);
      }
      i--;
      if(max_width == -1 || i < max_width)
        __W_NAME(, unget_char)(stream, c, count);
      is_hex = 0;
    }
  } else {
    if(max_width == -1 || i < max_width) {
      if(c == __W_EOF && stream->has_error) return -1;
      __W_NAME(, unget_char)(stream, c, count);
    }
    is_hex = 0;
  }
  if(is_hex) {
    base = 16.0;
    max_digits = LDBL_MAX_HEX_MANT_DIG;
  } else {
    base = 10.0;
    max_digits = LDBL_MAX_DEC_MANT_DIG;
  }
  x = 0.0;
  is_first = 1;
  is_dot = 0;
  digits = 0;
  int_digits = 0;
  significant_digits = 0;
  while(1) {
    long double digit;
    if(max_width != -1 && i >= max_width) break;
    c = __W_NAME(, get_char)(stream, count);
    if(c == __W_EOF) {
      if(stream->has_error) return -1;
      break;
    }
    if(c != '.') {
      if(c >= '0' && c <= '9') {
        i++;
        digit = c - '0';
      } else if(is_hex && (c & ~0x20) >= 'A' && (c & ~0x20) <= 'F') {
        i++;
        digit = (c & ~0x20) - 'A' + 10;
      } else {
        __W_NAME(, unget_char)(stream, c, count);
        break;
      }
      if(digits <= max_digits + 1) {
        x = x * base + digit;
        if(x >= 1.0 || is_dot) {
          digits++;
          significant_digits++;
        }
      } else
        digits++;
    } else {
      if(is_dot) {
        __W_NAME(, unget_char)(stream, c, count);
        break;
      }
      i++;
      if(x == 0.0) {
        digits = 1;
        significant_digits = 1;
      }
      is_dot = 1;
      int_digits = digits; 
    }
    is_first = 0;
  }
  can_parse_exp = 1;
  if(is_first) {
    if(!is_hex) {
      char buf[9];
      can_parse_exp = 0;
      if(__W_NAME(, get_field)(stream, buf, (max_width - i < 8 ? max_width - i : 8), count) == NULL)
        return -1;
      if(strcasecmp(buf, "inf") == 0 || strcasecmp(buf, "infinity") == 0)
        x = HUGE_VALL;
      else if(strncasecmp(buf, "nan", 3) == 0)
        x = __builtin_nanl("");
      else
        return -1;
    } else
      return -1;
  }
  if(can_parse_exp) {
    long exp;
    unsigned long abs_exp;
    int is_exp_first, is_exp_overflow;
    if(!is_dot) int_digits = digits;
    c = ((max_width == -1 || i < max_width) ? __W_NAME(, get_char)(stream, count) : __W_EOF);
    if(is_hex ? (c == 'P' || c == 'p') : (c == 'E' || c == 'e')) {
      int is_exp_minus;
      i++;
      if(max_width != -1 && i >= max_width) return -1;
      c = __W_NAME(, get_char)(stream, count);
      if(c == __W_EOF) return -1;
      switch(c) {
      case '+':
        i++;
        is_exp_minus = 0;
        break;
      case '-':
        i++;
        is_exp_minus = 1;
        break;
      default:
        __W_NAME(, unget_char)(stream, c, count);
        is_exp_minus = 0;
        break;
      }
      abs_exp = 0;
      is_exp_overflow = 0;
      is_exp_first = 1;
      while(1) {
        unsigned digit;
        if(max_width != -1 && i >= max_width) break;
        c = __W_NAME(, get_char)(stream, count);
        if(c == __W_EOF) {
          if(stream->has_error) return -1;
          break;
        }
        if(c >= '0' && c <= (base > 10 ? '9' : base - 1 + '0')) {
          i++;
          digit = c - '0';
        } else {
          __W_NAME(, unget_char)(stream, c, count);
          break;
        }
        if(!is_exp_overflow) {
          if(abs_exp <= ULONG_MAX / 10) {
            abs_exp *= 10;
            if(abs_exp <= ULONG_MAX - digit)
              abs_exp += digit;
            else
              is_exp_overflow = 1;
          } else
            is_exp_overflow = 1;
        }
        is_exp_first = 0;
      }
      if(is_exp_first) return -1;
      if(is_exp_overflow) abs_exp = ULONG_MAX;
      if(is_exp_minus) {
        if(abs_exp <= ((unsigned long long) (-(LONG_MIN + 1))) + 1UL)
          exp = -((long long) abs_exp);
        else
          exp = LONG_MIN;
      } else {
        if(abs_exp <= LONG_MAX)
          exp = (long long) abs_exp;
        else
          exp = LONG_MAX;
      }
    } else {
      if(max_width == -1 || i < max_width) {
        if(c == __W_EOF && stream->has_error) return -1;
        __W_NAME(, unget_char)(stream, c, count);
      }
      exp = 0;
      is_exp_overflow = 0;
    }
    if((digits << (is_hex ? 2 : 0)) <= ((unsigned long) LONG_MAX)) {
      if(!is_exp_overflow) {
        if(x != 0.0) {
          long fract_digit_exp = ((long) (digits - int_digits)) << (is_hex ? 2 : 0);
          long unsignificant_digit_exp = ((long) (digits - significant_digits)) << (is_hex ? 2 : 0);
          int saved_errno = errno;
          exp -= fract_digit_exp;
          exp += unsignificant_digit_exp;
          x = __uportlibc_pow_mul_for_conv(x, exp, fract_digit_exp, is_hex);
          if(HUGE_VALL != LDBL_MAX) errno = saved_errno;
        }
      } else {
        if(exp >= LONG_MAX) {
          x = HUGE_VALL;
          if(HUGE_VALL == LDBL_MAX) errno = ERANGE;
        } else
          x = 0.0;
      }
    } else {
      x = HUGE_VALL;
      if(HUGE_VALL == LDBL_MAX) errno = ERANGE;
    }
  }
  x *= sign;
  switch(spec->length) {
  case 0:
    if(!isnanl(x)) {
      if(x <= FLT_MAX && x >= -FLT_MAX) {
        if(value != NULL) *(value->fp) = x;
      } else {
        if(value != NULL) *(value->fp) = (x >= 0.0 ? HUGE_VALF : -HUGE_VALF);
        if(HUGE_VALF == FLT_MAX) errno = ERANGE;
      }
    } else {
      if(value != NULL) *(value->fp) = __builtin_nanf("");
    }
    break;
  case LENGTH_LONG:
    if(!isnanl(x)) {
      if(x <= DBL_MAX && x >= -DBL_MAX) {
        if(value != NULL) *(value->dp) = x;
      } else {
        if(value != NULL) *(value->dp) = (x >= 0.0 ? HUGE_VAL : -HUGE_VAL);
        if(HUGE_VAL == DBL_MAX) errno = ERANGE;
      }
    } else {
      if(value != NULL) *(value->dp) = __builtin_nan("");
    }
    break;
  case LENGTH_LONG_DOUBLE:
    if(!isnanl(x)) {
      if(x <= LDBL_MAX && x >= -LDBL_MAX) {
        if(value != NULL) *(value->ldp) = x;
      } else {
        if(value != NULL) *(value->ldp) = (x >= 0.0 ? HUGE_VALL : -HUGE_VALL);
        if(HUGE_VALL == LDBL_MAX) errno = ERANGE;
      }
    } else {
      if(value != NULL) *(value->ldp) = __builtin_nanl("");
    }
    break;
  default:
    errno = EINVAL;
    return -1;
  }
  return 0;
}

static int __W_NAME(, convert_to_char)(struct __W_NAME(vx, scanf_stream) *stream, const struct conv_spec *spec, const union arg_value *value, int *count)
{
#if __W != 'w'
  int max_width = (spec->max_width != 0 ? spec->max_width : -1);
  if(spec->length != LENGTH_LONG) {
    int c = __W_NAME(, get_char)(stream, count);
    if(c == EOF) return -1;
    if(value != NULL) *(value->cp) = c;
  } else {
    wchar_t c;
    mbstate_t state;
    int i, is_first;
    memset(&state, 0, sizeof(mbstate_t));
    is_first = 1;
    i = 0;
    while(1) {
      char tmp_b;
      size_t res;
      if(max_width != -1 && i >= max_width) {
        if(!is_first) errno = EILSEQ;
        return -1;
      }
      int b = __W_NAME(, get_char)(stream, count);
      if(b == EOF) {
        if(stream->has_error) return -1;
        if(!is_first) errno = EILSEQ;
        return -1;
      }
      i++;
      tmp_b = b;
      res = mbrtowc(&c, &tmp_b, 1, &state);
      if(res == ((size_t) (-1))) return -1;
      if(res != ((size_t) (-2))) break;
      is_first = 0;
    }
    if(value != NULL) *(value->wcp) = c;
  }
#else
  if(spec->length != LENGTH_LONG) {
    mbstate_t state;
    char buf[MB_LEN_MAX];
    wint_t c = __W_NAME(, get_char)(stream, count);
    if(c == WEOF) return -1;
    memset(&state, 0, sizeof(mbstate_t));
    if(wcrtomb((value != NULL ? value->cp : buf), c, &state) == ((size_t) (-1))) return -1;
  } else {
    wint_t c = __W_NAME(, get_char)(stream, count);
    if(c == WEOF) return -1;
    if(value != NULL) *(value->wcp) = (wchar_t) c;
  }
#endif
  return 0;
}

static int __W_NAME(, check_str_char)(const struct conv_spec *spec, __W_CHAR_INT c)
{
  if(spec->conv_c == '[') {
    __W_CONST_CHAR_PTR set = spec->set;
    size_t i, set_size = spec->set_size; 
    int is_excluding = 0;
    if(spec->set_size >= 1 && spec->set[0] == '^') {
      set++;
      set_size--;
      is_excluding = 1;
    }
    for(i = 0; i < set_size; i++) {
      if(i == 0 || i + 1 == set_size) {
        if(c == (__W_CHAR_INT) ((__W_UCHAR) set[i])) return !is_excluding;
      } else {
        if(set[i] != '-') {
          if(c == (__W_CHAR_INT) ((__W_UCHAR) set[i])) return !is_excluding;
        } else {
          if(c > (__W_CHAR_INT) ((__W_UCHAR) set[i - 1]) && c < (__W_CHAR_INT) ((__W_UCHAR) set[i + 1]))
            return !is_excluding;
        }
      }
    }
    return is_excluding;
  } else
    return !__W_ISSPACE(c);
}

static int __W_NAME(, convert_to_str)(struct __W_NAME(vx, scanf_stream) *stream, const struct conv_spec *spec, const union arg_value *value, int *count)
{
  int max_width = (spec->max_width != 0 ? spec->max_width : -1);
  switch(spec->conv_c) {
  case '[':
    break;
  case 's':
    if(__W_NAME(, skip_spaces)(stream, count) == -1) {
      if(stream->has_error) return -1;
    }
    break;
  default:
    errno = EINVAL;
    return -1;
  }
#if __W != 'w'
  if(spec->length != LENGTH_LONG) {
    char *str = (value != NULL ? value->cp : NULL);
    int i;
    for(i = 0; max_width == -1 || i < max_width; i++) {
      int c = __W_NAME(, get_char)(stream, count);
      if(c == EOF) {
        if(stream->has_error) return -1;
        break;
      }
      if(!__W_NAME(, check_str_char)(spec, (int) ((unsigned char) c))) {
        __W_NAME(, unget_char)(stream, c, count);
        break;
      }
      if(str != NULL) {
        *str = c;
        str++;
      }
    }
    if(str != NULL) *str = 0;
  } else {
    wchar_t *str = (value != NULL ? value->wcp : NULL);
    mbstate_t state;
    int i;
    memset(&state, 0, sizeof(mbstate_t));
    for(i = 0; max_width == -1 || i < max_width; ) {
      wchar_t c;
      int is_first = 1;
      char buf[MB_LEN_MAX];
      int j = 0;
      while(1) {
        char tmp_b;
        size_t res;
        if(max_width != -1 && i >= max_width) {
          if(!is_first) {
            errno = EILSEQ;
            return -1;
          }
          c = WEOF;
          break;
        }
        int b = __W_NAME(, get_char)(stream, count);
        if(b == EOF) {
          if(stream->has_error) return -1;
          if(!is_first) {
            errno = EILSEQ;
            return -1;
          }
          c = WEOF;
          break;
        }
        buf[j] = b;
        j++;
        i++;
        tmp_b = b;
        if(!__W_NAME(, check_str_char)(spec, (int) ((unsigned char) b))) {
          for(j--; j >= 0; j--) {
            __W_NAME(, unget_char)(stream, buf[j], count);
          }
          c = WEOF;
          break;
        }
        res = mbrtowc(&c, &tmp_b, 1, &state);
        if(res == ((size_t) (-1))) return -1;
        if(res != ((size_t) (-2))) break;
        is_first = 0;
      }
      if(c == WEOF) break;
      if(str != NULL) {
        *str = c;
        str++;
      }
    }
    if(str != NULL) *str = 0;
  }
#else
  if(spec->length != LENGTH_LONG) {
    char *str = (value != NULL ? value->cp : NULL);
    mbstate_t state;
    int i;
    memset(&state, 0, sizeof(mbstate_t));
    for(i = 0; max_width == -1 || i < max_width; i++) {
      char buf[MB_LEN_MAX];
      size_t len;
      int c = __W_NAME(, get_char)(stream, count);
      if(c == WEOF) {
        if(stream->has_error) return -1;
        break;
      }
      if(!__W_NAME(, check_str_char)(spec, c)) {
        __W_NAME(, unget_char)(stream, c, count);
        break;
      }
      len = wcrtomb((str != NULL ? str : buf), c, &state);
      if(len == ((size_t) (-1))) return -1;
      if(str != NULL) str += len;
    }
    if(str != NULL) *str = 0;
  } else {
    wchar_t *str = (value != NULL ? value->wcp : NULL);
    int i;
    for(i = 0; max_width == -1 || i < max_width; i++) {
      wint_t c = __W_NAME(, get_char)(stream, count);
      if(c == WEOF) {
        if(stream->has_error) return -1;
        break;
      }
      if(!__W_NAME(, check_str_char)(spec, ((wchar_t) c))) {
        __W_NAME(, unget_char)(stream, c, count);
        break;
      }
      if(str != NULL) {
        *str = c;
        str++;
      }
    }
    if(str != NULL) *str = 0;
  }
#endif
  return 0;
}

static int __W_NAME(, convert_to_ptr)(struct __W_NAME(vx, scanf_stream) *stream, const struct conv_spec *spec, const union arg_value *value, int *count)
{
  struct conv_spec new_spec = *spec;
  union arg_value new_value;
  unsigned long x;
  new_spec.length = LENGTH_LONG;
  new_spec.conv_c = 'x';
  new_value.ulp = &x;
  if(__W_NAME(, convert_to_int)(stream, &new_spec, &new_value, count) == -1) return -1;
  if(value != NULL) *(value->vpp) = (void *) x;
  return 0;
}

int __W_UPORTLIBC_NAME(vx, scanf)(struct __W_NAME(vx, scanf_stream) *stream, const __W_CHAR_PTR format, va_list ap)
{
  __W_CONST_CHAR_PTR tmp_format = format;
  unsigned char arg_types[NL_ARGMAX];
  union arg_value arg_values[NL_ARGMAX];
  unsigned i, curr_arg_idx = 0, arg_count = 0;
  int count, item_count, is_overflow, is_first;
  for(i = 0; i < NL_ARGMAX; i++) arg_types[i] = ARG_TYPE_UNDEF;
  while(*tmp_format != 0) {
    struct conv_spec spec;
    int res = __W_NAME(, parse_conv_spec)(&tmp_format, &spec, arg_types, &curr_arg_idx, &arg_count);
    if(res == -1) return EOF;
    if(res == 0) tmp_format++;
  }
  for(i = 0; i < arg_count; i++) {
    switch(arg_types[i]) {
    case ARG_TYPE_CHAR_PTR:
      arg_values[i].cp = va_arg(ap, char *);
      break;
    case ARG_TYPE_SCHAR_PTR:
      arg_values[i].scp = va_arg(ap, signed char *);
      break;
    case ARG_TYPE_SHORT_PTR:
      arg_values[i].sp = va_arg(ap, short *);
      break;
    case ARG_TYPE_INT_PTR:
      arg_values[i].ip = va_arg(ap, int *);
      break;
    case ARG_TYPE_LONG_PTR:
      arg_values[i].lp = va_arg(ap, long *);
      break;
    case ARG_TYPE_LONG_LONG_PTR:
      arg_values[i].llp = va_arg(ap, long long *);
      break;
    case ARG_TYPE_UCHAR_PTR:
      arg_values[i].ucp = va_arg(ap, unsigned char *);
      break;
    case ARG_TYPE_USHORT_PTR:
      arg_values[i].usp = va_arg(ap, unsigned short *);
      break;
    case ARG_TYPE_UINT_PTR:
      arg_values[i].uip = va_arg(ap, unsigned *);
      break;
    case ARG_TYPE_ULONG_PTR:
      arg_values[i].ulp = va_arg(ap, unsigned long *);
      break;
    case ARG_TYPE_ULONG_LONG_PTR:
      arg_values[i].ullp = va_arg(ap, unsigned long long *);
      break;
    case ARG_TYPE_FLOAT_PTR:
      arg_values[i].fp = va_arg(ap, float *);
      break;
    case ARG_TYPE_DOUBLE_PTR:
      arg_values[i].dp = va_arg(ap, double *);
      break;
    case ARG_TYPE_LONG_DOUBLE_PTR:
      arg_values[i].ldp = va_arg(ap, long double *);
      break;
    case ARG_TYPE_WCHAR_T_PTR:
      arg_values[i].wcp = va_arg(ap, wchar_t *);
      break;
    case ARG_TYPE_VOID_PTR_PTR:
      arg_values[i].vpp = va_arg(ap, void **);
      break;
    default:
      errno = EINVAL;
      return -1;
    }
  }
  count = item_count = 0;
  is_overflow = 0;
  is_first = 1;
  curr_arg_idx = 0;
  arg_count = 0;
  while(*format != 0) {
    struct conv_spec spec;
    union arg_value *value;
    int res = __W_NAME(, parse_conv_spec)(&format, &spec, arg_types, &curr_arg_idx, &arg_count);
    if(res == -1) return -1;
    value = (spec.arg_idx != -1 ? &(arg_values[spec.arg_idx]) : NULL);
    if(res != 0) {
      switch(spec.conv_c) {
      case 'd':
      case 'i':
      case 'u':
      case 'o':
      case 'X':
      case 'x':
        if(__W_NAME(, convert_to_int)(stream, &spec, value, &count) == -1)
          return ((!is_first || count + stream->pushed_c_count != 0) && !stream->has_error) ? item_count : EOF;
        break;
      case 'F':
      case 'f':
      case 'E':
      case 'e':
      case 'G':
      case 'g':
      case 'A':
      case 'a':
        if(__W_NAME(, convert_to_float)(stream, &spec, value, &count) == -1)
          return ((!is_first || count + stream->pushed_c_count != 0) && !stream->has_error) ? item_count : EOF;
        break;
      case 'c':
        if(__W_NAME(, convert_to_char)(stream, &spec, value, &count) == -1)
          return ((!is_first || count + stream->pushed_c_count != 0) && !stream->has_error) ? item_count : EOF;
        break;
      case '[':
      case 's':
        if(__W_NAME(, convert_to_str)(stream, &spec, value, &count) == -1)
          return ((!is_first || count + stream->pushed_c_count != 0) && !stream->has_error) ? item_count : EOF;
        break;
      case 'p':
        if(__W_NAME(, convert_to_ptr)(stream, &spec, value, &count) == -1)
          return ((!is_first || count + stream->pushed_c_count != 0) && !stream->has_error) ? item_count : EOF;
        break;
      case 'n':
        if(value != NULL) *(value->ip) = count;
        break;
      }
      if(spec.arg_idx != -1 && spec.conv_c != 'n') {
        if(item_count < INT_MAX)
          item_count++;
        else
          is_overflow = 1;
      }
    } else {
      if(__W_ISSPACE(*format)) {
        if(__W_NAME(, skip_spaces)(stream, &count) == -1) {
          if(stream->has_error) return EOF;
        }
      } else {
        __W_INT c = __W_NAME(, get_char)(stream, &count);
        if(c == __W_EOF || c != *format)
          return ((!is_first || count + stream->pushed_c_count != 0) && !stream->has_error) ? item_count : EOF;
      }
      format++;
    }
    is_first = 0;
  }
  for(i = 0; i < stream->pushed_c_count; i++) {
    stream->unget_char(stream->data, stream->pushed_cs[i]);
  }
  if(!is_overflow) {
    return item_count;
  } else {
    errno = EOVERFLOW;
    return EOF;
  }
}
