/*** finner.c -- main snippet
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
#if defined HAVE_CONFIG_H
# include "config.h"
#endif	/* HAVE_CONFIG_H */
#if defined HAVE_VERSION_H
# include "version.h"
#endif	/* HAVE_VERSION_H */
#include <unistd.h>
#include <stdlib.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <assert.h>
#include <stdint.h>
#include "finner.h"
#include "nifty.h"

struct co_terms_retval_s;

struct ctx_s {
	bool allp;
	size_t abs;
	void(*prntf)(const struct ctx_s*, const struct co_terms_retval_s*);
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

static inline void*
recalloc(void *old, size_t oldnmemb, size_t newnmemb, size_t size)
{
	old = realloc(old, newnmemb * size);
	memset((char*)old + oldnmemb * size, 0, (newnmemb - oldnmemb) * size);
	return old;
}

static inline size_t
ROUNDTO(size_t x, size_t n)
{
/* round X to N */
	return (x + (n - 1U)) & ~(size_t)(n - 1U);
}

static inline __attribute__((unused)) size_t
ROUNDLEASTTO(size_t x, size_t least, size_t n)
{
/* round X (but at least LEAST) to N */
	if (x >= least) {
		return ROUNDTO(x, n);
	}
	return ROUNDTO(least, n);
}


#include "finner.rlc"

static const struct co_snarf_retval_s {
	const char *buf;
	size_t bsz;
} *co_snarf(const int fd, size_t npr)
{
	/* upon the first call we expect a completely processed buffer
	 * just to determine the buffer's size */
	static char buf[64U * 1024U + 1U];
	static struct co_snarf_retval_s ret = {buf};
	static enum {
		SNARF_NONE,
		SNARF_READ,
		SNARF_POST,
	} st;
	ssize_t nrd;

	switch (st) {
	case SNARF_NONE:
		/* leave some good advice about our access pattern */
		posix_fadvise(fd, 0, 0, POSIX_FADV_WILLNEED);
		posix_fadvise(fd, 0, 0, POSIX_FADV_SEQUENTIAL);
		/* change state */
		st = SNARF_READ;

	case SNARF_READ:
		/* now it's NPR less unprocessed bytes */
		ret.bsz -= npr;
		/* trim full buffers */
		ret.bsz &= sizeof(buf) - 1U - 1U;

		/* check if we need to move buffer contents */
		if (ret.bsz > 0) {
			memmove(buf, buf + npr, ret.bsz);
		}

		nrd = read(fd, buf + ret.bsz, sizeof(buf) - 1U - ret.bsz);
		if (nrd > 0) {
			/* we've got NRD more unprocessed bytes */
			ret.bsz += nrd;

			/* as a service mark the eob */
			buf[ret.bsz] = eob;

			/* process */
			return &ret;
		}
		/* change state */
		st = SNARF_POST;
		/* final drain */
		if (ret.bsz) {
			/* we don't care how much got processed */
			return &ret;
		}
		/* add npr again so the next one doesn't underflow */
		ret.bsz += npr;
	case SNARF_POST:
		/* now it's NPR less unprocessed bytes */
		ret.bsz -= npr;
		/* check if we need to move buffer contents */
		if (ret.bsz) {
			memmove(buf, buf + npr, ret.bsz);
			/* process */
			return &ret;
		}
		st = SNARF_NONE;
		ret.bsz = 0U;
		break;
	}
	return 0;
}

static struct co_terms_retval_s {
	union {
		struct {
			const char *base;
			size_t nannos;
			extent_t bbox;
		};
		anno_t _algn_;
	};
	anno_t annos[];
} *co_terms(const struct co_snarf_retval_s *rd)
{
	static struct co_terms_retval_s *rv;
	static size_t rz;
#define TERMS_EXTRA	(sizeof(*rv) / sizeof(*rv->annos))

	_Static_assert(TERMS_EXTRA > 0U, "terms array type is bigger than header");

	if (UNLIKELY(rv == NULL && rd == NULL)) {
		/* just return */
		return NULL;
	} else if (UNLIKELY(rv == NULL)) {
		/* start out with a reasonable number of annos */
		rv = calloc(4096U, sizeof(*rv->annos));
		rz = 4096U - TERMS_EXTRA;
	} else if (UNLIKELY(rd == NULL)) {
		free(rv);
		rv = NULL;
		rz = 0UL;
		return NULL;
	}

	const char *bp = rd->buf;
	const char *const ep = rd->buf + rd->bsz;
	rv->nannos = 0U;
	while (bp < ep) {
		anno_t a = terms1(&bp, rd->buf, ep);
		if (a.b.state < 0) {
			continue;
		}
		/* otherwise store extent */
		if (UNLIKELY(rv->nannos >= rz)) {
			/* resize */
			const size_t olz = rz + TERMS_EXTRA;
			const size_t nuz = ROUNDTO(2U * olz, 4096U);

			rv = recalloc(rv, olz, nuz, sizeof(*rv->annos));
			rz = nuz - TERMS_EXTRA;
		}
		rv->annos[rv->nannos++] = a;
	}

	/* set up result */
	rv->base = rd->buf;
	/* go back to last separator */
	for (; bp > rd->buf && (unsigned char)bp[-1] > ' '; bp--);
	rv->bbox = (extent_t){0U, bp - rd->buf};
	return rv;
#undef TERMS_EXTRA
}

/* terminators */
static void
co_textr(const struct ctx_s *ctx, const struct co_terms_retval_s *ta)
{
	const extent_t bbox = ta->bbox;

	for (size_t i = 0U; i < ta->nannos; i++) {
		const extent_t x = ta->annos[i].x;
		const fn_bid_t b = ta->annos[i].b;
		const char *tp = ta->base + x.sta;
		const size_t tz = x.end - x.sta;

		if (UNLIKELY(x.sta >= bbox.end)) {
			/* stuff beyond the bounding box needs to be fed
			 * into the buffer again in the next iteration */
			break;
		} else if (UNLIKELY(b.state < 0)) {
			/* who put that here? */
			continue;
		}
		/* reproduce the extent */
		fwrite(tp, sizeof(*tp), tz, stdout);
		fputc('\t', stdout);
		fputs(B(b), stdout);
		fputc('\t', stdout);
		fprintf(stdout, "[%zu,%zu)",
			x.sta + ctx->abs, x.end + ctx->abs);
		fputc('\n', stdout);
	}
	return;
}

static void
co_tanno(const struct ctx_s *UNUSED(ctx), const struct co_terms_retval_s *ta)
{
	/* check output device */
	const int colourp = isatty(STDOUT_FILENO);
	const extent_t bbox = ta->bbox;
	size_t last = 0U;

	for (size_t i = 0U; i < ta->nannos; i++) {
		const extent_t x = ta->annos[i].x;
		const fn_bid_t b = ta->annos[i].b;
		const size_t this = x.sta;

		if (UNLIKELY(x.sta >= bbox.end)) {
			/* stuff beyond the bounding box needs to be fed
			 * into the buffer again in the next iteration */
			break;
		} else if (b.state >= 0 && this >= last) {
			/* print from last streak to here */
			const size_t llen = this - last;
			const size_t tlen = x.end - this;

			fwrite(ta->base + last, sizeof(char), llen, stdout);

			colourp && fputs("\x1b[1m", stdout);
			fwrite(ta->base + this, sizeof(char), tlen, stdout);
			colourp && fputs("\x1b[0;2m", stdout);
			fputc('/', stdout);
			fputs(B(b), stdout);
			colourp && fputs("\x1b[0m", stdout);

			last = x.end;
		}
	}
	/* write portion between last and ta->bbox.end */
	fwrite(ta->base + last, sizeof(char), ta->bbox.end - last, stdout);
	return;
}


static int
proc1(const struct ctx_s *ctx, const char *fn)
{
	struct ctx_s ctxcpy = *ctx;
	int rc = 0;
	int fd;

	if (fn == NULL) {
		fd = STDIN_FILENO;
		fn = "-";
	} else if ((fd = open(fn, O_RDONLY)) < 0) {
		error("Error: cannot open file `%s'", fn);
		return -1;
	}

	ctxcpy.abs = 0U;
	for (ssize_t npr = 0;; ctxcpy.abs += npr) {
		const struct co_snarf_retval_s *rd;
		const struct co_terms_retval_s *tv;

		rd = co_snarf(fd, npr);
		tv = co_terms(rd);

		if (UNLIKELY(tv == NULL)) {
			break;
		}

		ctx->prntf(&ctxcpy, tv);
		npr = tv->bbox.end;
	}

	/* clean up */
	close(fd);
	return rc;
}


#include "finner.yucc"

static int
cmd_anno(struct yuck_cmd_anno_s argi[static 1U])
{
	struct ctx_s ctx = {.prntf = co_tanno};
	size_t i = 0U;
	int rc = 0;

	if (!argi->nargs) {
		goto one_off;
	}
	for (; i < argi->nargs; i++) {
	one_off:
		rc |= proc1(&ctx, argi->args[i]);
	}
	return rc & 1;
}

static int
cmd_extr(struct yuck_cmd_extr_s argi[static 1U])
{
	struct ctx_s ctx = {.allp = argi->all_flag, .prntf = co_textr};
	size_t i = 0U;
	int rc = 0;

	if (!argi->nargs) {
		goto one_off;
	}
	for (; i < argi->nargs; i++) {
	one_off:
		rc |= proc1(&ctx, argi->args[i]);
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

	switch (argi->cmd) {
	case FINNER_CMD_ANNO:
		rc = cmd_anno((void*)argi);
		break;

	case FINNER_CMD_EXTR:
		rc = cmd_extr((void*)argi);
		break;

	default:
		break;
	}

out:
	yuck_free(argi);
	return rc;
}

/* finner.c ends here */
