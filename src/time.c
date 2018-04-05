/*** time.c -- checker for times
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
#include <stdbool.h>
#include <assert.h>
#include "finner.h"
#include "nifty.h"

#define DIGIT(x)	((unsigned char)((x) ^ '0'))
#define DIGITP(x)	(DIGIT(x) < 10U)

#ifdef RAGEL_BLOCK
%%{
	machine finner;

	h = digit{1,2} " "? ("am" | "pm" | "AM" | "PM") ;
	hm = digit{1,2} ("h"|":") digit{2} (" "? ("am" | "pm" | "AM" | "PM"))? ;
	hms = digit{1,2} ":" digit{2} ":" digit{2} ;
	time = (h | hm | hms) @{c(time)} ;
}%%
#endif	/* RAGEL_BLOCK */

static bool
check_h(const char s[static 2U])
{
	switch (DIGIT(s[0U])) {
	case 0U:
	case 1U:
		return DIGITP(s[1U]) || !DIGITP(s[1U]);
	case 2U:
		return DIGIT(s[1U]) <= 4U || !DIGITP(s[1U]);
	default:
		break;
	}
	return DIGITP(s[0U]) && !DIGITP(s[1U]);
}

static bool
check_m(const char s[static 2U])
{
	return DIGIT(s[0U]) < 6U && DIGITP(s[1U]);
}

static bool
check_s(const char s[static 2U])
{
	return DIGIT(s[0U]) < 6U && DIGITP(s[1U]) ||
		/* allow leap seconds */
		DIGIT(s[0U]) == 6U && DIGIT(s[1U]) == 0U;
}


fn_bid_t
fn_time(const char *str, size_t len)
{
	if (len < 3U) {
		return (fn_bid_t){-1};
	} else if (!check_h(str + 0U)) {
		/* hour doesn't check out */
		return (fn_bid_t){-1};
	} else if (len <= 5U && (str[len - 1U] | 0x20) == 'm') {
		/* obviously just the time and the am/pm indicator */
		;
	} else if (!check_m(str + 2U + DIGITP(str[1U]))) {
		/* minute is b0rked */
		return (fn_bid_t){-1};
	} else if (len > 5U && (str[len - 1U] | 0x20) == 'm') {
		/* am/pm indicator has no seconds */
		;
	} else if (len > 5U && !check_s(str + 2U + DIGITP(str[1U]) + 3U)) {
		/* seconds don't work */
		return (fn_bid_t){-1};
	}

	/* we survived the else-if tornado */
	return S("time");
}

/* time.c ends here */
