/*** num.c -- checker for numbers
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
#include "num.h"


/* class implementation */
fn_bid_t
fn_num_bid(const char *str, size_t len)
{
	const char *sp = str;
	const char *const ep = str + len;

	if (UNLIKELY(*sp == '-')) {
		/* allow leading `-' */
		sp++;
	}
	if (*sp == '0') {
		/* demand decimal dot now */
		if (*++sp != '.') {
			return fn_nul_bid;
		}
	} else if (!(*sp >= '1' && *sp <= '9')) {
		return fn_nul_bid;
	}
	/* only digits for now */
	for (sp++; sp < ep; sp++) {
		if (!(*sp >= '0' && *sp <= '9')) {
			break;
		}
	}
	if (sp < ep) {
		/* might be `.' or `,' */
		switch (*sp++) {
		case '.':
			/* only allow digits from now on */
			for (; sp < ep; sp++) {
				if (!(*sp >= '0' && *sp <= '9')) {
					break;
				}
			}
			break;
		case ',':
		rechk:
			/* allow digits */
			for (size_t i = 3U; i && sp < ep; sp++, i--) {
				if (!(*sp >= '0' && *sp <= '9')) {
					return fn_nul_bid;
				}
			}
			if (sp >= ep) {
				break;
			} else if (*sp++ == ',') {
				/* ah, another comma group */
				goto rechk;
			}
			/* otherwise it's bullshit */
			break;
		default:
			sp--;
			break;
		}
	}
	return (fn_bid_t){FINNER_NUM, ep - sp};
}

/* num.c ends here */
