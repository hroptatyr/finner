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

#define DIGITP(x)	(((unsigned char)((x) ^ '0')) < 10U)

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
		return (unsigned int)(s[1U] ^ '0') > 0U;
	case '1':
		return (unsigned int)(s[1U] ^ '0') <= 2U;
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
		return (unsigned int)(s[1U] ^ '0') > 0U;
	case '1':
	case '2':
		return true;
	case '3':
		return (unsigned int)(s[1U] ^ '0') <= 1U;
	}
	return false;
}


/* class implementation */
fn_bid_t
fn_date_bid(const char *str, size_t len)
{
	char sep = '\0';

	/* at least 2 digits we want */
	if (!(len == 8U || len == 10U)) {
		return fn_nul_bid;
	} else if (!DIGITP(str[0U]) || !DIGITP(str[1U])) {
		return fn_nul_bid;
	}
	if (DIGITP(str[2U])) {
		/* could be YYYY-MM-DD */
		if (!DIGITP(str[3U])) {
			return fn_nul_bid;
		}
		if (DIGITP(str[4U])) {
			/* must be YYYYMMDD */
			if (!DIGITP(str[5U]) ||
			    !DIGITP(str[6U]) ||
			    !DIGITP(str[7U])) {
				/* apparently not */
				return fn_nul_bid;
			} else if (len != 8U) {
				return fn_nul_bid;
			} else if (!check_y(str + 0U)) {
				return fn_nul_bid;
			} else if (!check_m(str + 4U)) {
				return fn_nul_bid;
			} else if (!check_d(str + 6U)) {
				return fn_nul_bid;
			}
		} else {
			switch (str[4U]) {
			case '/':
			case '-':
				if (len == 10U) {
					sep = str[4U];
					break;
				}
			default:
				return fn_nul_bid;
			}
			if (!DIGITP(str[5U]) || !DIGITP(str[6U])) {
				return fn_nul_bid;
			} else if (str[7U] != sep) {
				return fn_nul_bid;
			} else if (!DIGITP(str[8U]) || !DIGITP(str[9U])) {
				return fn_nul_bid;
			} else if (!check_y(str + 0U)) {
				return fn_nul_bid;
			} else if (!check_m(str + 5U)) {
				return fn_nul_bid;
			} else if (!check_d(str + 8U)) {
				return fn_nul_bid;
			}
		}
	} else {
		switch (str[2U]) {
		case '.':
		case '/':
			if (len == 10U) {
				sep = str[2U];
				break;
			}
		default:
			return fn_nul_bid;
		}
		if (!DIGITP(str[3U]) || !DIGITP(str[4U])) {
			return fn_nul_bid;
		} else if (str[5U] != sep) {
			return fn_nul_bid;
		} else if (!DIGITP(str[6U]) || !DIGITP(str[7U]) ||
			   !DIGITP(str[8U]) || !DIGITP(str[9U])) {
			return fn_nul_bid;
		} else if (!check_y(str + 6U)) {
			return fn_nul_bid;
		} else if (!check_m(str + 0U) && !check_d(str + 3U)) {
			return fn_nul_bid;
		} else if (!check_d(str + 0U) || !check_d(str + 3U)) {
			return fn_nul_bid;
		}
	}

	/* bid just any number really */
	return (fn_bid_t){FINNER_DATE};
}

/* date.c ends here */
