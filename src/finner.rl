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

	include finner "ccy.rl";
	include finner "ccysym.rl";
	include finner "unit-1.rl";

	int = ("+" | "-")? /[1-9]/ digit* ;

	float = ("+" | "-")? "0" ("." | ",") digit+
		| ("+" | "-")? /[1-9]/ digit* ("." | ",") digit+ ;

	num = int | float ;

	amt = ccy " "? (num @{c(num)}) | num " "? ccy ;

	date =
		digit{4} ("/" | "-")? digit{2} ("/" | "-")? digit{2} @{c(date_y1)} |
		digit{2} ("." | "/")? digit{2} ("." | "/")? digit{4} @{c(date_yl)} ;

	finner =
		num @{c(num)} |
		date |
		ccy @{r("ccy")} |
		ccysym |
		ccy ("." | ":" | "/")? ccy @{c(fxpair)} |
		upper{2} upnum{9} digit @{c(isin)} |
		"BBG" (consonant | digit){8} digit @{c(figi)} |
		upnum{6} @{c(wkn)} |
		unit_1 |
		amt @{r("amt")} |
		(alnum | "*" | "@" | "#"){8} digit @{c(cusip)} |
		(consonant | digit){6} digit @{c(sedol)} |
		upnum{18} digit{2} @{c(lei)} |
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
