/* -*- c -*- */
#include <string.h>
#include <stdio.h>
#include "finner.h"
#include "nifty.h"

#ifdef __INTEL_COMPILER
# pragma warning (disable:2415)
#endif  /* __INTEL_COMPILER */

#define p(x)	
#define q(x)	\
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
	include finner "unit-1.rl";

	int = ("+" | "-")? /[1-9]/ digit* | "0" ;

	float = ("+" | "-")? "0" ("." | ",") digit+
		| ("+" | "-")? /[1-9]/ digit* ("." | ",") digit+ ;

	num = int | float ;

	amt = ccy " "? (num @{c(num)}) | num " "? ccy ;

	finner =
		ccy @{c(ccy)} |
		upper{2} upnum{9} digit @{c(isin)} |
		"BBG" (consonant | digit){8} digit @{c(figi)} |
		upnum{6} @{c(wkn)} |
		num @{c(num)} |
		unit_1 @{m(unit_1)} |
		amt @{c(amt)} |
		(alnum | "*" | "@" | "#"){8} digit @{c(cusip)} |
		(consonant | digit){6} digit @{c(sedol)} |
		upnum{18} digit{2} @{p(lei)} |
		empty ;

	main := (any* @{q = p + 1U;}) :> ( finner ) ;

	write data;
}%%

static annu_t
terms1(const char **pp, const char *const s, const char *const pe)
{
	const char *p = *pp;
	const char *q = p;
	fn_state_t S = 0;
	annu_t r = {};
	int cs;

#if 0
	fputs("TERMS1(", stderr);
	fwrite(p, 1, pe - p, stderr);
	fputs(")\n", stderr);
#endif

#define state(x)	(S = (intptr_t)(x));
#define c(x)	\
	with (fn_bnu_t y = fn_##x(q, p + 1U - q)) { \
		if (!y.print) { \
			break; \
		} \
		p(x); \
		r = (annu_t){{q - s, p + 1U - s}, y}; \
	}
#define m(x)	\
	with (fn_bnu_t y = {fn_##x, S}) { \
		p(x); \
		r = (annu_t){{q - s, p + 1U - s}, y}; \
	}
	%% write init;
	%% write exec;
	*pp = p;
	return r;
}
