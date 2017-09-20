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

#include "iso4217.h"

#define DIGITP(x)	(((unsigned char)((x) ^ '0')) < 10U)


/* class implementation */
fn_bid_t
fn_ccysym_bid(const char *str, size_t len)
{
	const char *sp = str;
	const char *const ep = str + len;
	iso4217_t c = UNK;

	/* dollars first */
	if (len >= 2U && sp[1U] == '$') {
		/* x$ */
		switch (*sp) {
		case 'A':
			c = AUD;
			break;
		case 'B':
			c = BSD;
			break;
		case 'C':
			c = CAD;
			break;
		case 'G':
			c = GYD;
			break;
		case 'S':
			c = SGD;
			break;
		case 'R':
			c = BRL;
			break;
		case 'L':
			c = LRD;
			break;
		case 'N':
			c = NAD;
			break;
		case 'Z':
			c = ZWL;
			break;
		case 'J':
			c = JMD;
			break;
		default:
			return fn_nul_bid;
		}
		sp += 2U;
		goto fin;
	} else if (len >= 3U && sp[2U] == '$') {
		switch (*sp) {
		case 'H':
			c = sp[1U] == 'K' ? HKD : UNK;
			break;
		case 'A':
			c = sp[1U] == 'U' ? AUD : UNK;
			break;
		case 'G':
			c = sp[1U] == 'Y' ? GYD : UNK;
			break;
		case 'N':
			c = sp[1U] == 'Z' ? NZD
				: sp[1U] == 'T' ? TWD
				: UNK;
			break;
		case 'S':
			c = sp[1U] == 'G' ? SGD
				: sp[1U] == 'I' ? SBD
				: UNK;
			break;
		case 'U':
			c = sp[1U] == 'S' ? USD : UNK;
			break;
		case 'B':
			c = sp[1U] == 'Z' ? BZD
				: sp[1U] == 'D' ? BMD
				: UNK;
			break;
		case 'C':
			c = sp[1U] == 'I' ? KYD : UNK;
			break;
		case 'E':
			c = sp[1U] == 'C' ? XCD : UNK;
			break;
		case 'F':
			c = sp[1U] == 'J' ? FJD : UNK;
			break;
		case 'T':
			c = sp[1U] == 'T' ? TTD : UNK;
			break;
		default:
			return fn_nul_bid;
		}
		sp += 3U;
		goto fin;
	}
	/* otherwise proceed in order */
	switch (*sp++) {
	case 'S':
		if (len >= 3U && *sp++ == 'F' && *sp++ == 'r') {
			c = CHF;
			break;
		}
		return fn_nul_bid;
	case 'D':
		if (len >= 2U && *sp++ == 'M') {
			c = DEM;
			break;
		}
		return fn_nul_bid;
	case 'F':
		if (len >= 2U && *sp++ == 'r') {
			c = FRF;
			break;
		}
		return fn_nul_bid;
	case '$':
		c = USD;
		break;
	case 'R':
		if (len >= 2U && *sp == 'M') {
			sp++;
			c = MYR;
			break;
		}
		c = ZAR;
		break;
	case 'z':
		if (len >= 3U && *sp++ == '\xc5' && *sp++ == '\x82') {
			c = PLN;
			break;
		}
		return fn_nul_bid;
	case 'B':
		if (len >= 4U && *sp++ == 'd' && *sp++ == 's' && *sp++ == '$') {
			c = BBD;
			break;
		}
		return fn_nul_bid;

	case '\xc2':
		/* GBP, JPY */
		if (UNLIKELY(len < 2U)) {
			return fn_nul_bid;
		}
		switch (*sp++) {
		case '\xa3':
			c = GBP;
			break;
		case '\xa5':
			c = JPY;
			break;
		default:
			return fn_nul_bid;
		}
		break;
	case '\xd6':
		/* AMD */
		if (UNLIKELY(len < 2U)) {
			return fn_nul_bid;
		}
		switch (*sp++) {
		case '\x8f':
			c = AMD;
			break;
		default:
			return fn_nul_bid;
		}
		break;
	case '\xe0':
		/* THB */
		if (UNLIKELY(len < 3U || *sp++ != '\xb8')) {
			return fn_nul_bid;
		}
		switch (*sp++) {
		case '\xbf':
			c = THB;
			break;
		default:
			return fn_nul_bid;
		}
		break;
	case '\xe2':
		/* EUR, NRN */
		if (UNLIKELY(len < 3U || *sp++ != '\x82')) {
			return fn_nul_bid;
		}
		switch (*sp++) {
		case '\xac':
			c = EUR;
			break;
		case '\xa6':
			c = NGN;
			break;
		case '\xa9':
			c = KRW;
			break;
		case '\xab':
			c = VND;
			break;
		case '\xb4':
			c = UAH;
			break;
		case '\xb9':
			c = INR;
			break;
		case '\xba':
			c = TRY;
			break;
		default:
			return fn_nul_bid;
		}
		break;
	default:
		return fn_nul_bid;
	}
fin:
	/* next must be EP or a digit */
	if (sp < ep && !DIGITP(*sp)) {
		return fn_nul_bid;
	} else if (!c) {
		return fn_nul_bid;
	}
	uintptr_t s = 0U;
	memcpy(&s, iso4217[c].sym, 4U);
	return (fn_bid_t){FINNER_CCY, ep - sp, s};
}

/* ccysym.c ends here */
