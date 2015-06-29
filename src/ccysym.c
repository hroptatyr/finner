/*** ccysym.c -- checker for currencies expressed by symbols
 *
 * Copyright (C) 2014-2015 Sebastian Freundt
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
#include "nifty.h"
#include "ccysym.h"

typedef enum {
	UNK,
	AUD,
	CAD,
	DEM,
	EUR,
	GBP,
	JPY,
	SGD,
	USD,
	NCCY
} ccy_t;

static const char *const ccy[] = {
	[AUD] = "AUD",
	[CAD] = "CAD",
	[DEM] = "DEM",
	[EUR] = "EUR",
	[GBP] = "GBP",
	[JPY] = "JPY",
	[SGD] = "SGD",
	[USD] = "USD",
};


/* class implementation */
fn_bid_t
fn_ccysym_bid(const char *str, size_t len)
{
	const char *sp = str;
	const char *const ep = str + len;
	ccy_t state = UNK;

	switch (*sp++) {
	case 'A':
		state = AUD;
		goto ACS;
	case 'C':
		state = CAD;
		goto ACS;
	case 'S':
		state = SGD;
		goto ACS;
	ACS:
		/* AUD, CAD, SGD */
		if (sp >= ep || *sp++ != '$') {
			return fn_nul_bid;
		}
		break;
	case 'D':
		/* DM */
		if (sp >= ep || *sp++ != 'M') {
			return fn_nul_bid;
		}
		state = DEM;
		break;
	case '$':
		/* USD */
		state = USD;
		break;
	case '\xc2':
		/* GBP, JPY */
		if (sp >= ep) {
			return fn_nul_bid;
		}
		switch (*sp++) {
		case '\xa3':
			state = GBP;
			break;
		case '\xa5':
			state = JPY;
			break;
		default:
			return fn_nul_bid;
		}
		break;
	case '\xe2':
		/* EUR? */
		if (sp + 1U >= ep || *sp++ != '\x82' || *sp++ != '\xac') {
			return fn_nul_bid;
		}
		state = EUR;
		break;
	default:
		return fn_nul_bid;
	}

	return (fn_bid_t){FINNER_CCY, ep - sp, state};
}

/* ccysym.c ends here */