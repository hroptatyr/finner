/*** phash.c -- perfect hasher for 3 letter codes
 *
 * Copyright (C) 2016-2017 Sebastian Freundt
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
/***
 * The input to this compiler is of the form
 *
 *   CCC
 *   ...
 *
 * so for instance
 *
 *   MON
 *   TUE
 *
 * and this compiler will emit a C file with the following routine
 *
 *   static bool valid_cc_p(const char *str);
 *
 * that can be used to query whether the code in STR has been
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

static int
ccl_add(const char ln[static 3U], size_t len)
{
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

	/* otherwise add */
	lst[lnt].cc[0U] = ln[0U];
	lst[lnt].cc[1U] = ln[1U];
	lst[lnt].cc[2U] = ln[2U];
	lst[lnt].cc[3U] = '\0';
	lnt++;
	return 1;
}

int
main(int argc, char *argv[])
{
/* pretty much like cccc but for 3 letter combos that aren't prefix-free */
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

	puts("\
static bool\n\
valid_cc_p(const char cc[static 2U])\n\
{\n\
#define C2I(x)			((x) - 'A')\n\
#define BEGINNING_WITH(x)	[C2I(x)]\n\
#define ALLOW(x)		(1U << (x))\n\
#define AND			|\n\
	static const uint_fast32_t _cc[] = {");

	for (char c = 'A'; c <= 'Z'; c++) {
		printf("\
		BEGINNING_WITH('%c') = ", c);
		for (size_t i = 0U; i < lnt; i++) {
			if (lst[i].cc[0U] == c) {
				printf("ALLOW('%c'^'%c') AND ", lst[i].cc[1U], lst[i].cc[2U]);
			}
		}
		puts("0U,");
	}
	puts("\
	};");
	puts("\
\n\
	if (cc[0U] >= 'A' && cc[0U] <= 'Z' &&\n\
	    cc[1U] >= 'A' && cc[1U] <= 'Z' &&\n\
	    cc[2U] >= 'A' && cc[2U] <= 'Z') {\n\
		return (_cc BEGINNING_WITH(cc[0U]) & ALLOW(cc[1U] ^ cc[2U]));\n\
	}");

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

/* phash.c */
