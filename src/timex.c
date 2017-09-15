/*** timex.c -- checker for times
 *
 * Copyright (C) 2014-2017 Sebastian Freundt
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
#include <stdbool.h>
#include <assert.h>
#include "nifty.h"
#include "timex.h"

#define DIGITP(x)	(((unsigned char)((x) ^ '0')) < 10U)
#define SEPARP(x)	((x) == ':' || (x) == 'h')


/* class implementation */
fn_bid_t
fn_timex_bid(const char *str, size_t len)
{
	const char *const ep = str + len;
	const char *sp = str;

	/* at least a digit and a separator we want */
	if (len < 2U) {
		return fn_nul_bid;
	} else if (!DIGITP(str[0U])) {
		return fn_nul_bid;
	}
	sp++;
	sp += DIGITP(*sp);
	if (sp >= ep || !SEPARP(*sp)) {
		return fn_nul_bid;
	}
	/* optionally allow another pair of digits */
	if (++sp < ep) {
		if (sp + 2U > ep || !(DIGITP(sp[0U]) && DIGITP(sp[1U]))) {
			return fn_nul_bid;
		}
		sp += 2U;
		/* optionally allow another separator, only : though */
		if (sp < ep && *sp++ == ':') {
			/* another 2 digits */
			if (sp + 2U > ep ||
			    !(DIGITP(sp[0U]) && DIGITP(sp[1U]))) {
				return fn_nul_bid;
			}
			sp += 2U;
		}
	}

	/* bid just any number really */
	return (fn_bid_t){FINNER_TIME, ep - sp};
}

/* timex.c ends here */
