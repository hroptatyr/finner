/*** finner.c -- main snippet
 *
 * Copyright (C) 2014-2015 Sebastian Freundt
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
#if defined HAVE_CONFIG_H
# include "config.h"
#endif	/* HAVE_CONFIG_H */
#if defined HAVE_VERSION_H
# include "version.h"
#endif	/* HAVE_VERSION_H */
#include <unistd.h>
#include <stdlib.h>
#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <assert.h>
#include "coru.h"
#include "nifty.h"

struct anno_s {
	size_t sta;
	size_t end;
};


static void
__attribute__((format(printf, 1, 2)))
error(const char *fmt, ...)
{
	va_list vap;
	va_start(vap, fmt);
	vfprintf(stderr, fmt, vap);
	va_end(vap);
	if (errno) {
		fputc(':', stderr);
		fputc(' ', stderr);
		fputs(strerror(errno), stderr);
	}
	fputc('\n', stderr);
	return;
}


struct co_snarf_retval_s {
	const char *buf;
	size_t bsz;
};

DEFCORU(co_snarf, {int fd;}, void *UNUSED(arg))
{
	/* upon the first call we expect a completely processed buffer
	 * just to determine the buffer's size */
	const int fd = CORU_CLOSUR(fd);
	static char buf[64U * 1024U];
	static struct co_snarf_retval_s ret = {buf};
	ssize_t npr = sizeof(buf);
	ssize_t nrd;

	/* leave some good advice about our access pattern */
	posix_fadvise(fd, 0, 0, POSIX_FADV_WILLNEED);
	posix_fadvise(fd, 0, 0, POSIX_FADV_SEQUENTIAL);

	/* enter the main snarf loop */
	while ((nrd = read(fd, buf + ret.bsz, sizeof(buf) - ret.bsz)) > 0) {
		/* we've got NRD more unprocessed bytes */
		ret.bsz += nrd;

		/* process */
		npr = YIELD(&ret);
		/* now it's NPR less unprocessed bytes */
		ret.bsz -= npr;

		/* check if we need to move buffer contents */
		if (ret.bsz > 0) {
			memmove(buf, buf + npr, ret.bsz);
		}
	}
	/* final drain */
	if (ret.bsz) {
		/* we don't care how much got processed */
		YIELD(&ret);
	}
	return 0;
}

struct co_terms_retval_s {
	const char *base;
	size_t nannos;
	struct anno_s bbox;
	struct anno_s annos[];
};

