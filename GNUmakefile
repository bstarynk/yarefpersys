#!/usr/bin/gmake -f
### file yarefpersys/GNUmakefile
### See https://github.com/bstarynk/yarefpersys
CC = gcc
RM ?= rm -vf
YRPS_SHORTGIT := $(shell ./yrps-shortgit.sh)
YRPS_WARNINGS ?= -Wall -Wextra
YRPS_OPTIMS ?= -O -g
YRPS_DEFINES=  -DYRPS_ID=\"$(YRPS_SHORTGIT)\" -DYRPS_SRCDIR=\"$(realpath .)\"
CFLAGS ?= $(YRPS_WARNINGS) $(YRPS_OPTIMS)
.PHONY: all clean modules indent

SOURCES= $(wildcard [a-z]*.c)
OBJECTS= $(patsubst %.c, obj/%.o, $(SOURCES))
# by convention _[0-9A-Za-z]*.c files are generated
MODULESOURCES= $(shell /bin/ls _[0-9A-Za-z]*.c)

all: modules yarefpersys

yarefpersys: $(OBJECTS) obj/__build.o | GNUmakefile
	$(CC) -UYRPS_LINK $(OBJECTS) obj/__build.o -o $@ -lunistring -lcurl -ldl -lreadline

__build.c: $(SOURCES) $(shell which $(CC)) | GNUmakefile
	printf "// yarefpersys/__build.c is GENERATED\n" > $@
	printf "const char yrps_build_date[]= __DATE__;\n" >> $@
	printf "const char yrps_build_dir[]= \"%s\";\n" $(realpath .) >> $@
	printf "const char yrps_build_git[]= \"%s\";\n" $(YRPS_SHORTGIT) >> $@
	printf "const char yrps_build_compiler[]= \"%s\";\n" $(shell which $(CC)) >> $@
	printf "const char yrps_build_compiler_version[]= \"%s\";\n" '$(shell $(CC) --version | head -1)' >> $@

obj/_%.so: _%.c yrps.h | GNUmakefile
	$(CC) -UYRPS_MODULE -DYRPS_THIS_MODULE=\"$(basename $<)\" $(YRPS_DEFINES) -fPIC -shared -o $@ $<


obj/%.o: %.c yrps.h | GNUmakefile
	$(CC) -c $(CFLAGS) -DYRPS_THIS_BASE=\"$(basename $<)\" $(YRPS_DEFINES)  -o $@ $<

modules: $(patsubst _%.c, obj/_%.so, $(MODULESOURCES))

clean:
	$(RM) obj/*.o obj/*.so yarefpersys *~ __build.c

indent:
	for f in [a-z]*.[ch]; do indent --no-tabs  $$f ; done
