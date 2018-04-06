/*** ccy.c -- checker for currencies
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

#define DIGIT(x)	((unsigned char)((x) ^ '0'))
#define DIGITP(x)	(DIGIT(x) < 10U)

#ifdef RAGEL_BLOCK
%%{
	machine finner;

	ccy_valid =
		"AD" 'F'i |
		"AE" 'D'i |
		"AF" 'N'i |
		"AL" 'L'i |
		"AM" 'D'i |
		"AN" 'G'i |
		"AO" 'A'i |
		"AR" 'S'i |
		"AT" 'S'i |
		"AU" 'D'i |
		"AW" 'G'i |
		"AZ" 'N'i |
		"BA" 'M'i |
		"BB" 'D'i |
		"BD" 'T'i |
		"BE" 'F'i |
		"BG" 'N'i |
		"BH" 'D'i |
		"BI" 'F'i |
		"BM" 'D'i |
		"BN" 'D'i |
		"BO" 'B'i |
		"BR" 'L'i |
		"BS" 'D'i |
		"BT" 'N'i |
		"BW" 'P'i |
		"BY" 'N'i |
		"BZ" 'D'i |
		"CA" 'D'i |
		"CD" 'F'i |
		"CF" 'P'i |
		"CH" 'F'i |
		"CL" 'P'i |
		"CN" 'Y'i |
		"CO" 'P'i |
		"CR" 'C'i |
		"CS" 'D'i |
		"CU" 'C'i |
		"CV" 'E'i |
		"CY" 'P'i |
		"CZ" 'K'i |
		"DD" 'M'i |
		"DE" 'M'i |
		"DJ" 'F'i |
		"DK" 'K'i |
		"DO" 'P'i |
		"DZ" 'D'i |
		"EC" 'S'i |
		"EE" 'K'i |
		"EG" 'P'i |
		"EQ" 'E'i |
		"ER" 'N'i |
		"ES" 'P'i |
		"ET" 'B'i |
		"EU" 'R'i |
		"FI" 'M'i |
		"FJ" 'D'i |
		"FK" 'P'i |
		"FR" 'F'i |
		"GB" 'P'i |
		"GBX" |
		"GE" 'L'i |
		"GH" 'S'i |
		"GI" 'P'i |
		"GM" 'D'i |
		"GN" 'F'i |
		"GR" 'D'i |
		"GT" 'Q'i |
		"GW" 'P'i |
		"GY" 'D'i |
		"HK" 'D'i |
		"HN" 'L'i |
		"HR" 'K'i |
		"HT" 'G'i |
		"HU" 'F'i |
		"ID" 'R'i |
		"IE" 'P'i |
		"IL" 'S'i |
		"IN" 'R'i |
		"IQ" 'D'i |
		"IR" 'R'i |
		"IS" 'K'i |
		"IT" 'L'i |
		"JM" 'D'i |
		"JO" 'D'i |
		"JP" 'Y'i |
		"KE" 'S'i |
		"KG" 'S'i |
		"KH" 'R'i |
		"KM" 'F'i |
		"KP" 'W'i |
		"KR" 'W'i |
		"KW" 'D'i |
		"KY" 'D'i |
		"KZ" 'T'i |
		"LA" 'K'i |
		"LB" 'P'i |
		"LK" 'R'i |
		"LR" 'D'i |
		"LS" 'L'i |
		"LT" 'L'i |
		"LU" 'F'i |
		"LV" 'L'i |
		"LY" 'D'i |
		"MA" 'D'i |
		"MC" 'F'i |
		"MD" 'L'i |
		"MG" 'A'i |
		"MK" 'D'i |
		"MM" 'K'i |
		"MN" 'T'i |
		"MO" 'P'i |
		"MR" 'O'i |
		"MT" 'L'i |
		"MU" 'R'i |
		"MV" 'R'i |
		"MW" 'K'i |
		"MX" 'N'i |
		"MY" 'R'i |
		"MZ" 'N'i |
		"NA" 'D'i |
		"NG" 'N'i |
		"NI" 'O'i |
		"NL" 'G'i |
		"NO" 'K'i |
		"NP" 'R'i |
		"NZ" 'D'i |
		"OM" 'R'i |
		"PA" 'B'i |
		"PE" 'N'i |
		"PG" 'K'i |
		"PH" 'P'i |
		"PK" 'R'i |
		"PL" 'N'i |
		"PT" 'E'i |
		"PY" 'G'i |
		"QA" 'R'i |
		"RO" 'N'i |
		"RS" 'D'i |
		"RU" 'B'i |
		"RW" 'F'i |
		"SA" 'R'i |
		"SB" 'D'i |
		"SC" 'R'i |
		"SD" 'G'i |
		"SE" 'K'i |
		"SG" 'D'i |
		"SH" 'P'i |
		"SK" 'K'i |
		"SL" 'L'i |
		"SO" 'S'i |
		"SR" 'D'i |
		"SS" 'P'i |
		"ST" 'D'i |
		"SV" 'C'i |
		"SY" 'P'i |
		"SZ" 'L'i |
		"TH" 'B'i |
		"TJ" 'S'i |
		"TM" 'T'i |
		"TN" 'D'i |
		"TO" 'P'i |
		"TP" 'E'i |
		"TR" 'Y'i |
		"TT" 'D'i |
		"TW" 'D'i |
		"TZ" 'S'i |
		"UA" 'H'i |
		"UG" 'X'i |
		"US" 'D'i |
		"UY" 'U'i |
		"UZ" 'S'i |
		"VA" 'L'i |
		"VE" 'F'i |
		"VN" 'D'i |
		"VU" 'V'i |
		"WS" 'T'i |
		"XA" 'F'i |
		"XB" 'A'i |
		"XC" 'D'i |
		"XD" 'R'i |
		"XE" 'U'i |
		"XF" 'O'i |
		"XO" 'F'i |
		"XP" 'D'i |
		"XS" 'U'i |
		"XT" 'S'i |
		"XU" 'A'i |
		"XX" 'X'i |
		"YD" 'D'i |
		"YE" 'R'i |
		"YU" 'D'i |
		"ZA" 'R'i |
		"ZM" 'W'i |
		"ZR" 'N'i |
		"ZW" 'L'i ;

	ccy = ccy_valid @{c(ccy)} ;
}%%
#endif	/* RAGEL_BLOCK */

static const char*
ccy(fn_state_t st)
{
	static char buf[] = "ccy(...)";
	memcpy(buf + 4U, &st, 3U);
	return buf;
}


fn_bid_t
fn_ccy(const char *str, size_t UNUSED(len))
{
/* we trust the regexp */
	fn_state_t capture;
	if (len < 3U) {
		return (fn_bid_t){-1};
	}
	memcpy(&capture, str + len - 3U, 3U);
	return (fn_bid_t){capture, ccy};
}

/* ccy.c ends here */
