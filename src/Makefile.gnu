#
#  $Id$
# 

CFLAGS	= -O2 -Wall -I /usr/include -I /usr/local/include -I ../fluidsynth/include
CFLAGS	+= -I ../fluidsynth/include -L ../fluidsynth/src/.libs
#CFLAGS	+= -D __WITH_SQLITE__ 
CFLAGS	+= -D __WITH_SQLITE__ -D SQLITE_TEMP_STORE=3

#CFLAGS=-O2 -Wall -I /usr/include -I /usr/local/include -D __NOIZEBOX_DEBUG__
#CFLAGS= -O -Wall -I /usr/include -I /usr/local/include -D __FLUIDSYNTH_MIDI_DRIVER__  -D __NOIZEBOX_DEBUG__

LDFLAGS= -lsqlite3 -lncurses -lfluidsynth -L/usr/local/lib -pthread -I .

HEADERS       := $(wildcard *.h)
SOURCES       := $(wildcard *.c)
ASMSRCS       := $(wildcard *.s)
OBJECTS       := $(patsubst %.c,%.o,$(SOURCES))
OBJECTS       += $(patsubst %.s,%.o,$(ASMSRCS))

GIT_VERSION   := $(shell git describe --tags)
CFLAGS	      += -DVERSION=\"$(GIT_VERSION)\"


PRG=noizebox

$(PRG): $(OBJECTS) Makefile
	@printf "Linking   $@:"
	@$(CC) $(LDFLAGS) $(CFLAGS) $(OBJECTS) -o $@
	@printf "\tdone.\n"



mrproper: clean
	@rm -f $(PRG)

clean:
	@rm -f  $(OBJECTS) *~
	@chmod 644 $(SOURCES) $(HEADERS) $(ASMSRCS)
.c.o:
	@printf "Compiling $<:"
	@$(CC) $(CFLAGS) -Os -c $< -o $@
	@printf "\tdone.\n"
install:
	install -m 755 noizebox ../nanobsd/Noizebox/Contents/FreeBSD/amd64/
