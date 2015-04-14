/*** bidder.h -- determine token types
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
#if !defined INCLUDED_bidder_h_
#define INCLUDED_bidder_h_

#include <stddef.h>

typedef enum {
	FINNER_TOKEN,
	FINNER_FIGI,
	FINNER_ISIN,
	FINNER_CUSIP,
	FINNER_SEDOL,
	FINNER_CCY,
	FINNER_NTOKENS
} finner_token_t;

/**
 * We'll do anonymous bidding.  Every registered checker is asked in
 * turns to submit a tender.  The highest BID value will win.
 * The STATE value can be used by the bidder to record some state.
 *
 * For further optimisation, any bid >= 128U will end the bidding
 * process immediately. */
typedef struct {
	unsigned int bid;
	unsigned int state;
} nmck_bid_t;

/**
 * A bidder class. */
typedef nmck_bid_t(*nmck_bid_f)(const char *str, size_t len);

/**
 * Convenience routine to determine the token type. */
extern finner_token_t finner_bid(const char *str, size_t len);

extern const char *const finner_bidstr[FINNER_NTOKENS];

#endif	/* INCLUDED_bidder_h_ */
