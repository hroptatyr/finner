/*** bidder.c -- determine token types
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
#if defined HAVE_CONFIG_H
# include "config.h"
#endif	/* HAVE_CONFIG_H */
#include "bidder.h"
#include "nifty.h"
/* bidders */
#include "figi.h"
#include "isin.h"
#include "cusip.h"
#include "sedol.h"
#include "ccy.h"
#include "fxpair.h"
#include "amt.h"
#include "wkn.h"
#include "date.h"

const char *const finner_bidstr[FINNER_NTOKENS] = {
	[FINNER_TERM] = "term",
	[FINNER_FIGI] = "figi",
	[FINNER_ISIN] = "isin",
	[FINNER_CUSIP] = "cusip",
	[FINNER_SEDOL] = "sedol",
	[FINNER_CCY] = "ccy",
	[FINNER_FXPAIR] = "fxpair",
	[FINNER_AMT] = "amt",
	[FINNER_WKN] = "wkn",
	[FINNER_DATE] = "date",
};


/* public api */
fn_bid_t
finner_bid(const char *str, size_t len)
{
#define CHECK(bidder)				\
	with (fn_bid_t x = bidder(str, len)) {		\
		if (x.bid) {				\
			return x;			\
		}					\
	}

	/* start the bidding */
	CHECK(fn_figi_bid);
	CHECK(fn_isin_bid);
	CHECK(fn_cusip_bid);
	CHECK(fn_sedol_bid);
	CHECK(fn_ccy_bid);
	CHECK(fn_fxpair_bid);
	CHECK(fn_amt_bid);
	CHECK(fn_date_bid);
	/* high risk stuff last */
	CHECK(fn_wkn_bid);
	return fn_nul_bid;
}

/* bidder.c ends here */
