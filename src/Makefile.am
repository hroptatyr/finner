### Makefile.am
include $(top_builddir)/version.mk

AM_CFLAGS = $(EXTRA_CFLAGS)
AM_CPPFLAGS = -D_POSIX_C_SOURCE=201001L -D_XOPEN_SOURCE=700 -D_BSD_SOURCE
AM_CPPFLAGS += -D_HAVE_VERSION_H
AM_LDFLAGS = $(XCCLDFLAGS)

bin_PROGRAMS =
noinst_PROGRAMS =
lib_LIBRARIES =
noinst_LIBRARIES =
noinst_HEADERS =
BUILT_SOURCES =
EXTRA_DIST = $(BUILT_SOURCES)
CLEANFILES = 
SUFFIXES =

CLEANFILES += version.c
EXTRA_DIST += version.c.in

noinst_LIBRARIES += libfinner.a
libfinner_a_SOURCES = finner.h
libfinner_a_SOURCES += bidder.c bidder.h
libfinner_a_SOURCES += collector.c collector.h
EXTRA_libfinner_a_SOURCES =
libfinner_a_SOURCES += figi.c figi.h
libfinner_a_SOURCES += isin.c isin.h
EXTRA_libfinner_a_SOURCES += isin-cc.ccl
BUILT_SOURCES += isin-cc.c
libfinner_a_SOURCES += lei.c lei.h
libfinner_a_SOURCES += cusip.c cusip.h
libfinner_a_SOURCES += sedol.c sedol.h
libfinner_a_SOURCES += ccy.c ccy.h
EXTRA_libfinner_a_SOURCES += ccy-cc.ccl
BUILT_SOURCES += ccy-cc.c
libfinner_a_SOURCES += iso4217.c iso4217.h
libfinner_a_SOURCES += fxpair.c fxpair.h
EXTRA_libfinner_a_SOURCES += fxpair-cc.ccl
BUILT_SOURCES += fxpair-cc.c
libfinner_a_SOURCES += num.c num.h
libfinner_a_SOURCES += unit-1.c unit-1.h
libfinner_a_SOURCES += wkn.c wkn.h
libfinner_a_SOURCES += date.c date.h
libfinner_a_SOURCES += datex.c datex.h
BUILT_SOURCES += months-cc.c
libfinner_a_SOURCES += timex.c timex.h
libfinner_a_SOURCES += ccysym.c ccysym.h
libfinner_a_SOURCES += amt.c amt.h

bin_PROGRAMS += finner
finner_SOURCES = finner.c finner.yuck
finner_SOURCES += nifty.h
finner_LDADD = libfinner.a
BUILT_SOURCES += finner.yucc


## ccl->c
SUFFIXES += .ccl
.ccl.c:
	$(AM_V_GEN) $(top_builddir)/build-aux/cccc < $< > $@ || $(RM) -- $@

## phl->c
SUFFIXES += .phl
.phl.c:
	$(AM_V_GEN) $(top_builddir)/build-aux/phash < $< > $@ || $(RM) -- $@

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
