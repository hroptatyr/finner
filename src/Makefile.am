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
EXTRA_libfinner_a_SOURCES =
libfinner_a_SOURCES += figi.c
libfinner_a_SOURCES += isin.c
EXTRA_libfinner_a_SOURCES += isin-cc.ccl
BUILT_SOURCES += isin-cc.c
libfinner_a_SOURCES += lei.c
libfinner_a_SOURCES += cusip.c
libfinner_a_SOURCES += sedol.c
libfinner_a_SOURCES += iso4217.c iso4217.h
libfinner_a_SOURCES += fxpair.c
EXTRA_libfinner_a_SOURCES += fxpair-cc.ccl
BUILT_SOURCES += fxpair-cc.c
libfinner_a_SOURCES += num.c
libfinner_a_SOURCES += unit-1.c
libfinner_a_SOURCES += wkn.c
libfinner_a_SOURCES += ccy.c
libfinner_a_SOURCES += cfi.c
libfinner_a_SOURCES += gics.c
libfinner_a_SOURCES += amt.c
libfinner_a_SOURCES += date.c
libfinner_a_SOURCES += time.c
libfinner_a_SOURCES += ccysym.c

bin_PROGRAMS += finner
finner_SOURCES = finner.c finner.yuck
finner_SOURCES += nifty.h
finner_LDADD = libfinner.a
BUILT_SOURCES += finner.yucc
BUILT_SOURCES += finner.rlc


## ccl->c
SUFFIXES += .ccl
.ccl.c:
	$(AM_V_GEN) $(top_builddir)/build-aux/cccc < $< > $@ || $(RM) -- $@

## version rules
version.c: $(srcdir)/version.c.in $(top_builddir)/.version
	$(AM_V_GEN) PATH="$(top_builddir)/build-aux:$${PATH}" \
		yuck$(EXEEXT) scmver --ignore-noscm --force -o $@ \
			--use-reference --reference $(top_builddir)/.version \
			$(srcdir)/version.c.in

## yuck rule
SUFFIXES += .yuck
SUFFIXES += .yucc
.yuck.yucc:
	$(AM_V_GEN) PATH=$(top_builddir)/build-aux:"$${PATH}" \
		yuck$(EXEEXT) gen -o $@ $<

SUFFIXES += .rl
SUFFIXES += .rlc
.rl.rlc:
	$(AM_V_GEN) PATH=$(top_builddir)/build-aux:"$${PATH}" \
		ragel -F1 -C -o $@ $<

finner.rlc: unit-1.c ccy.c ccysym.c amt.c time.c date.c num.c

## Create preprocessor output (debugging purposes only)
.c.i:
	$(COMPILE) -E -o $@ $<

## Create assembler output (debugging purposes only)
.c.s:
	$(COMPILE) -S -c $(AM_CFLAGS) $<
