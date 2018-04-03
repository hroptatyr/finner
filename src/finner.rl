#include <string.h>
#include <stdio.h>
#include "finner.h"
#include "nifty.h"

#ifdef __INTEL_COMPILER
# pragma warning (disable:2415)
#endif  /* __INTEL_COMPILER */

#define p(x)	//fwrite(q, 1, p + 1U - q, stdout), puts(#x);

%%{
	machine finner;

	upnum = digit | upper;
	lonum = digit | lower;
	check = "_" | "?";
	vowel = "A" | "E" | "I" | "O" | "U";
	consonant = upper - vowel;
	sep = "\f" | "\n" | "\r\n" | "\t" | "\v" | " ";

	include finner "ccy.rl";
	include finner "unit-1.rl";

	int = ("+" | "-")? /[1-9]/ digit* | "0" ;

	float = ("+" | "-")? "0" ("." | ",") digit+
		| ("+" | "-")? /[1-9]/ digit* ("." | ",") digit+ ;

	num = int unit_1? | float unit_1? ;

	amt = ccy " "? (num @{c(num)}) | num " "? ccy ;

	finner =
		ccy @{c(ccy)} |
		upper{2} upnum{9} (digit | check) @{c(isin)} |
		"BBG" (consonant | digit){8} (digit | check) @{p(figi)} |
		upnum{6} @{c(wkn)} |
		num @{c(num)} |
		amt @{c(amt)} |
		(alnum | "*" | "@" | "#"){8} (digit | check) @{p(cusip)} |
		(consonant | digit){6} (digit | check) @{p(sedol)} |
		upnum{18} (digit{2} | check{2}) @{p(lei)} |
		empty ;

	main := (any* @{q = p + 1U;}) :> ( finner ) ;

	write data;
}%%

static annu_t
terms1(const char **pp, const char *const s, const char *const pe)
{
	const char *p = *pp;
	const char *q = p;
	annu_t r = {};
	int cs;

#if 1
	fputs("TERMS1(", stdout);
	fwrite(p, 1, pe - p, stdout);
	fputs(")\n", stdout);
#endif

#define c(x)	\
	with (fn_bnu_t y = fn_##x(q, p + 1U - q)) { \
		if (!y.print) { \
			break; \
		} \
		fwrite(q, 1, p + 1U - q, stdout); \
		fputc('\t', stdout); \
		puts(#x); \
		r = (annu_t){{q - s, p + 1U - s}, y}; \
	}
	%% write init;
	%% write exec;
	*pp = p;
	return r;
}
