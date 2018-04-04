/*** cusip.c -- checker for CUSIPs
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
#include "finner.h"
#include "nifty.h"

static const char*
cusip(fn_state_t UNUSED(st))
{
	return "CUSIP";
}


fn_bnu_t
fn_cusip(const char *str, size_t len)
{
	uint_fast32_t sum = 0U;

	/* common cases first */
	if (len < 9U) {
		return (fn_bnu_t){NULL};
	}

	/* use the left 8 digits */
	for (size_t i = 0U; i < 8U; i++) {
		uint_fast32_t d;

		switch (str[i]) {
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
			return (fn_bnu_t){NULL};
		}

		/* double every other */
		d <<= i % 2U;
		sum += (d / 10U) + (d % 10U);
	}
	with (uint_fast32_t d = (unsigned char)(str[8U] ^ '0')) {
		if (d >= 10U) {
			/* last one must be a digit */
			return (fn_bnu_t){NULL};
		}
		sum += d;
	}
	/* have a look at the check equation */
	if ((sum %= 10)) {
		/* check digit don't match */
		return (fn_bnu_t){NULL};
	}
	return (fn_bnu_t){cusip};
}

/* cusip.c ends here */
