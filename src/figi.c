/*** figi.c -- checker for FIGIs
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
#include <assert.h>
#include "nifty.h"
#include "figi.h"

static const nmck_bid_t nul_bid;

static char
calc_chk(const char *str, size_t UNUSED(len))
{
/* calculate the check digit for an expanded ISIN */
	unsigned int sum = 0U;

	/* use the left 11 digits */
	for (size_t i = 0U; i < 11U; i++) {
		unsigned int d;

		switch (str[i]) {
		case '0' ... '9':
			d = (str[i] ^ '0');
			break;
		case 'B':
		case 'C':
		case 'D':
		case 'F':
		case 'G':
		case 'H':
		case 'J':
			d = 10 + (str[i] - 'A');
			break;
		case 'K':
		case 'L':
		case 'M':
		case 'N':
		case 'P':
		case 'Q':
		case 'R':
		case 'S':
		case 'T':
			d = 20 + (str[i] - 'K');
			break;
		case 'V':
		case 'W':
		case 'X':
		case 'Y':
		case 'Z':
			d = 30 + (str[i] - 'U');
			break;
		default:
			return '\0';
		}

		if (i % 2U) {
			d *= 2U;
		}
		sum += (d / 10U) + (d % 10U);
	}
	/* sum can be at most 665, so check digit is */
	return (char)(((700U - sum) % 10U) ^ '0');
}


/* class implementation */
nmck_bid_t
nmck_figi_bid(const char *str, size_t len)
{
	/* common cases first */
	if (len != 12U) {
		return nul_bid;
	} else if (str[0U] != 'B' || str[1U] != 'B' || str[2U] != 'G') {
		/* currently only BB is registered as certified provider */
		return nul_bid;
	}

	with (char chk = calc_chk(str, len)) {
		if (!chk) {
			return nul_bid;
		} else if (chk != str[11U]) {
			return nul_bid;
		}
	}
	/* bid high */
	return (nmck_bid_t){255U};
}

/* figi.c ends here */
