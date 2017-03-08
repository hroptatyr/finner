### Makefile.am
include $(top_builddir)/version.mk

AM_CFLAGS = $(EXTRA_CFLAGS)
AM_CPPFLAGS = -D_POSIX_C_SOURCE=201001L -D_XOPEN_SOURCE=700 -D_BSD_SOURCE
AM_CPPFLAGS += -D_HAVE_VERSION_H
AM_LDFLAGS = $(XCCLDFLAGS)

bin_PROGRAMS =
noinst_PROGRAMS =
lib_LTLIBRARIES =
noinst_LTLIBRARIES =
pkglib_LTLIBRARIES =
noinst_HEADERS =
BUILT_SOURCES =
EXTRA_DIST = $(BUILT_SOURCES)
CLEANFILES = 
SUFFIXES =

CLEANFILES += version.c
EXTRA_DIST += version.c.in

noinst_LTLIBRARIES += libbidder.la
libbidder_la_SOURCES = finner.h
libbidder_la_SOURCES += bidder.c bidder.h
libbidder_la_SOURCES += collector.c collector.h
EXTRA_libbidder_la_SOURCES =
libbidder_la_SOURCES += figi.c figi.h
libbidder_la_SOURCES += isin.c isin.h
EXTRA_libbidder_la_SOURCES += isin-cc.ccl
BUILT_SOURCES += isin-cc.c
libbidder_la_SOURCES += lei.c lei.h
libbidder_la_SOURCES += cusip.c cusip.h
libbidder_la_SOURCES += sedol.c sedol.h
libbidder_la_SOURCES += ccy.c ccy.h
EXTRA_libbidder_la_SOURCES += ccy-cc.ccl
BUILT_SOURCES += ccy-cc.c
libbidder_la_SOURCES += fxpair.c fxpair.h
EXTRA_libbidder_la_SOURCES += fxpair-cc.ccl
BUILT_SOURCES += fxpair-cc.c
libbidder_la_SOURCES += num.c num.h
libbidder_la_SOURCES += wkn.c wkn.h
libbidder_la_SOURCES += date.c date.h
libbidder_la_SOURCES += ccysym.c ccysym.h
libbidder_la_SOURCES += amt.c amt.h

bin_PROGRAMS += finner
finner_SOURCES = finner.c finner.yuck
finner_SOURCES += nifty.h
finner_LDADD = libbidder.la
BUILT_SOURCES += finner.yucc


## ccl->c
SUFFIXES += .ccl
.ccl.c:
	$(AM_V_GEN) $(top_builddir)/build-aux/cccc < $< > $@ || $(RM) -- $@

## version rules
version.c: version.c.in $(top_builddir)/.version
	$(AM_V_GEN) PATH="$(top_builddir)/build-aux:$${PATH}" \
		yuck$(EXEEXT) scmver --ignore-noscm --force -o $@ \
			--use-reference --reference $(top_builddir)/.version $<

## yuck rule
SUFFIXES += .yuck
SUFFIXES += .yucc
.yuck.yucc:
	$(AM_V_GEN) PATH=$(top_builddir)/build-aux:"$${PATH}" \
		yuck$(EXEEXT) gen -o $@ $<

## Create preprocessor output (debugging purposes only)
.c.i:
	$(COMPILE) -E -o $@ $<

## Create assembler output (debugging purposes only)
.c.s:
	$(COMPILE) -S -c $(AM_CFLAGS) $<
