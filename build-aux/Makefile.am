LANG = C
LC_ALL = C

AM_CFLAGS = $(EXTRA_CFLAGS)

noinst_PROGRAMS =
BUILT_SOURCES =
EXTRA_DIST =
DISTCLEANFILES =

if !HAVE_YUCK
include yuck.am
endif  !HAVE_YUCK

noinst_PROGRAMS += cccc
cccc_CPPFLAGS = -D_POSIX_C_SOURCE=200809L -D_XOPEN_SOURCE=700 -D_BSD_SOURCE
cccc_CPPFLAGS += -D_ALL_SOURCE
cccc_CPPFLAGS += -D_DARWIN_C_SOURCE
cccc_CPPFLAGS += -D_NETBSD_SOURCE

## Makefile.am ends here
