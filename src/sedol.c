/*** sedol.c -- checker for SEDOLs
 *
 * Copyright (C) 2014-2018 Sebastian Freundt
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
#include "finner.h"
#include "nifty.h"

static char
calc_chk(const char *str, size_t UNUSED(len))
{
/* calculate the check digit for an expanded ISIN */
	unsigned int w[] = {1U, 3U, 1U, 7U, 3U, 9U};
	unsigned int sum = 0U;

	/* use the left 6 digits */
	for (size_t i = 0U; i < 6U; i++) {
		unsigned int d;

		switch (str[i]) {
		case '0' ... '9':
			d = (str[i] ^ '0');
			break;
		case 'A' ... 'Z':
			d = 10 + (str[i] - 'A');
			break;
		default:
			return 0U;
		}

		sum += w[i] * d;
	}

	/* sum can be at most 840, so check digit is */
	return (char)(((840U - sum) % 10U) ^ '0');
}


/* class implementation */
fn_bid_t
fn_sedol_bid(const char *str, size_t len)
{
	/* common cases first */
	if (len != 7U) {
		return fn_nul_bid;
	}

	with (char chk = calc_chk(str, len)) {
		if (!chk) {
			return fn_nul_bid;
		} else if (chk != str[6U]) {
			return fn_nul_bid;
		}
	}
	/* bid just any number really */
	return (fn_bid_t){FINNER_SEDOL};
}

/* sedol.c ends here */
