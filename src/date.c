/*** date.c -- checker for dates
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

	m = 
		"Jan" "uary"? | "Feb" "ruary"? | "Mar" "ch"? |
		"Apr" "il"? | "May" | "Jun" "e"? |
		"Jul" "y"? | "Aug" "ust"? | "Sep" "tember"? |
		"Oct" "ober"? | "Nov" "ember"? | "Dec" "ember"? ;
	date =
		digit{1,2} (" " | "-" | "/") m (" " | "-" | "/") digit{4} @{c(date_m2)} |
		m " " digit{1,2} ", " digit{4} @{c(date_m1)} |
		digit{4} ("/" | "-")? digit{2} ("/" | "-")? digit{2} @{c(date_y1)} |
		digit{1,2} ("." | "/")? digit{1,2} ("." | "/")? digit{4} @{c(date_yl)} ;
}%%
#endif	/* RAGEL_BLOCK */

static bool
check_y(const char s[static 4U])
{
/* just accept 19xx and 20xx dates */
	switch (s[0U]) {
	case '1':
		return s[1U] == '9';
	case '2':
		return s[1U] == '0';
	default:
		break;
	}
	return false;
}

static bool
check_m(const char s[static 2U])
{
	switch (s[0U]) {
	case '0':
		return DIGIT(s[1U]) > 0U;
	case '1':
		return DIGIT(s[1U]) <= 2U || !DIGITP(s[1U]);
	case '2':
	case '3':
	case '4':
	case '5':
	case '6':
	case '7':
	case '8':
	case '9':
		return !DIGITP(s[1U]);
	default:
		break;
	}
	return false;
}

static bool
check_d(const char s[static 2U])
{
	switch (s[0U]) {
	case '0':
		return DIGIT(s[1U]) > 0U && DIGITP(s[1U]);
	case '1':
	case '2':
		return true;
	case '3':
		return DIGIT(s[1U]) <= 1U || !DIGITP(s[1U]);
	case '4':
	case '5':
	case '6':
	case '7':
	case '8':
	case '9':
		return !DIGITP(s[1U]);
	}
	return false;
}


fn_bid_t
fn_date_y1(const char *str, size_t len)
{
/* check if str is a date with the year upfront */
	if (len < 8U) {
		return (fn_bid_t){-1};
	} else if (!DIGITP(str[4U]) && str[4U] != str[7U]) {
		/* separators don't match*/
		return (fn_bid_t){-1};
	} else if (!check_y(str + 0U)) {
		/* year's fucked */
		return (fn_bid_t){-1};
	} else if (!check_m(str + 4U + !DIGITP(str[4U]))) {
		/* Month is cunted, in that order */
		return (fn_bid_t){-1};
	} else if (!check_d(str + 6U + (!DIGITP(str[4U]) << 1U))) {
		/* Day is fucked, in that order */
		return (fn_bid_t){-1};
	}

	/* we survived the else-if tornado */
	return S("date");
}

fn_bid_t
fn_date_yl(const char *str, size_t len)
{
/* check if str is a date with the year last */
	size_t sep1, sep2;

	if (len < 8U) {
		return (fn_bid_t){-1};
	} else if (DIGITP(str[sep1 = 1U]) && DIGITP(str[sep1 = 2U])) {
		/* no separator? */
		return (fn_bid_t){-1};
	} else if (DIGITP(str[sep2 = 3U]) &&
		   DIGITP(str[sep2 = 4U]) &&
		   DIGITP(str[sep2 = 5U])) {
		/* no second separator? */
		return (fn_bid_t){-1};
	} else if (str[sep1] != str[sep2]) {
		/* separators don't match */
		return (fn_bid_t){-1};
	} else if (!check_y(str + sep2 + 1U)) {
		/* year's fucked */
		return (fn_bid_t){-1};
	} else if (check_m(str + sep1 + 1U) && check_d(str + 0U)) {
		/* day month order worked */
		;
	} else if (check_m(str + 0U) && check_d(str + sep1 + 1U)) {
		/* month day order worked */
		;
	} else {
		return (fn_bid_t){-1};
	}

	/* we survived the else-if tornado */
	return S("date");
}

fn_bid_t
fn_date_m1(const char *str, size_t len)
{
/* month first */
	const char *sp;

	if (len < 11U) {
		return (fn_bid_t){-1};
	}

	sp = memchr(str + 3U, ' ', len - 3U);
	if (sp < str + 3U || sp > str + 9U) {
		/* how did we get here? */
		return (fn_bid_t){-1};
	} else if (!check_d(sp + 1U)) {
		/* day is wrong */
		return (fn_bid_t){-1};
	} else if (!check_y(str + len - 4U)) {
		/* year doesn't work */
		return (fn_bid_t){-1};
	}

	return S("date");
}

fn_bid_t
fn_date_m2(const char *str, size_t len)
{
/* month second */
	size_t sep1, sep2;

	if (len < 10U) {
		return (fn_bid_t){-1};
	} else if (!check_d(str)) {
		return (fn_bid_t){-1};
	} else if (DIGITP(str[sep1 = 1U]) && DIGITP(str[sep1 = 2U])) {
		/* no separator? */
		return (fn_bid_t){-1};
	} else if (DIGITP(str[sep2 = len - 5U])) {
		/* no second separator? */
		return (fn_bid_t){-1};
	} else if (str[sep1] != str[sep2]) {
		/* separators don't match */
		return (fn_bid_t){-1};
	} else if (!check_y(str + sep2 + 1U)) {
		/* year doesn't work */
		return (fn_bid_t){-1};
	}

	return S("date");
}

/* date.c ends here */
