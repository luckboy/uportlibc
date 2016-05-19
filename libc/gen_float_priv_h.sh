#!/bin/sh
# Copyright (c) 2016 Łukasz Szpakowski
#
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included in
# all copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
# THE SOFTWARE.

script1=`cat <<EOF
#include <float.h>

function ceil(x)
{
    if(x >= 0.0)
        return x - int(x) > 1e-6 ? int(x) + 1 : int(x);
    else
        return int(x);
}

function floor(x)
{
    if(x >= 0.0)
        return int(x);
    else
        return int(x) - x > 1e-6 ? int(x) - 1 : int(x);
}

BEGIN {
    printf("#ifndef _FLOAT_PRIV_H\\n");  
    printf("#define _FLOAT_PRIV_H\\n");
    printf("\n");
    logr_log10 = log(FLT_RADIX) / log(10.0);
    max_dec_exp = floor(LDBL_MAX_EXP * logr_log10);
    min_dec_exp = floor(LDBL_MIN_EXP * logr_log10);
    max_dec_mant_dig = ceil(LDBL_MANT_DIG * logr_log10);
    printf("#define LDBL_MAX_DEC_EXP\t\t(%d)\n", max_dec_exp);
    printf("#define LDBL_MIN_DEC_EXP\t\t(%d)\n", min_dec_exp);
    printf("#define LDBL_MAX_DEC_MANT_DIG\t\t(%d)\n", max_dec_mant_dig);
    printf("#define LDBL_MAX_DEC_STRTOLD_EXP\tLDBL_MAX_DEC_EXP\n");
    printf("#define LDBL_MIN_DEC_STRTOLD_EXP\tLDBL_MIN_DEC_EXP\n");
    printf("\n");
    logr_log2 = log(FLT_RADIX) / log(2.0);
    max_hex_exp = floor(LDBL_MAX_EXP * logr_log2);
    min_hex_exp = floor(LDBL_MIN_EXP * logr_log2);
    max_hex_mant_dig = ceil(LDBL_MANT_DIG * (logr_log2 / 4.0));
    min_hex_strtold_exp = floor((LDBL_MIN_EXP - 1.0) * logr_log2) - 3;
    printf("#define LDBL_MAX_HEX_EXP\t\t(%d)\n", max_hex_exp);
    printf("#define LDBL_MIN_HEX_EXP\t\t(%d)\n", min_hex_exp);
    printf("#define LDBL_MAX_HEX_MANT_DIG\t\t(%d)\n", max_hex_mant_dig);
    printf("#define LDBL_MAX_HEX_STRTOLD_EXP\tLDBL_MAX_HEX_EXP\n");
    printf("#define LDBL_MIN_HEX_STRTOLD_EXP\t(%d)\n", min_hex_strtold_exp);
    printf("\n");
    printf("#endif\n");
}
EOF`
script2=`printf '%s' "$script1" | $CC $CFLAGS -E -P -x c -`
echo -n | awk "$script2"
