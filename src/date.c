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
check_m(const char s[static 4U])
{
	switch (s[0U]) {
	case '0':
		return DIGIT(s[1U]) > 0U;
	case '1':
		return DIGIT(s[1U]) <= 2U;
	default:
		break;
	}
	return false;
}

static bool
check_d(const char s[static 4U])
{
	switch (s[0U]) {
	case '0':
		return DIGIT(s[1U]) > 0U;
	case '1':
	case '2':
		return true;
	case '3':
		return DIGIT(s[1U]) <= 1U;
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
	if (len < 8U) {
		return (fn_bid_t){-1};
	} else if (!DIGITP(str[2U]) && str[2U] != str[5U]) {
		/* separators don't match */
		return (fn_bid_t){-1};
	} else if (!check_y(str + 4U + (!DIGITP(str[2U]) << 1U))) {
		/* year's fucked */
		return (fn_bid_t){-1};
	} else if (!check_m(str + 2U + !DIGITP(str[2U])) && !check_d(str + 0U)) {
		/* Day Month is fucked, in that order */
		return (fn_bid_t){-1};
	} else if (!check_m(str + 0U) && !check_d(str + 2U + !DIGITP(str[2U]))) {
		/* Month Day is cunted, in that order */
		return (fn_bid_t){-1};
	}

	/* we survived the else-if tornado */
	return S("date");
}

/* date.c ends here */
