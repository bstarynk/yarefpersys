#!/usr/bin/gmake -f
### file yarefpersys/GNUmakefile
### See https://github.com/bstarynk/yarefpersys
CC ?= gcc
RM ?= rm -vf
YRPS_SHORTGIT := $(shell ./yrps-shortgit.sh)
CFLAGS ?= -Wall -Wextra -O -g
CFLAGS += -DYRPS_ID=\"$(YRPS_SHORTGIT)\" -DYRPS_SRCDIR=\"$(realpath .)\"
.PHONY: all clean modules indent

SOURCES= $(wildcard [a-z]*.c)
OBJECTS= $(patsubst %.c, %.o, $(SOURCES))
# by convention _[0-9A-Za-z]*.c files are generated
MODULESOURCES= $(shell /bin/ls _[0-9A-Za-z]*.c)

all: modules yarefpersys

yarefpersys: $(OBJECTS)
	$(CC) -UYRPS_LINK $(OBJECTS) -o $@ -lunistring -lcurl -ldl -lreadline

_%.so: _%.c yrps.h | GNUmakefile
	$(CC) -UYRPS_MODULE -DYRPS_THIS_MODULE=\"$(basename $<)\" $(CFLAGS) -fPIC -shared -o $@ $<


%.o: %.c yrps.h | GNUmakefile
	$(CC) -c $(CFLAGS) -DYRPS_THIS_BASE=\"$(basename $<)\"  -o $@ $<

modules: $(patsubst _%.c, _%.so, $(MODULESOURCES))

clean:
	$(RM) *.o *.so yarefpersys *~

indent:
	for f in [a-z]*.[ch]; do indent $$f ; done
