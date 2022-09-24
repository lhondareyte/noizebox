#
#  $Id$
# 

PRG      = noizebox
OSNAME  != uname
ARCH    != uname -m
SOURCES  = bank.c conf.c hinv.c ihm.c midi.c mixer.c noizebox.c synth.c 
OBJECTS  = bank.o conf.o hinv.o ihm.o midi.o mixer.o noizebox.o synth.o 

CFLAGS	 = -O2  -Wall -Wextra -Wpedantic
CFLAGS	 += -I /usr/include -I /usr/local/include -I .
CFLAGS	 += -I ../fluidsynth/include -I ../fluidsynth/build/include 
CFLAGS	 += -D SQLITE_TEMP_STORE=3
CFLAGS	 += -D __WT11_COMPAT__
CFLAGS  += -D __LEGACY_MIDI_PARSER__
#CFLAGS   += -D __WITH_JACK__
#CFLAGS  += -D __NOIZEBOX_DEBUG__
CFLAGS	 += -DVERSION=\"$(GIT_VERSION)\"
CFLAGS	 += -DYEAR=\"$(GIT_YEAR)\"

LDFLAGS  = -s -lsqlite3 -lncurses -lfluidsynth -pthread
LDFLAGS  += -L ../fluidsynth/build/src
LDFLAGS  += -L /usr/local/lib
LDFLAGS  += -L /usr/lib/$(ARCH)-linux-gnu

GIT_VER  = $(shell git describe --tags)
GIT_YEAR = $(shell git log -1 --format=%cd | cut -d' ' -f5)

$(PRG): $(OBJECTS)
	@printf "Linking   $@:"
	@$(CC) $(OBJECTS) -o $@ $(LDFLAGS)
	@printf "\tdone.\n"

mrproper: clean
	@rm -f $(PRG)

clean:
	@rm -f  $(OBJECTS) *~ $(PRG)
.c.o:
	@printf "Compiling $<:"
	@$(CC) $(CFLAGS) -Os -c $< -o $@
	@printf "\tdone.\n"
install:
	mkdir -p ../Noizebox/Contents/$(OSNAME)/$(ARCH)/
	install -m 755 $(PRG) ../Noizebox/Contents/$(OSNAME)/$(ARCH)/
