/*** gics.c -- checker for MSCI gics codes
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
#include <stdbool.h>
#include <assert.h>
#include "finner.h"
#include "nifty.h"

#ifdef RAGEL_BLOCK
%%{
	machine finner;

	gics_valid =
		"10101010" |
		"10101020" |
		"10102010" |
		"10102020" |
		"10102030" |
		"10102040" |
		"10102050" |
		"15101010" |
		"15101020" |
		"15101030" |
		"15101040" |
		"15101050" |
		"15102010" |
		"15103010" |
		"15103020" |
		"15104010" |
		"15104020" |
		"15104025" |
		"15104030" |
		"15104040" |
		"15104045" |
		"15104050" |
		"15105010" |
		"15105020" |
		"20101010" |
		"20102010" |
		"20103010" |
		"20104010" |
		"20104020" |
		"20105010" |
		"20106010" |
		"20106015" |
		"20106020" |
		"20107010" |
		"20201010" |
		"20201020" |
		"20201030" |
		"20201040" |
		"20201050" |
		"20201060" |
		"20201070" |
		"20201080" |
		"20202010" |
		"20202020" |
		"20301010" |
		"20302010" |
		"20303010" |
		"20304010" |
		"20304020" |
		"20305010" |
		"20305020" |
		"20305030" |
		"25101010" |
		"25101020" |
		"25102010" |
		"25102020" |
		"25201010" |
		"25201020" |
		"25201030" |
		"25201040" |
		"25201050" |
		"25202010" |
		"25202020" |
		"25203010" |
		"25203020" |
		"25203030" |
		"25301010" |
		"25301020" |
		"25301030" |
		"25301040" |
		"25302010" |
		"25302020" |
		"25401010" |
		"25401020" |
		"25401025" |
		"25401030" |
		"25401040" |
		"25501010" |
		"25502010" |
		"25502020" |
		"25503010" |
		"25503020" |
		"25504010" |
		"25504020" |
		"25504030" |
		"25504040" |
		"25504050" |
		"25504060" |
		"30101010" |
		"30101020" |
		"30101030" |
		"30101040" |
		"30201010" |
		"30201020" |
		"30201030" |
		"30202010" |
		"30202020" |
		"30202030" |
		"30203010" |
		"30301010" |
		"30302010" |
		"35101010" |
		"35101020" |
		"35102010" |
		"35102015" |
		"35102020" |
		"35102030" |
		"35103010" |
		"35201010" |
		"35202010" |
		"35203010" |
		"40101010" |
		"40101015" |
		"40102010" |
		"40201010" |
		"40201020" |
		"40201030" |
		"40201040" |
		"40202010" |
		"40203010" |
		"40203020" |
		"40203030" |
		"40203040" |
		"40204010" |
		"40301010" |
		"40301020" |
		"40301030" |
		"40301040" |
		"40301050" |
		"40401010" |
		"40401020" |
		"40402010" |
		"40402020" |
		"40402030" |
		"40402035" |
		"40402040" |
		"40402045" |
		"40402050" |
		"40402060" |
		"40402070" |
		"40403010" |
		"40403020" |
		"40403030" |
		"40403040" |
		"45101010" |
		"45102010" |
		"45102020" |
		"45102030" |
		"45103010" |
		"45103020" |
		"45103030" |
		"45201010" |
		"45201020" |
		"45202010" |
		"45202020" |
		"45202030" |
		"45203010" |
		"45203015" |
		"45203020" |
		"45203030" |
		"45204010" |
		"45205010" |
		"45205020" |
		"45301010" |
		"45301020" |
		"50101010" |
		"50101020" |
		"50102010" |
		"50201010" |
		"50201020" |
		"50201030" |
		"50201040" |
		"50202010" |
		"50202020" |
		"50203010" |
		"55101010" |
		"55102010" |
		"55103010" |
		"55104010" |
		"55105010" |
		"55105020" |
		"60101010" |
		"60101020" |
		"60101030" |
		"60101040" |
		"60101050" |
		"60101060" |
		"60101070" |
		"60101080" |
		"60102010" |
		"60102020" |
		"60102030" |
		"60102040" ;

	gics = gics_valid @{c(gics)} ;
}%%
#endif	/* RAGEL_BLOCK */


fn_bid_t
fn_gics(const char *UNUSED(str), size_t len)
{
/* we trust the regexp */
	if (len != 8U) {
		return (fn_bid_t){-1};
	}
	return S("gics");
}

/* gics.c ends here */
