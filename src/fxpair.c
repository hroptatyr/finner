/*** fxpair.c -- checker for FX pairs
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
#include <assert.h>
#include "finner.h"
#include "nifty.h"

/* allowed isin country codes */
#include "fxpair-cc.c"

typedef union {
	uintptr_t u;
	struct {
		char b[3U];
		char t[3U];
	};
} fxpair_state_t;


/* class implementation */
fn_bid_t
fn_fxpair_bid(const char *str, size_t len)
{
	fxpair_state_t s = {0U};

	/* common cases first */
	if (len < 6U || len > 7U) {
		return fn_nul_bid;
	} else if (!valid_cc_p(str + 0U)) {
		return fn_nul_bid;
	}
	memcpy(s.b, str, 3U);
	with (const char *sp = str + 3U) {
		switch (*sp) {
		case '.':
		case ':':
		case '/':
			sp++;
		default:
			break;
		}
		if (!valid_cc_p(sp)) {
			return fn_nul_bid;
		} else if (!memcmp(str, sp, 3U)) {
			return fn_nul_bid;
		}
		memcpy(s.t, sp, 3U);
	}

	/* bid just any number really */
	return (fn_bid_t){FINNER_FXPAIR, 0U, s.u};
}

const char*
fn_fxpair_prs(uintptr_t u)
{
	static char buf[7U];
	fxpair_state_t s = {u};
	memcpy(buf, s.b, 6U);
	return buf;
}

/* fxpair.c ends here */
