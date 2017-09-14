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
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <assert.h>
#include <stdint.h>
#include "finner.h"
#include "bidder.h"
#include "collector.h"
#include "nifty.h"

struct co_terms_retval_s;

struct ctx_s {
	bool allp;
	size_t abs;
	void(*prntf)(const struct ctx_s*, const struct co_terms_retval_s*);
};

_Static_assert(
	sizeof(fn_bid_t) == 2U * sizeof(uintptr_t),
	"possible size problem with fn_bid_t");


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

static inline size_t
ROUNDLEASTTO(size_t x, size_t least, size_t n)
{
/* round X (but at least LEAST) to N */
	if (x >= least) {
		return ROUNDTO(x, n);
	}
	return ROUNDTO(least, n);
}


static const struct co_snarf_retval_s {
	const char *buf;
	size_t bsz;
} *co_snarf(const int fd, size_t npr)
{
	/* upon the first call we expect a completely processed buffer
	 * just to determine the buffer's size */
	static char buf[64U * 1024U];
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
		/* check if we need to move buffer contents */
		if (ret.bsz > 0) {
			memmove(buf, buf + npr, ret.bsz);
		}

		nrd = read(fd, buf + ret.bsz, sizeof(buf) - ret.bsz);
		if (nrd > 0) {
			/* we've got NRD more unprocessed bytes */
			ret.bsz += nrd;

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

	case SNARF_POST:
		st = SNARF_NONE;
		ret.bsz = 0U;
		break;
	}
	return 0;
}

static const struct co_terms_retval_s {
	const char *base;
	size_t nannos;
	extent_t bbox;
	struct anno_s annos[];
} *co_terms(const struct co_snarf_retval_s *rd)
{
	static struct co_terms_retval_s *rv;
	static size_t rz;
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
		CLS_TRSEP,
		CLS_ALNUM,
	} cl;
	const char *bp = rd->buf;
	const char *ap = rd->buf;
	const char *fp = rd->buf;
	size_t ia = 0U;
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
		return NULL;
	}

	for (const char *const ep = rd->buf + rd->bsz; bp < ep; bp++) {
		if (UNLIKELY(*bp < 0)) {
			cl = CLS_UNK;
		} else if (*bp <= 0x20) {
			cl = CLS_TRSEP;
		} else if (*bp <= 0x22) {
			cl = CLS_TRSEP;
		} else if (*bp <= 0x25) {
			cl = CLS_ALNUM;
		} else if (*bp == '+') {
			cl = CLS_ALNUM;
		} else if (*bp < 0x30) {
			cl = CLS_PUNCT;
		} else if (*bp < 0x3a) {
			cl = CLS_ALNUM;
		} else if (*bp < 0x40) {
			cl = CLS_TRSEP;
		} else if (*bp < 0x5b) {
			cl = CLS_ALNUM;
		} else if (*bp < 0x5e) {
			cl = CLS_TRSEP;
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
			case CLS_TRSEP:
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
			case CLS_TRSEP:
				goto yield;
			}
			break;

		yield:
			if (UNLIKELY(ia >= rz)) {
				/* resize */
				const size_t olz = rz + TERMS_EXTRA;
				const size_t nuz = ROUNDTO(2U * olz, 4096U);

				rv = recalloc(rv, olz, nuz, sizeof(*rv->annos));
				rz = nuz - TERMS_EXTRA;
			}
			rv->annos[ia].x.sta = ap - rd->buf;
			rv->annos[ia].x.end = fp - rd->buf;
			ia++;

			st = ST_NONE;
			ap = bp;
			fp = NULL;
			break;
		}
	}

	/* set up result */
	rv->base = rd->buf;
	if (LIKELY((rv->nannos = ia))) {
		/* calculate proper bounding box */
		rv->bbox.sta = 0U;
		rv->bbox.end = ap - rd->buf;
	} else {
		/* include everything in the bounding box */
		rv->bbox.sta = 0U;
		rv->bbox.end = bp - rd->buf;
	}
	return rv;
#undef TERMS_EXTRA
}

/* bidders or higher level stuff that takes a terms retval
 *
 * We store the bids as they come in, so the resulting BIDS array
 * coincides with the terms retval array TA->ANNOS.
 *
 * Terms for which a bidder returned FINNER_TERM (or FINNER_CUT)
 * with a STATE != 0 are cut in halves at STATE bytes.  The STATE
 * slot is then reused to point to the index of the newly created
 * half-term (behind anything in TA->ANNOS).  The second half is
 * stored in the slot behind it. */
