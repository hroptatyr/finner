/*** cusip.c -- checker for CUSIPs
 *
 * Copyright (C) 2014-2015 Sebastian Freundt
 *
 * Author:  Sebastian Freundt <freundt@ga-group.nl>
 *
 * This file is part of numchk.
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
#include "nifty.h"
#include "cusip.h"

static unsigned int
calc_chk(const char *str, size_t len)
{
/* calculate the check digit for an expanded ISIN */
	unsigned int sum = 0U;
	size_t i = 0U;

	/* use the left 8 digits */
	for (size_t j = 0U; j < 8U && i < len; i++) {
		unsigned int d;

		switch (str[i]) {
		case '-':
			/* ignore */
			continue;
		case '0' ... '9':
			d = (str[i] ^ '0');
			break;
		case 'A' ... 'Z':
			d = 10 + (str[i] - 'A');
			break;
		case '*':
			d = 36;
			break;
		case '@':
			d = 37;
			break;
		case '#':
			d = 38;
			break;
		default:
			return 0U;
		}

		if (j++ % 2U) {
			d *= 2U;
		}
		sum += (d / 10U) + (d % 10U);
	}
	/* check if need to skip optional - */
	if (LIKELY(i < len) && UNLIKELY(str[i] == '-')) {
		i++;
	}

	/* sum can be at most 342, so check digit is */
	return i << 8U ^ (((400U - sum) % 10U) ^ '0');
}


/* class implementation */
fn_bid_t
fn_cusip_bid(const char *str, size_t len)
{
	/* common cases first */
	if (len < 8U || len > 11U) {
		return fn_nul_bid;
	}

	with (unsigned int cc = calc_chk(str, len)) {
		unsigned int consumed = cc >> 8U;
		char chk = (char)(cc & 0xff);

		if (!cc) {
			return fn_nul_bid;
		} else if (consumed != len - 1U) {
			return fn_nul_bid;
		} else if (chk != str[consumed]) {
			return fn_nul_bid;
		}
	}
	/* bid higher than isin */
	return (fn_bid_t){FINNER_CUSIP};
}

/* cusip.c ends here */
