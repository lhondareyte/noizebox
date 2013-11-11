#
#  $Id$
# 

CFLAGS=-O2 -Wall -I /usr/include -I /usr/local/include -D __NOIZEBOX_DEBUG__
#CFLAGS= -O -Wall -I /usr/include -I /usr/local/include -D __FLUIDSYNTH_MIDI_DRIVER__  -D __NOIZEBOX_DEBUG__

LDFLAGS= -lncurses -lfluidsynth -L/usr/local/lib -pthread -I .
#LDFLAGS= -lncurses -L/usr/local/lib

HEADERS       := $(wildcard *.h)
HEADERS       += $(wildcard ihm/*.h)
SOURCES       := $(wildcard *.c)
SOURCES       += $(wildcard ihm/*.c)
ASMSRCS       := $(wildcard *.s)
ASMSRCS       += $(wildcard ihm/*.s)
OBJECTS       := $(patsubst %.c,%.o,$(SOURCES))
OBJECTS       += $(patsubst %.s,%.o,$(ASMSRCS))


PRG=../noizebox

$(PRG): $(OBJECTS) Makefile
	@printf "Linking   $@:"
	@$(CC) $(LDFLAGS) $(CFLAGS) $(OBJECTS) -s -o $@
	@printf "\tdone.\n"
	@ls -l $(PRG)



install:
	@scripts/install.sh
mrproper: clean
	@rm -f $(PRG)

clean:
	@rm -f  $(OBJECTS) *~
	@chmod 644 $(SOURCES) $(HEADERS) $(ASMSRCS)
.c.o:
	@printf "Compiling $<:"
	@$(CC) $(CFLAGS) -Os -c $< -o $@
	@printf "\tdone.\n"
