/*** ccysym.c -- checker for currencies represented by their symbols
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

	ccysym =
		"A$" @{r("ccy(AUD)")} |
		"B$" @{r("ccy(BSD)")} |
		"C$" @{r("ccy(CAD)")} |
		"G$" @{r("ccy(GYD)")} |
		"S$" @{r("ccy(SGD)")} |
		"R$" @{r("ccy(BRL)")} |
		"L$" @{r("ccy(LRD)")} |
		"N$" @{r("ccy(NAD)")} |
		"Z$" @{r("ccy(ZWL)")} |
		"J$" @{r("ccy(JMD)")} |
		"HK$" @{r("ccy(HKD)")} |
		"AU$" @{r("ccy(AUD)")} |
		"GY$" @{r("ccy(GYD)")} |
		"NZ$" @{r("ccy(NZD)")} |
		"SG$" @{r("ccy(SGD)")} |
		"US$" @{r("ccy(USD)")} |
		"BZ$" @{r("ccy(BZD)")} |
		"BD$" @{r("ccy(BMD)")} |
		"CI$" @{r("ccy(KYD)")} |
		"EC$" @{r("ccy(XCD)")} |
		"FJ$" @{r("ccy(FJD)")} |
		"TT$" @{r("ccy(TTD)")} |
		"SFr" @{r("ccy(CHF)")} |
		"DM" @{r("ccy(DEM)")} |
		"Fr" @{r("ccy(FRF)")} |
		"RM" @{r("ccy(MYR)")} |
		"Bds$" @{r("ccy(BBD)")} |
		"z" 0xc5 0x82 @{r("ccy(PLN)")} |
		0xc2 0xa3 @{r("ccy(GBP)")} |
		0xc2 0xa5 @{r("ccy(JPY)")} |
		0xd6 0x8f @{r("ccy(AMD)")} |
		0xe0 0xb8 0xbf @{r("ccy(THB)")} |
		0xe2 0x82 0xac @{r("ccy(EUR)")} |
		0xe2 0x82 0xa6 @{r("ccy(NGN)")} |
		0xe2 0x82 0xa9 @{r("ccy(KRW)")} |
		0xe2 0x82 0xab @{r("ccy(VND)")} |
		0xe2 0x82 0xb4 @{r("ccy(UAH)")} |
		0xe2 0x82 0xb9 @{r("ccy(INR)")} |
		0xe2 0x82 0xba @{r("ccy(TRY)")} |
		"$" @{r("ccy(USD)")} ;
}%%
#endif	/* RAGEL_BLOCK */

/* ccysym.c ends here */
