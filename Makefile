#
#  $Id$
# 
MAKE	= gmake
MODULES = src rsc
APP     = noizebox
APPDIR  = Noizebox

all: modules
	for dir in $(MODULES); do \
		(cd $$dir; $(MAKE) ; cd ..); \
	done
clean:
	for dir in $(MODULES); do \
		(cd $$dir; $(MAKE) clean ; cd ..); \
	done
	@rm -rf $(APP).pkg $(APP).md5 $(APPDIR)/*
	@cd port && make clean

init-modules:
	@echo $(CC)
	@printf "Updating modules ..."
	@git submodule init
	@git submodule update
	@echo "done."

modules: init-modules
	@mkdir -p fluidsynth/build
	@cd fluidsynth/build && cmake -DCMAKE_C_COMPILER=$(CC)  \
                                     -Denable-ipv6=off .. \
                                     -Denable-readline=off .. \
                                     -Denable-dbus=off .. \
                                     -Denable-libsndfile=off .. \
                                     -Denable-jack=off .. \
                                     -Denable-aufile=off .. \
                                     && $(MAKE)

install: all
	@rm -f rsc/soundfont.conf
	@rsc/mksf2db.sh --empty
	@utils/install.sh Noizebox ./fluidsynth/build/src/libfluidsynth*so.*
	@install -m 644 rsc/$(APP).conf $(APPDIR)/Resources/etc/
	@install -m 644 rsc/soundfont.conf $(APPDIR)/Resources/
	@install -m 644 rsc/noizebox-usb.conf $(APPDIR)/Resources/etc/devd
	@install -m 755 rsc/$(APP).sh $(APPDIR)/$(APP)

package: install
	@find Noizebox \( -type l -o -type f \) > port/pkg-plist
	@cd port && make package

clean-port:
	@cd port && make clean && rm -f pkg-plist

distclean: clean clean-port

