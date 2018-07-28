#
#  $Id$
# 
PRG	= noizebox
MAKE	= gmake
CC      = gcc
CFLAGS	= -O2 -Wall -I /usr/include -I /usr/local/include -I ../fluidsynth/include
CFLAGS	+= -I ../fuildsynth/include -I ../fluidsynth/build/include 
CFLAGS	+= -D __WITH_SQLITE__ -D SQLITE_TEMP_STORE=3
#CFLAGS	+= -D __SPDIF_ADAPTER__
CFLAGS	+= -D __WT11_COMPAT__
CFLAGS  += -D __NOIZEBOX_DEBUG__

LDFLAGS	= -lsqlite3 -lncurses -lfluidsynth -L/usr/local/lib -pthread -I .
LDFLAGS	+= -L ../fluidsynth/build/src

HEADERS	= $(shell ls *.h)
SOURCES	= $(shell ls *.c)
OBJECTS = $(subst .c,.o,$(SOURCES))

OSNAME	= $(shell uname)
ARCH	= $(shell uname -m)
GIT_TAG	= $(shell git describe --tags)
GIT_YEAR= $(shell git log -1 --format=%cd | cut -d' ' -f5)
CFLAGS	+= -DVERSION=\"$(GIT_TAG)\"
CFLAGS	+= -DYEAR=\"$(GIT_YEAR)\"

ifeq ($( GIT_TAG),)
GIT_TAG = devel
endif


$(PRG): $(OBJECTS)
	@printf "Linking   $@:"
	@$(CC) $(LDFLAGS) $(CFLAGS) $(OBJECTS) -o $@
	@printf "\tdone.\n"

clean:
	@rm -f  $(OBJECTS) $(PRG) *~

.c.o:
	@printf "Compiling $<:"
	@$(CC) $(CFLAGS) -Os -c $< -o $@
	@printf "\tdone.\n"
install:
	mkdir -p ../Noizebox/Contents/$(OSNAME)/$(ARCH)/
	install -m 755 $(PRG) ../Noizebox/Contents/$(OSNAME)/$(ARCH)/
