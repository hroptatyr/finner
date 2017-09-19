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

typedef enum {
	UNK,
	AUX,
} timex_state_t;


/* class implementation */
fn_bid_t
fn_timex_bid(const char *str, size_t len)
{
	const char *const ep = str + len;
	const char *sp = str;
	timex_state_t s = UNK;

	/* at least a digit and a separator we want */
	if (len < 2U) {
		return fn_nul_bid;
	} else if (!DIGITP(str[0U])) {
		switch (*str) {
		case 'a':
		case 'A':
		case 'p':
		case 'P':
			/* am/pm indicator */
			if (*++sp == '.') {
				sp++;
			}
			if (sp >= ep || !(*sp == 'm' || *sp == 'M')) {
				break;
			}
			/* optional dot */
			if (*++sp == '.' && sp < ep) {
				sp++;
			}
			/* make sure the string ends here */
			if (sp < ep) {
				break;
			}
			s = AUX;
			goto good;
		default:
			break;
		}
		return fn_nul_bid;
	}
	/* try 14h30 or 14:30 or the likes */
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
good:
	/* bid just any number really */
	return (fn_bid_t){FINNER_TIME, ep - sp, s};
}

fn_bid_t
fn_timex_collect(const struct anno_s *av, size_t len)
{
	if (av->b.bid == FINNER_TIME && av->b.state == AUX) {
		/* degrade */
		return (fn_bid_t){FINNER_DEGR, 1U};
	} else if (len <= 1U) {
		return fn_nul_bid;
	} else if (av[1U].b.bid != FINNER_TIME || av[1U].b.state != AUX) {
		return fn_nul_bid;
	}
	/* otherwise collect */
	return (fn_bid_t){FINNER_TIME, 2U};
}

/* timex.c ends here */
