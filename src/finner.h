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

/**
 * Bidders are expected to respond with a state >= 0. */
typedef intptr_t fn_state_t;

/**
 * Return type for bidders.  STATE (if > 0) is used as a const char*.
 * If optional print routine is given, STATE will be turned into
 * a string through evaluation. */
typedef struct {
	fn_state_t state;
	const char *(*print)(fn_state_t state);
} fn_bid_t;

typedef struct {
	size_t sta;
	size_t end;
} extent_t;

typedef struct {
	fn_bid_t b;
	extent_t x;
} anno_t;


extern fn_bid_t fn_wkn(const char*, size_t);
extern fn_bid_t fn_num(const char*, size_t);
extern fn_bid_t fn_isin(const char*, size_t);
extern fn_bid_t fn_figi(const char*, size_t);
extern fn_bid_t fn_cusip(const char*, size_t);
extern fn_bid_t fn_sedol(const char*, size_t);
extern fn_bid_t fn_lei(const char*, size_t);
extern fn_bid_t fn_fxpair(const char*, size_t);
extern fn_bid_t fn_date_y1(const char*, size_t);
extern fn_bid_t fn_date_yl(const char*, size_t);
extern fn_bid_t fn_date_m1(const char*, size_t);
extern fn_bid_t fn_date_m2(const char*, size_t);
extern fn_bid_t fn_time(const char*, size_t);


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
/* modelled after ctype.h's isX() routines. */
	return c == eob;
}

/* turn bid into a string */
static inline const char*
B(fn_bid_t b)
{
	return !b.print ? (const char*)b.state : b.print(b.state);
}

/* turn string into bid */
static inline fn_bid_t
S(const char *x)
{
	return (fn_bid_t){(fn_state_t)x};
}

#endif	/* INCLUDED_finner_h_ */