static const struct co_terms_retval_s*
co_tbids(const struct co_terms_retval_s *ta)
{
	static struct co_terms_retval_s *rv;
	static size_t rz;
	size_t nb = 0U;

#define TBIDS_EXTRA	(sizeof(*rv) / sizeof(*rv->annos))
	_Static_assert(TBIDS_EXTRA > 0U, "tbids array type is bigger than header");

	if (UNLIKELY(rv == NULL && ta == NULL)) {
		/* just return */
		return NULL;
	} else if (UNLIKELY(ta == NULL)) {
		free(rv);
		rv = NULL;
		return NULL;
	} else if (UNLIKELY(ta->nannos > rz)) {
		/* scale to number of annos */
		size_t nuz = ROUNDTO(ta->nannos + TBIDS_EXTRA, 4096U);

		rv = recalloc(rv, rz + TBIDS_EXTRA, nuz, sizeof(*rv->annos));
		rz = nuz - TBIDS_EXTRA;
	}

	for (size_t i = 0U; i < ta->nannos; i++) {
		extent_t x = ta->annos[i].x;
		const char *tp = ta->base + x.sta;
		size_t tz = x.end - x.sta;
		fn_bid_t b;

	rebid:
		/* resize? */
		if (nb > rz) {
			const size_t olz = rz + TBIDS_EXTRA;
			const size_t nuz = ROUNDTO(2U * olz, 4096U);

			rv = recalloc(rv, olz, nuz, sizeof(*rv->annos));
			rz = nuz - TBIDS_EXTRA;
		}

		/* get ourselves some bids */
		b = finner_bid(tp, tz);

		/* sanitise and maybe cut the whole thing */
		if (!b.bid) {
			if (UNLIKELY(b.leftover || b.state)) {
				/* that's rubbish */
				b = fn_nul_bid;
			}
		} else if (b.leftover) {
			const size_t cz = b.leftover;

			if (LIKELY(cz < tz)) {
				/* cut! put this bid into half-term 1 */
				rv->annos[nb].x = (extent_t){x.sta, x.end - cz};
				rv->annos[nb++].b = b;
				/* massage pointers for the other half */
				x.sta += tz - cz;
				tp += tz - cz;
				tz = cz;
				goto rebid;
			} else {
				/* otherwise it's rubbish again */
				b = fn_nul_bid;
			}
		}

		/* assign and move on */
		rv->annos[nb].x = x;
		rv->annos[nb++].b = b;
	}
	/* assign our findings, and copy base and bbox */
	rv->base = ta->base;
	rv->nannos = nb;
	rv->bbox = ta->bbox;
	return rv;
#undef TBIDS_EXTRA
}

/* collectors
 *
 * Go through bids present in TB and merge adjacent ones into an
 * annotated collection. */
