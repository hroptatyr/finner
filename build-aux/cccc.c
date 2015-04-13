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
 *   CC [\t LEN]
 *   ...
 *
 * so for instance
 *
 *   DE	17
 *   AU	22
 *
 * and this compiler will emit a C file with the following routine
 *
 *   static bool valid_cc_p(const char *str);
 *
 * that can be used to query whether the country code in STR has been
 * mentioned in the input.  While the length argument is optional, if
 * any of the lines in the input contains a length, the following
 * routine will also be emitted:
 *
 *   static unsigned int cc_len(const char *str);
 *
 * that maps a valid country code to its length argument.  It won't
 * check for invalid country codes though. */
#ifdef HAVE_CONFIG_H
# include "config.h"
#endif	/* HAVE_CONFIG_H */
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

struct cclen_s {
	char cc[2U];
	uint_fast8_t len;
};

static struct cclen_s *lst;
static size_t lnt;
static size_t lzt;
static bool has_lens;

static int
ccl_add(const char ln[static 2U], size_t len)
{
	long unsigned int l;
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

	/* read the length argument */
	if ((l = strtoul(ln + 2, NULL, 10))) {
		has_lens = true;
	}

	/* check it's not there already */
	for (i = 0U; i < lnt; i++) {
		if (lst[i].cc[0U] == ln[0U] &&
		    lst[i].cc[1U] == ln[1U]) {
			/* yep, there he is */
			lst[i].len = (uint_fast8_t)l;
			return 0;
		}
	}
	/* otherwise add */
	lst[i].cc[0U] = ln[0U];
	lst[i].cc[1U] = ln[1U];
	lst[i].len = (uint_fast8_t)l;
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
#include <stdint.h>\n\
\n\
static bool\n\
valid_cc_p(const char cc[static 2U])\n\
{\n\
#define C2I(x)			((x) - 'A')\n\
#define BEGINNING_WITH(x)	[C2I(x)] =\n\
#define ALLOW(x)		(1U << C2I(x))\n\
#define AND			|\n\
	static const uint_fast32_t _cc[] = {");

	for (char c = 'A'; c <= 'Z'; c++) {
		printf("\
		BEGINNING_WITH('%c')\t", c);
		for (size_t i = 0U; i < lnt; i++) {
			if (lst[i].cc[0U] == c) {
				printf("ALLOW('%c') AND ", lst[i].cc[1U]);
			}
		}
		puts("0U,");
	}
	puts("\
	};\n\
\n\
	if (cc[0U] >= 'A' && cc[0U] <= 'Z' &&\n\
	    cc[1U] >= 'A' && cc[1U] <= 'Z') {\n\
		return (_cc[C2I(cc[0U])] & ALLOW(cc[1U]));\n\
	}\n\
	return false;\n\
#undef C2I\n\
#undef BEGINNING_WITH\n\
#undef ALLOW\n\
#undef AND\n\
}");

	if (has_lens) {
		unsigned off = 0U;

		puts("\
static unsigned int\n\
cc_len(const char cc[static 2U])\n\
{\n\
#define C2I(x)			((x) - 'A')\n\
#define COUNTRIES_WITH(x)	[C2I(x) * 2U] =\n\
#define THERE_ARE(x)		x\n\
#define AT(x)			, 2U * x\n\
	static uint_fast16_t _offs[] = {");

		for (char c = 'A'; c <= 'Z'; c++) {
			unsigned cnt = 0U;
			for (size_t i = 0U; i < lnt; i++) {
				if (lst[i].cc[0U] == c) {
					cnt++;
				}
			}
			printf("\
		COUNTRIES_WITH('%c') THERE_ARE(%u) AT(%u),\n", c, cnt, off);
			off += cnt;
		}

		puts("\
	};\n\
	static uint_fast8_t _cc[] = {");

		/* the most idiotic sorting ever */
		for (char c1 = 'A'; c1 <= 'Z'; c1++) {
			printf("\
		/* %c */\n\
		", c1);
			for (char c2 = 'A'; c2 <= 'Z'; c2++) {
				for (size_t i = 0U; i < lnt; i++) {
					if (lst[i].cc[0U] == c1 &&
					    lst[i].cc[1U] == c2) {
						printf("\
'%c', ", c2);
					}
				}
			}
			for (char c2 = 'A'; c2 <= 'Z'; c2++) {
				for (size_t i = 0U; i < lnt; i++) {
					if (lst[i].cc[0U] == c1 &&
					    lst[i].cc[1U] == c2) {
						printf("\
%hhu, ", lst[i].len);
					}
				}
			}
			putchar('\n');
		}
		puts("\
	};\n\
	const unsigned int off = _offs[C2I(cc[0U]) * 2U + 1U];\n\
	const unsigned int len = _offs[C2I(cc[0U]) * 2U + 0U];\n\
	unsigned i;\n\
\n\
	for (i = off; i < off + len; i++) {\n\
		if (_cc[i] == cc[1U]) {\n\
			return _cc[i + len];\n\
		}\n\
	}\n\
	return 0;\n\
#undef C2I\n\
#undef COUNTRIES_WITH\n\
#undef THERE_ARE\n\
#undef AT\n\
}");
	}

	if (lst) {
		free(lst);
	}
	return 0;
}

/* cccc.c */
