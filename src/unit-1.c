/*** unit-1.c -- unitless factors
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

typedef enum {
	UNK,
	HUNDRED,
	THOUSAND,
	MILLION,
	BILLION,
	TRILLION,
	PERCENT,
	BPOINT,
} unit_1_t;

static const char*
unit_1(fn_state_t st)
{
	switch (st) {
	case BILLION:
		return "*1000000000";
	case MILLION:
		return "*1000000";
	case THOUSAND:
		return "*1000";
	case TRILLION:
		return "*1000000000000";
	case PERCENT:
		return "*0.01";
	case BPOINT:
		return "*0.0001";
	case HUNDRED:
		return "*100";
	default:
		break;
	}
	return "1";
}


fn_bnu_t
fn_unit_1(const char *str, size_t len)
{
	const char *sp = str;
	const char *const ep = str + len;
	unit_1_t guess = UNK;

	switch (*sp) {
	case '%':
		guess = PERCENT;
		sp++;
		break;
	case 'p':
		if (++sp >= ep) {
			/* don't worry */
			break;
		} else if (sp + 6U > ep || memcmp(sp, "ercent", 6U)) {
			/* not percent */
			break;
		}
		/* otherwise use him */
		sp += 6U;
		guess = PERCENT;
		break;

	case 'b':
		if (++sp >= ep) {
			/* it's any bullshit */
			break;
		}
		switch (*sp) {
		case 'p':
			guess = BPOINT;
			break;
		case 'n':
			guess = BILLION;
			break;
		default:
			guess = BILLION;
			goto illion;
		}
		if (++sp < ep && (*sp != '.' || ++sp < ep)) {
			/* only accept bn. and bp. */
			guess = UNK;
		}
		break;
	case 'm':
		guess = MILLION;
		if (++sp >= ep) {
			/* it's 11m or so */
			break;
		}
		goto illion;
	case 't':
		if (++sp >= ep || *sp++ != 'r') {
			/* it's nothing */
			break;
		}
		guess = TRILLION;
		goto illion;

	illion:
		if (sp + 6U != ep || memcmp(sp, "illion", 6U)) {
			/* nope, not an illion of any kind */
			guess = UNK;
		}
		sp += 6U;
		break;
	}
	if (guess == UNK) {
		return (fn_bnu_t){};
	}
	return (fn_bnu_t){unit_1, guess};
}

/* unit-1.c ends here */
