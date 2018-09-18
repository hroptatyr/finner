# Help the Developers and yourself. Just use the C locale and settings
# for the compilation. They can still be overriden by make LANG=<whatever>
# but that is general a not very good idea
include $(top_builddir)/version.mk

LANG = C
LC_ALL = C

AM_CFLAGS = $(EXTRA_CFLAGS)
AM_CPPFLAGS = -D_POSIX_C_SOURCE=200112L -D_XOPEN_SOURCE=600 -D_BSD_SOURCE
AM_CPPFLAGS += -DTEST

EXTRA_DIST = $(BUILT_SOURCES) $(TESTS)
TESTS =
TEST_EXTENSIONS =
BUILT_SOURCES =
dt_tests =
bin_tests =
batch_tests =
check_PROGRAMS =
CLEANFILES = $(check_PROGRAMS)

## our little helper
include clitoris.am
AM_CLIT_LOG_FLAGS = --builddir $(top_builddir)/src --verbose --keep-going

TESTS += wkn_test_01.clit
TESTS += wkn_test_02.clit
EXTRA_DIST += wkn_test_01.txt

TESTS += cftc_test_01.clit
TESTS += cftc_test_02.clit
EXTRA_DIST += cftc_test_01.txt

TESTS += nl_01.clit
TESTS += nl_02.clit

TESTS += amt_01.clit
EXTRA_DIST += amt_01.txt

TESTS += date_01.clit
EXTRA_DIST += date_01.txt

TESTS += ccysym_01.clit
EXTRA_DIST += ccysym_01.txt

TESTS += cfi_test_01.clit
EXTRA_DIST += cfi_test_01.txt

## Makefile.am ends here
