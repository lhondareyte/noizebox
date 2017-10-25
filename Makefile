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
clean:
	for dir in $(MODULES); do \
		(cd $$dir; $(MAKE) clean ; cd ..); \
	done
	@rm -rf noizebox.pkg noizebox.md5 Noizebox/*
	@rm -rf fluidsynth
	@cd port && make clean

fluidsynth:
	@printf "Fetching fluidsynth ..."
	@git clone https://github.com/FluidSynth/fluidsynth.git
	@echo "done."
	@mkdir fluidsynth/build
	cd fluidsynth/build && cmake -DCMAKE_C_COMPILER=$(GCC)  \
                                     -Denable-ipv6=off .. \
                                     -Denable-readline=off .. \
                                     -Denable-dbus=off .. \
                                     -Denable-libsndfile=off .. \
                                     -Denable-aufile=off .. \
                                     && gmake
package:
	@utils/install.sh
	@touch Noizebox/Resources/soundfont.conf
	@find Noizebox -type f > port/pkg-plist
	@cd port && make package

clean-port:
	@cd port && make clean
