### file yarefpersys/GNUmakefile
### See https://github.com/bstarynk/yarefpersys
CC ?= gcc
RM ?= rm -vf
YRPS_SHORTGIT := $(shell ./yrps-shortgit.sh)
CFLAGS ?= -Wall -Wextra -O -g
CFLAGS += -DYRPS_ID=\"$(YRPS_SHORTGIT)\" -DYRPS_SRCDIR=\"$(realpath .)\"
.PHONY: all clean modules

OBJECTS= main_yrps.o parse_yrps.o
MODULESOURCES= $(shell /bin/ls _[a-z]*.c)

all: modules yarefpersys

yarefpersys: $(OBJECTS)
	$(CC) -UYRPS_LINK $(OBJECTS) -o $@ -lunistring -lcurl -ldl -lreadline

_%.so: _%.c yrps.h | GNUmakefile
	$(CC) -UYRPS_MODULE -DYRPS_THIS_MODULE=\"$(basename $<)\" $(CFLAGS) -fPIC -shared -o $@ $<


%.o: %.c yrps.h | GNUmakefile
	$(CC) -c $(CFLAGS)   -o $@ $<

modules: $(patsubst _%.c, _%.so, $(MODULESOURCES))

clean:
	$(RM) *.o *.so yarefpersys *~
