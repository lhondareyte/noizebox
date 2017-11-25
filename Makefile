#
#  $Id$
# 
MAKE=gmake
GCC=/usr/local/bin/gcc
MODULES = src rsc
package: all
all: modules
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

modules:
	@printf "Updating modules ..."
	@git submodule init
	@git submodule update
	@echo "done."
	@mkdir -p fluidsynth/build
	@cd fluidsynth/build && cmake -DCMAKE_C_COMPILER=$(GCC)  \
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
