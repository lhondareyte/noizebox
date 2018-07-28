#
#  $Id$
# 
MAKE	= gmake
MODULES = src rsc

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

init-modules:
	@echo $(CC)
	@printf "Updating modules ..."
	@git submodule init
	@git submodule update
	@echo "done."

modules:
	@mkdir -p fluidsynth/build
	@cd fluidsynth/build && cmake -DCMAKE_C_COMPILER=$(CC)  \
                                     -Denable-ipv6=off .. \
                                     -Denable-readline=off .. \
                                     -Denable-dbus=off .. \
                                     -Denable-libsndfile=off .. \
                                     -Denable-aufile=off .. \
                                     && $(MAKE)

install: src/noizebox
	@rm -f rsc/soundfont.conf
	@rsc/mksf2db.sh --empty
	@utils/install.sh Noizebox ./fluidsynth/build/src/libfluidsynth*so.*

package: install
	@find Noizebox -type f > port/pkg-plist
	@cd port && make package

clean-port:
	@cd port && make clean
