#
#  $Id$
# 
MAKE=gmake
GCC=/usr/local/bin/gcc
MODULES = src rsc
package: all
all: fluidsynth
	for dir in $(MODULES); do \
		(cd $$dir; $(MAKE) ; cd ..); \
	done
package:
	@utils/install.sh
clean:
	for dir in $(MODULES); do \
		(cd $$dir; $(MAKE) clean ; cd ..); \
	done
	rm -rf noizebox.pkg noizebox.md5 Noizebox/*

fluidsynth:
	git clone https://github.com/FluidSynth/fluidsynth.git
	mkdir fluidsynth/build
	cd fluidsynth/build && cmake -DCMAKE_C_COMPILER=$(GCC)  \
                                     -Denable-ipv6=off .. \
                                     -Denable-readline=off .. \
                                     -Denable-dbus=off .. \
                                     && gmake
