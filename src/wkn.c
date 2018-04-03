/*** wkn.c -- checker for WKNs, very high in false positives
 *
 * Copyright (C) 2014-2018 Sebastian Freundt
 *
 * Author:  Sebastian Freundt <freundt@ga-group.nl>
 *
 * This file is part of finner.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * 3. Neither the name of the author nor the names of any contributors
 *    may be used to endorse or promote products derived from this
 *    software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR "AS IS" AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED.  IN NO EVENT SHALL THE REGENTS OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR
 * BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE
 * OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN
 * IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 ***/
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdint.h>
#include <ctype.h>
#include "finner.h"
#include "nifty.h"

static const char*
wkn(fn_state_t UNUSED(st))
{
	return "WKN";
}


fn_bnu_t
fn_wkn(const char *str, size_t len)
{
	size_t ndigits = 0U;

	if (len < 6U) {
	nope:
		return (fn_bnu_t){NULL};
	}
	/* we need at least one digit */
	for (size_t i = 0U; i < 6U; i++) {
		if ((unsigned char)(str[i] ^ '0') < 10U) {
			ndigits++;
		} else if (str[i] < 'A') {
			goto nope;
		} else if (str[i] > 'Z') {
			goto nope;
		} else if (str[i] == 'I') {
			goto nope;
		} else if (str[i] == 'O') {
			goto nope;
		}
	}
	if (!ndigits) {
		/* check for CBKTLR und CBKBZR */
		if (str[0U] != 'C' ||
		    (memcmp(str, "CBKTLR", 6U) && memcmp(str, "CBKBZR", 6U))) {
			goto nope;
		}
	}

	/* bid bid bid */
	return (fn_bnu_t){wkn};
}

/* wkn.c ends here */
