/*** finner.h -- main snippet
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
#if !defined INCLUDED_finner_h_
#define INCLUDED_finner_h_
#include <stdint.h>

typedef enum {
	FINNER_TERM,
	FINNER_FIGI,
	FINNER_ISIN,
	FINNER_LEI,
	FINNER_CUSIP,
	FINNER_SEDOL,
	FINNER_CCY,
	FINNER_FXPAIR,
	FINNER_NUM,
	FINNER_WKN,
	FINNER_DATE,
	FINNER_TIME,
	FINNER_UNIT_1,
	FINNER_NTOKENS,

	/* collections here */
	FINNER_AMT,
	FINNER_NTAGS,

	/* degrading */
	FINNER_DEGR = -1,
} fn_tok_t;

/**
 * We'll do anonymous bidding.  Registered bidders are asked in
 * sequence to submit a tender.  The first bidder with a bid different
 * from FINNER_TERM will seal the deal.
 *
 * Optionally, the bidder can submit a tender for only a *prefix* of the
 * term string in question.  In that case LEFTOVER must be the number
 * of bytes the bidder chose to ignore.  Internally, this will result
 * in the term being cut in two halves, the prefix one with a bid, and
 * a new term constructed from the left-overs that is subject to bidding
 * in the next round.
 *
 * The STATE value can be used by the bidder to record some state.
 * Refer to the documentation of the bidder in question to find out
 * about STATE. */
typedef struct {
	fn_tok_t bid;
	union {
		unsigned int leftover;
		unsigned int span;
	};
	uintptr_t state;
} fn_bid_t;

#define fn_nul_bid	((fn_bid_t){FINNER_TERM})

typedef intptr_t fn_state_t;

/**
 * New bidding state. */
typedef struct {
	const char *(*print)(fn_state_t state);
	fn_state_t state;
} fn_bnu_t;

typedef struct {
	size_t sta;
	size_t end;
} extent_t;

struct anno_s {
	extent_t x;
	fn_bid_t b;
};

typedef struct {
	extent_t x;
	fn_bnu_t b;
} annu_t;


extern fn_bnu_t fn_wkn(const char*, size_t);
extern fn_bnu_t fn_num(const char*, size_t);
extern fn_bnu_t fn_ccy(const char*, size_t);
extern fn_bnu_t fn_isin(const char*, size_t);
extern fn_bnu_t fn_figi(const char*, size_t);
extern fn_bnu_t fn_amt(const char*, size_t);
extern const char *fn_unit_1(fn_state_t);


/* convenience */
static inline size_t
fn_extent_dist(extent_t a, extent_t b)
{
/* calculate the distance between extents A and B,
 * defined as
 * + b.sta - a.end  for  a < b,
 * + a.sta - b.end  for  b < a,
 * + 0 otherwise */
	if (b.sta > a.end) {
		return b.sta - a.end;
	} else if (b.end < a.sta) {
		return a.sta - b.end;
	}
	return 0U;
}

#define eob	'\377'

static inline int
iseob(int c)
{
	return c == eob;
}

#endif	/* INCLUDED_finner_h_ */
