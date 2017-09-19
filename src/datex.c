/*** datex.c -- fragments of dates
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
#include "datex.h"

#define DIGITP(x)	(((unsigned char)((x) ^ '0')) < 10U)

typedef enum {
	UNK,
	FRAG,
} datex_state_t;

static bool
check_y(const char s[static 4U])
{
/* just accept 19xx and 20xx dates */
	switch (s[0U]) {
	case '1':
		return s[1U] == '9' && DIGITP(s[2U]) && DIGITP(s[3U]);
	case '2':
		return s[1U] == '0' && DIGITP(s[2U]) && DIGITP(s[3U]);
	default:
		break;
	}
	return false;
}

#include "months-cc.c"


/* class implementation */
fn_bid_t
fn_datex_bid(const char *str, size_t len)
{
	const char *sp = str;

	if (*sp >= 'A' && *sp <= 'Z') {
		char buf[4U] = "\0\0\0\0";
		size_t i;

		*buf = *sp;
		for (i = 1U;
		     i < 3U && i < len && sp[i] >= 'a' && sp[i] <= 'z';
		     i++) {
			buf[i] = (char)(sp[i] - 0x20);
		}
		if (i < 3U || !valid_cc_p(buf)) {
			return fn_nul_bid;
		} else if (i >= len) {
			/* successful fragment of 3 */
			return (fn_bid_t){FINNER_DATE, len - 3U, FRAG};
		}

		switch (*sp) {
		yep:
			return (fn_bid_t){FINNER_DATE, 0U, FRAG};

		case 'J':
			/* jan(uary) jun(e) jul(y) */
			if (len == 4U) {
				switch (sp[3U]) {
				case 'e':
				case 'y':
					goto yep;
				default:
					break;
				}
			} else if (len == 7U && !memcmp(sp + 3U, "uary", 4U)) {
				goto yep;
			}
			break;
		case 'F':
			/* feb(ruary) fri(day) */
			if (len == 8U && !memcmp(sp + 3U, "ruary", 5U)) {
				goto yep;
			} else if (len == 6U && !memcmp(sp + 3U, "day", 3U)) {
				goto yep;
			}
			break;
		case 'M':
			/* mar(ch) may mon(day)*/
			if (len == 5U && sp[3U] == 'c' && sp[4U] == 'h') {
				goto yep;
			} else if (len = 6U && !memcmp(sp + 3U, "day", 3U)) {
				goto yep;
			}
			break;
		case 'A':
			/* apr(il) aug(ust) */
			if (len = 5U && sp[3U] == 'i' && sp[4U] == 'l') {
				goto yep;
			} else if (len = 6U && !memcmp(sp + 3U, "ust", 3U)) {
				goto yep;
			}
			break;
		case 'S':
			/* sep(tember), sat(urday), sun(day) */
			if (len == 9U && !memcmp(sp + 3U, "tember", 6U)) {
				goto yep;
			} else if (len == 8U && !memcmp(sp + 3U, "urday", 5U)) {
				goto yep;
			} else if (len == 6U && !memcmp(sp + 3U, "day", 3U)) {
				goto yep;
			}
			break;
		case 'O':
			/* oct(ober) */
			if (len = 7U && !memcmp(sp + 3U, "ober", 4U)) {
				goto yep;
			}
			break;
		case 'N':
			/* nov(ember) */
		case 'D':
			/* dec(ember) */
			if (len == 8U && !memcmp(sp + 3U, "ember", 5U)) {
				goto yep;
			}
			break;
		case 'T':
			/* tue(sday) thu(rsday) */
			if ((len == 7U || len == 8U) &&
			    !memcmp(sp + len - 4U, "sday", 4U)) {
				goto yep;
			}
			break;
		case 'W':
			/* wed(nesday) */
			if (len == 9U && !memcmp(sp + 3U, "nesday", 6U)) {
				goto yep;
			}
			break;
		default:
			break;
		}
	} else if (len == 4U && check_y(sp)) {
		return (fn_bid_t){FINNER_DATE, 0U, FRAG};
	}
	return fn_nul_bid;
}

fn_bid_t
fn_datex_collect(const struct anno_s *av, size_t len)
{
/* allow date+num[+date] and num+date[+date] */
	fn_tok_t a0 = len > 0U ? av[0U].b.bid : FINNER_TERM;
	fn_tok_t a1 = len > 1U ? av[1U].b.bid : FINNER_TERM;
	fn_tok_t a2 = len > 2U ? av[2U].b.bid : FINNER_TERM;
	datex_state_t s0 = len > 0U ? (datex_state_t)av[0U].b.state : UNK;
	datex_state_t s1 = len > 1U ? (datex_state_t)av[1U].b.state : UNK;
	datex_state_t s2 = len > 2U ? (datex_state_t)av[2U].b.state : UNK;

	switch (a0) {
	case FINNER_NUM:
		if (a1 == FINNER_DATE && s1 == FRAG) {
			unsigned int more = a2 == FINNER_DATE && s2 == FRAG;
			return (fn_bid_t){FINNER_DATE, 2U + more};
		}
		break;
	case FINNER_DATE:
		if (s0 != FRAG) {
			;
		} else if (a1 == FINNER_DATE && s1 == FRAG) {
			return (fn_bid_t){FINNER_DATE, 2U};
		} else if (a1 == FINNER_NUM) {
			unsigned int more = a2 == FINNER_DATE && s2 == FRAG;
			return (fn_bid_t){FINNER_DATE, 2U + more};
		}
		break;
	default:
		break;
	}
	return fn_nul_bid;
}

/* datex.c ends here */
