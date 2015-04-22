/*** amt.c -- checker for amounts
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
#include "amt.h"

#define FINNER_CCYNUM	(FINNER_CCY ^ FINNER_NUM)


/* class implementation */
fn_bid_t
fn_amt_collect(const struct anno_s *a, size_t n)
{
/* a number paired with a currency is retagged as amount AMT */
	switch (a->b.bid) {
	case FINNER_CCY:
	case FINNER_NUM:
		if (UNLIKELY(n == 1U)) {
			/* shame */
			return (fn_bid_t){FINNER_AMT, 0U};
		} else if ((a[0U].b.bid ^ a[1U].b.bid) == FINNER_CCYNUM) {
			/* yaay */
			break;
		}
		/* otherwise fallthrough */
	default:
		return fn_nul_bid;
	}
	if (n > 2U && (a[1U].b.bid ^ a[2U].b.bid) == FINNER_CCYNUM) {
		/* check the distances */
		size_t a01 = fn_extent_dist(a[0U].x, a[1U].x);
		size_t a12 = fn_extent_dist(a[1U].x, a[2U].x);

		if (a12 < a01 && a->b.bid == FINNER_NUM) {
			/* we prefer the constellation CCY NUM */
			return fn_nul_bid;
		}
	}
	return (fn_bid_t){FINNER_AMT, 2U};
}

/* amt.c ends here */
