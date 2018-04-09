/*** unit-1.c -- checker for unitless "units", i.e. factors
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

	unit_1 = 
		(num " ")? "million" @{r("num(*1000000)")} |
		num ("mm" | "m") @{r("num(*1000000)")} |
		(num " ")? "billion" @{r("num(*1000000000)")} |
		num "bn" @{r("num(*1000000000)")} |
		(num " ")? "trillion" @{r("num(*1000000000000)")} |
		num "tr" @{r("num(*1000000000000)")} |
		(num " ")? "percent" @{r("num(*0.01)")} |
		(num " "?)? "pct" @{r("num(*0.01)")} |
		num " "? "%" @{r("num(*0.01)")} |
		(num " ")? "basis points" @{r("num(*0.0001)")} |
		(num " "?)? "bps" @{r("num(*0.0001)")} |
		num " "? "bps" @{r("num(*0.0001)")} |
		amt ("k" | "m" | "bn" | "tr") @{x()} ;
}%%
#endif	/* RAGEL_BLOCK */

/* unit-1.c ends here */