static const struct co_terms_retval_s*
co_tcoll(const struct co_terms_retval_s *tb)
{
	static struct co_terms_retval_s *rv;
	static size_t rz;
	size_t nc = 0U;
	size_t lastb = 0U;

#define TCOLL_EXTRA	(sizeof(*rv) / sizeof(*rv->annos))
	_Static_assert(TCOLL_EXTRA > 0U, "tcoll array type is bigger than header");

	if (UNLIKELY(rv == NULL && tb == NULL)) {
		/* just return */
		return NULL;
	} else if (UNLIKELY(tb == NULL)) {
		free(rv);
		rv = NULL;
		return NULL;
	} else if (UNLIKELY(rv == NULL)) {
		/* get a comfy cushion */
		rv = calloc(4096U, sizeof(*rv->annos));
		rz = 4096U - TCOLL_EXTRA;
	}

	/* preset RV with goodness */
	rv->base = tb->base;
	rv->bbox = tb->bbox;
	for (size_t i = 0U; i < tb->nannos; i++) {
		const struct anno_s *const av = tb->annos + i;
		const size_t na = tb->nannos - i;
		fn_bid_t c;

		if (!av->b.bid) {
			continue;
		} else if (!(c = finner_collect(av, na)).bid) {
			continue;
		} else if (i == 0U && UNLIKELY(!c.span)) {
			/* huh?  this must be broken */
			continue;
		}
		/* copy all tokens from LASTB to I, if space there is */
		if (nc + (i - lastb) + c.span + 1U > rz) {
			/* resize */
			const size_t olz = rz + TCOLL_EXTRA;
			const size_t least = nc + (i - lastb) + c.span + 1U;
			const size_t nuz = ROUNDLEASTTO(2U * olz, least, 4096U);

			rv = recalloc(rv, olz, nuz, sizeof(*rv->annos));
			rz = nuz - TCOLL_EXTRA;
		}
		memcpy(rv->annos + nc,
		       tb->annos + lastb, (i - lastb) * sizeof(*rv->annos));
		/* adjust */
		nc += (i - lastb);
		lastb = i;

		if (UNLIKELY(!c.span)) {
			/* we've got to cut this short */
			rv->bbox.end = tb->annos[i - 1U].x.end;
			break;
		}
		/* merge the extents */
		rv->annos[nc].x.sta = tb->annos[i].x.sta;
		rv->annos[nc].x.end = tb->annos[i + c.span - 1U].x.end;
		/* now copy the beef */
		rv->annos[nc].b = c;
		/* that's it */
		nc++;
		i += c.span - 1U;
	}
	/* special case when there was no copying at all */
	if (!nc) {
		return tb;
	}
	/* copy the rest of the annos */
	if (nc + (tb->nannos - lastb) > rz) {
		/* resize */
		const size_t olz = rz + TCOLL_EXTRA;
		const size_t least = nc + (tb->nannos - lastb);
		const size_t nuz = ROUNDLEASTTO(2U * olz, least, 4096U);

		rv = recalloc(rv, olz, nuz, sizeof(*rv->annos));
		rz = nuz - TCOLL_EXTRA;
	}
	memcpy(rv->annos + nc,
	       tb->annos + lastb, (tb->nannos - lastb) * sizeof(*rv->annos));
	/* adjust */
	nc += (tb->nannos - lastb);

	/* assign our findings, and copy base and bbox */
	rv->nannos = nc;
	return rv;
#undef TCOLL_EXTRA
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
		} else if (b.bid || ctx->allp) {
			if (b.bid < FINNER_NTOKENS) {
				/* primitive i.e. non-collective token */
				fwrite(tp, sizeof(*tp), tz, stdout);
			} else if (i + b.span < ta->nannos) {
				for (size_t j = 1U; j < b.span; j++) {
					const extent_t px = ta->annos[i + j].x;
					const char *pp = ta->base + px.sta;
					const size_t pz = px.end - px.sta;

					fwrite(pp, sizeof(*pp), pz, stdout);
					fputc(' ', stdout);
				}
				/* write final one */
				const extent_t px = ta->annos[i + b.span].x;
				const char *pp = ta->base + px.sta;
				const size_t pz = px.end - px.sta;

				fwrite(pp, sizeof(*pp), pz, stdout);
			}
			fprintf(stdout, "\t%s\t[%zu,%zu)\n",
				finner_bidstr[b.bid],
				x.sta + ctx->abs, x.end + ctx->abs);
		}
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
		} else if (b.bid && this >= last) {
			/* print from last streak to here */
			const size_t llen = this - last;
			const size_t tlen = x.end - this;

			fwrite(ta->base + last, sizeof(char), llen, stdout);

			colourp && fputs("\x1b[1m", stdout);
			fwrite(ta->base + this, sizeof(char), tlen, stdout);
			colourp && fputs("\x1b[0;2m", stdout);
			fputc('/', stdout);
			fputs(finner_bidstr[b.bid], stdout);
			if (b.state && finner_statestr[b.bid]) {
				fputc('(', stdout);
				fputs(finner_statestr[b.bid](b.state), stdout);
				fputc(')', stdout);
			}
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
	const struct co_snarf_retval_s *rd = NULL;
	const struct co_terms_retval_s *tv = NULL;
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
	for (ssize_t npr = 0;; ctxcpy.abs += (npr = tv->bbox.end)) {
		if ((rd = co_snarf(fd, npr)) == NULL) {
			break;
		} else if ((tv = co_terms(rd)) == NULL) {
			break;
		} else if ((tv = co_tbids(tv)) == NULL) {
			break;
		} else if ((tv = co_tcoll(tv)) == NULL) {
			break;
		} else if (ctx->prntf(&ctxcpy, tv), 0) {
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
