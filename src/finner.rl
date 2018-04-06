/* -*- c -*- */
#include <string.h>
#include <stdio.h>
#include "finner.h"
#include "nifty.h"

#ifdef __INTEL_COMPILER
# pragma warning (disable:2415)
#endif  /* __INTEL_COMPILER */

#define dbgpr(x)	
#define _dbgpr(x)	\
	fwrite(q, 1, p + 1U - q, stderr); \
	fputc('\t', stderr); \
	fputs(#x, stderr); \
	fputc('\n', stderr);

%%{
	machine finner;

	upnum = digit | upper;
	lonum = digit | lower;
	vowel = "A" | "E" | "I" | "O" | "U";
	consonant = upper - vowel;
	sep = "\f" | "\n" | "\r\n" | "\t" | "\v" | " ";

	include finner "ccy.c";
	include finner "ccysym.rl";
	include finner "num.c";
	include finner "unit-1.rl";
	include finner "amt.c";
	include finner "fxpair.c";
	include finner "date.c";
	include finner "time.c";
	include finner "figi.c";
	include finner "isin.c";
	include finner "sedol.c";
	include finner "cusip.c";
	include finner "wkn.c";
	include finner "lei.c";

	finner =
		num |
		date |
		time |
		ccy |
		ccysym |
		fxpair |
		isin |
		figi |
		wkn |
		unit_1 |
		amt |
		cusip |
		sedol |
		lei |
		empty ;

	main := (any* @{q = p + 1U;}) :> ( finner ) ;

	write data;
}%%

static anno_t
terms1(const char **pp, const char *const s, const char *const pe)
{
	const char *p = *pp;
	const char *q = p;
	anno_t r = {-1};
	int cs;

#if 0
	fputs("TERMS1(", stderr);
	fwrite(p, 1, pe - p, stderr);
	fputs(")\n", stderr);
#endif

#define c(x)	\
	with (fn_bid_t y = fn_##x(q, p + 1U - q)) { \
		if (y.state < 0) { \
			break; \
		} \
		dbgpr(x); \
		r = (anno_t){y, {q - s, p + 1U - s}}; \
	}
#define r(x)	r = (anno_t){S(x), {q - s, p + 1U - s}};
	%% write init;
	%% write exec;
	*pp = p;
	return r;
}
