/*** cccc.c -- country code compiler
 *
 * Copyright (C) 2015 Sebastian Freundt
 *
 * Author:  Sebastian Freundt <freundt@ga-group.nl>
 *
 * This file is part of numchk.
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
/***
 * The input to this compiler is of the form
 *
 *   CC[C]
 *   ...
 *
 * so for instance
 *
 *   DEM
 *   AUD
 *
 * and this compiler will emit a C file with the following routine
 *
 *   static bool valid_cc_p(const char *str);
 *
 * that can be used to query whether the country code in STR has been
 * mentioned in the input. */
#ifdef HAVE_CONFIG_H
# include "config.h"
#endif	/* HAVE_CONFIG_H */
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

struct cclen_s {
	char cc[4U];
};

static struct cclen_s *lst;
static size_t lnt;
static size_t lzt;
static bool has_3chars;

static int
ccl_add(const char ln[static 2U], size_t len)
{
	size_t i;

	(void)len;
	if (lnt >= lzt) {
		size_t nuz = (lnt * 2U) ?: 64U;

		if ((lst = realloc(lst, nuz * sizeof(*lst))) == NULL) {
			fputs("\
fatal: insufficient memory to compile the input\n", stderr);
			exit(1);
		}
		lzt = nuz;
	}

	if (ln[2U] >= 'A' && ln[2U] <= 'Z') {
		has_3chars = true;
	}

	/* check it's not there already */
	for (i = 0U; i < lnt; i++) {
		if (lst[i].cc[0U] == ln[0U] &&
		    lst[i].cc[1U] == ln[1U]) {
			/* yep, there he is */
			lst[i].cc[2U] = ln[2U];
			return 0;
		}
	}
	/* otherwise add */
	lst[i].cc[0U] = ln[0U];
	lst[i].cc[1U] = ln[1U];
	lst[i].cc[2U] = ln[2U];
	lst[i].cc[3U] = '\0';
	lnt++;
	return 1;
}

int
main(int argc, char *argv[])
{
	char *line = NULL;
	size_t llen = 0UL;

#if defined HAVE_GETLINE
	for (ssize_t nrd; (nrd = getline(&line, &llen, stdin)) > 0;) {
		ccl_add(line, nrd);
	}
	free(line);
#elif defined HAVE_FGETLN
	while ((line = fgetln(stdin, &llen)) != NULL) {
		ccl_add(line, llen);
	}
#else
	fputs("\
error: cannot read from stdin\n\
getline() or fgetln() support missing.\n", stderr);
	exit(1);
#endif	/* GETLINE/FGETLN */

	puts("\
#include <stdbool.h>\n\
#include <stdint.h>\n");

	if (has_3chars) {
		/* three letter codes, we need popcnt */
		puts("\
static unsigned int\n\
xpopcnt(uint_fast32_t x)\n\
{\n\
	x = x - ((x >> 1U) & 0x55555555U);\n\
	x = (x & 0x33333333U) + ((x >> 2U) & 0x33333333U);\n\
	x = ((x + (x >> 4U) & 0xF0F0F0FU) * 0x1010101U) >> 24U;\n\
	return x & 0x1fU;\n\
}\n");
	}

	puts("\
static bool\n\
valid_cc_p(const char cc[static 2U])\n\
{\n\
#define C2I(x)			((x) - 'A')\n\
#define BEGINNING_WITH(x)	[C2I(x)]\n\
#define ALLOW(x)		(1U << C2I(x))\n\
#define AND			|\n\
	static const uint_fast32_t _cc[] = {");

	for (char c = 'A'; c <= 'Z'; c++) {
		printf("\
		BEGINNING_WITH('%c') = ", c);
		for (size_t i = 0U; i < lnt; i++) {
			if (lst[i].cc[0U] == c) {
				printf("ALLOW('%c') AND ", lst[i].cc[1U]);
			}
		}
		puts("0U,");
	}
	puts("\
	};");
	if (has_3chars) {
		/* three-letter codes */
		puts("\
	static const struct {\n\
		char _c[16U];\n\
	} _c3[] = {");
		for (char c = 'A'; c <= 'Z'; c++) {
			printf("\
		BEGINNING_WITH('%c') = {\"", c);
			for (size_t i = 0U; i < lnt; i++) {
				if (lst[i].cc[0U] == c) {
					printf("%c", lst[i].cc[2U]);
				}
			}
			puts("\"},");
		}
		puts("\
	};");
		puts("\
\n\
	if (cc[0U] >= 'A' && cc[0U] <= 'Z' &&\n\
	    cc[1U] >= 'A' && cc[1U] <= 'Z') {\n\
		uint_fast32_t v = _cc BEGINNING_WITH(cc[0U]);\n\
		if (v & ALLOW(cc[1U])) {\n\
			uint_fast32_t m = v & (ALLOW(cc[1U]) - 1U);\n\
			unsigned int pc = xpopcnt(m);\n\
			return cc[2U] == _c3 BEGINNING_WITH(cc[0U])._c[pc];\n\
		}\n\
	}");
	} else {
		/* two-letter codes */
		puts("\
\n\
	if (cc[0U] >= 'A' && cc[0U] <= 'Z' &&\n\
	    cc[1U] >= 'A' && cc[1U] <= 'Z') {\n\
		return (_cc BEGINNING_WITH(cc[0U]) & ALLOW(cc[1U]));\n\
	}");
	}
	puts("\
	return false;\n\
#undef C2I\n\
#undef BEGINNING_WITH\n\
#undef ALLOW\n\
#undef AND\n\
}");

	if (lst) {
		free(lst);
	}
	return 0;
}

/* cccc.c */
