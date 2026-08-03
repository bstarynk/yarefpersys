### file yarefpersys/GNUmakefile
### See https://github.com/bstarynk/yarefpersys
CC ?= gcc
RM ?= rm -vf
YRPS_SHORTGIT := $(shell ./yrps-shortgit.sh)
CFLAGS ?= -Wall -Wextra -O -g
CFLAGS += -DYRPS_ID=\"$(YRPS_SHORTGIT)\"
.PHONY: all clean

OBJECTS= main_yrps.o

all: yarefpersys

yarefpersys: $(OBJECTS)
	$(CC) -UYRPS_LINK $(OBJECTS) -o $@ -ldl -lreadline

%.o: %.c yrps.h
	$(CC) -c $(CFLAGS)   -o $@ $<

clean:
	$(RM) *.o yarefpersys *~