DEFCORU(co_terms, {}, void *arg)
{
	const struct co_snarf_retval_s *rd = arg;
	struct co_terms_retval_s *rv;
	size_t rz;

	/* start out with a reasonable number of annos */
	rv = calloc(4096U, sizeof(*rv));
	rz = 4096U - 1U/*hdr*/ - 1U/*bbox*/;

	/* enter the main snarf loop */
	do {
		/* this is a simple state machine,
		 * we start at NONE and wait for an ALNUM,
		 * in state ALNUM we can either go back to NONE (and yield) if
		 * neither a punct nor an alnum is read, or we go forward to
		 * PUNCT in state PUNCT we can either go back to NONE (and
		 * yield) if neither a punct nor an alnum is read, or we go back
		 * to ALNUM hand over to our friendly termify routine */
		enum state_e {
			ST_NONE,
			ST_SEEN_ALNUM,
			ST_SEEN_PUNCT,
		} st = ST_NONE;
		enum {
			CLS_UNK,
			CLS_PUNCT,
			CLS_SPACE,
			CLS_ALNUM,
		} cl;
		const char *bp = rd->buf;
		size_t ia = 0U;

		for (const char *ap, *fp, *const ep = rd->buf + rd->bsz;
		     bp < ep; bp++) {
			if (UNLIKELY(*bp < 0)) {
				cl = CLS_UNK;
			} else if (*bp <= 0x20) {
				cl = CLS_SPACE;
			} else if (*bp < 0x30) {
				cl = CLS_PUNCT;
			} else if (*bp < 0x3a) {
				cl = CLS_ALNUM;
			} else if (*bp < 0x40) {
				cl = CLS_PUNCT;
			} else if (*bp < 0x5b) {
				cl = CLS_ALNUM;
			} else if (*bp < 0x61) {
				cl = CLS_PUNCT;
			} else if (*bp < 0x7b) {
				cl = CLS_ALNUM;
			} else {
				cl = CLS_PUNCT;
			}

			switch (st) {
			case ST_NONE:
				switch (cl) {
				case CLS_ALNUM:
				case CLS_UNK:
					/* start the machine */
					st = ST_SEEN_ALNUM;
				default:
					ap = bp;
				}
				break;

			case ST_SEEN_ALNUM:
				switch (cl) {
				case CLS_PUNCT:
					/* better record the preliminary
					 * end-of-streak */
					st = ST_SEEN_PUNCT;
					fp = bp;
				default:
					break;
				case CLS_SPACE:
					fp = bp;
					goto yield;
				}
				break;

			case ST_SEEN_PUNCT:
				switch (cl) {
				default:
					/* aah, good one */
					st = ST_SEEN_ALNUM;
				case CLS_PUNCT:
					/* 2 puncts in a row,
					 * not on my account */
					break;
				case CLS_SPACE:
					goto yield;
				}
				break;

			yield:
				if (UNLIKELY(ia >= rz)) {
					size_t nu = (rz + 2U) * 2U;

					rv = realloc(rv, nu * sizeof(*rv));
					rz = nu - 2U;
				}
				rv->annos[ia].sta = ap - rd->buf;
				rv->annos[ia].end = fp - rd->buf;
				ia++;

				st = ST_NONE;
				ap = bp;
				fp = NULL;
				break;
			}
		}

		/* set up result */
		rv->base = rd->buf;
		if (UNLIKELY(!(rv->nannos = ia))) {
			rv->bbox.sta = 0U;
			rv->bbox.end = bp - rd->buf;
		} else {
			rv->bbox.sta = rv->annos[0U].sta;
			rv->bbox.end = rv->annos[ia - 1U].end;
		}
	} while ((rd = (const void*)YIELD(rv)));
	return 0;
}


static int
annotate1(const char *fn)
{
	struct cocore *snarf;
	struct cocore *terms;
	struct cocore *self;
	int rc = 0;
	int fd;

	if (fn == NULL) {
		fd = STDIN_FILENO;
		fn = "-";
	} else if ((fd = open(fn, O_RDONLY)) < 0) {
		error("Error: cannot open file `%s'", fn);
		return -1;
	}

	self = PREP();
	snarf = START_PACK(co_snarf, .next = self, .clo = {.fd = fd});
	terms = START_PACK(co_terms, .next = self);

	/* ping-pong relay between the corus
	 * technically we could let the corus flip-flop call each other
	 * but we'd like to filter bad input right away */
	const struct co_snarf_retval_s *rd = NULL;
	const struct co_terms_retval_s *ta = NULL;
	for (ssize_t npr = 0;; npr = ta->bbox.end) {
		if ((rd = NEXT1(snarf, npr)) == NULL) {
			break;
		} else if ((ta = NEXT1(terms, rd)) == NULL) {
			break;
		}
	}

	/* clean up */
	close(fd);
	return rc;
}


#include "finner.yucc"

static int
cmd_anno(struct yuck_cmd_anno_s argi[static 1U])
{
	size_t i = 0U;
	int rc = 0;

	if (!argi->nargs) {
		goto one_off;
	}
	for (; i < argi->nargs; i++) {
	one_off:
		rc |= annotate1(argi->args[i]);
	}
	return rc & 1;
}

int
main(int argc, char *argv[])
{
	yuck_t argi[1U];
	int rc = 0;

	if (yuck_parse(argi, argc, argv) < 0) {
		rc = 1;
		goto out;
	}

	/* get the coroutines going */
	initialise_cocore();

	switch (argi->cmd) {
	case FINNER_CMD_ANNO:
		rc = cmd_anno((void*)argi);
		break;

	default:
		break;
	}

out:
	yuck_free(argi);
	return rc;
}

/* finner.c ends here */
