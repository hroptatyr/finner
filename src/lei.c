/*** lei.c -- checker for ISO 17442 legal entity identifiers
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
 * OR OTHERWISE) ARLEIG IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN
 * IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 ***/
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdint.h>
#include <assert.h>
#include "finner.h"
#include "nifty.h"


fn_bid_t
fn_lei(const char *str, size_t len)
{
	uint_fast8_t buf[40U];
	size_t bsz = 0U;
	uint_fast32_t sum = 0U;

	/* common cases first */
	if (len < 20U) {
		return (fn_bid_t){-1};
	}

	/* expand string first */
	for (size_t i = 0U; i < 18U; i++) {
		switch (str[i]) {
		case '0' ... '9':
			buf[bsz++] = (unsigned char)(str[i] ^ '0');
			break;
		case 'A' ... 'J':
			buf[bsz++] = 1U;
			buf[bsz++] = (unsigned char)(str[i] - 'A');
			break;
		case 'K' ... 'T':
			buf[bsz++] = 2U;
			buf[bsz++] = (unsigned char)(str[i] - 'K');
			break;
		case 'U' ... 'Z':
			buf[bsz++] = 3U;
			buf[bsz++] = (unsigned char)(str[i] - 'U');
			break;
		default:
			return (fn_bid_t){-1};
		}
	}
	/* and the last two */
	with (uint_fast32_t d) {
		if ((d = (unsigned char)(str[18U] ^ '0')) >= 10U) {
			return (fn_bid_t){-1};
		}
		buf[bsz++] = d;
		if ((d = (unsigned char)(str[19U] ^ '0')) >= 10U) {
			return (fn_bid_t){-1};
		}
		buf[bsz++] = d;
	}

	/* now calc first sum */
	sum = buf[0U] * 10U + buf[1U];
	for (size_t i = 2U; i < bsz; sum %= 97U) {
		/* just so we calculate less modulos */
		for (const size_t n = i + 7U < bsz ? i + 7U : bsz; i < n; i++) {
			sum *= 10U;
			sum += buf[i];
		}
	}
	if ((sum %= 97U) - 1U) {
		/* doesn't check out */
		return (fn_bid_t){-1};
	}
	/* otherwise all's doog */
	return S("lei");
}

/* lei.c ends here */
