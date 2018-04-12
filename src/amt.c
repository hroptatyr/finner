/*** amt.c -- checker for amounts
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

#ifdef RAGEL_BLOCK
%%{
	machine finner;

	action G {
		fn_ccy_for_amt(r.b);
	}

	_num =
		num "k" |
		(num " ")? "million" |
		num ("mm" | "m") |
		(num " ")? "billion" |
		num "bn" |
		(num " ")? "trillion" |
		num "tr" |
		num ;
	amt = (ccy | ccysym) @G " "? _num @{c(amt)} |
		_num " "? (ccy | ccysym) @G @{c(amt)} |
		"R" @{fn_ccy_for_amt(S("ccy(ZAR)"));} " "? _num @{c(amt)} ;
}%%
#endif	/* RAGEL_BLOCK */

static fn_state_t last_ccy;

static const char*
amt(fn_state_t st)
{
	static char buf[] = "amt(...)";
	memcpy(buf + 4U, &st, 3U);
	return buf;
}


void
fn_ccy_for_amt(fn_bid_t b)
{
	const char *ccy = B(b);
	memcpy(&last_ccy, ccy + 4U, 3U);
	return;
}

fn_bid_t
fn_amt(const char *UNUSED(str), size_t UNUSED(len))
{
/* we trust the machine above and also that last_ccy
 * was updated properly, i.e. by calling fn_ccy_for_amt() */
	return (fn_bid_t){last_ccy, amt};
}

/* amt.c ends here */
