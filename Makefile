#
#  $Id$
# 
APP       = noizebox
MODULES   = src rsc
OSNAME   != uname -s
ARCH     != uname -m
SYS       = "$(OSNAME)/$(ARCH)"
APPDIR    = ./Noizebox
CONTENT   = $(APPDIR)/Contents/$(SYS)
FRAMEWORK = $(APPDIR)/Frameworks/$(SYS)
RESOURCE  = $(APPDIR)/Resources

all: init-modules modules

init-modules:
	@echo $(CC)
	@printf "Updating modules ..."
	@git submodule init
	@git submodule update
	@echo "done."

modules:
	@mkdir -p fluidsynth/build
	@cd fluidsynth/build && \
		cmake -DCMAKE_C_COMPILER=$(CC)  \
                      -Dosal=cpp11 \
                      -Denable-alsa=off \
                      -Denable-aufile=off \
                      -Denable-dbus=off \
                      -Denable-ipv6=off \
                      -Denable-jack=off \
                      -Denable-ladspa=off \
                      -Denable-lash=off \
                      -Denable-libinstpatch=off \
                      -Denable-libsndfile=off \
                      -Denable-oboe=off \
                      -Denable-pulseaudio=off \
                      -Denable-readline=off \
                      -Denable-systemd=off \
                      .. && make
	for dir in $(MODULES); do \
		(cd $$dir; $(MAKE) ; cd ..); \
	done

bintree: modules
	@rm -rf $(CONTENT) $(FRAMEWORK) $(RESOURCE)
	@mkdir -p $(CONTENT) $(FRAMEWORK) $(RESOURCE)/etc/devd
	@install -m 644 rsc/$(APP).conf $(RESOURCE)/etc/
	@install -m 644 rsc/soundfont.conf $(RESOURCE)
	@install -m 644 rsc/noizebox-usb.conf $(RESOURCE)/etc/devd
	@install -m 755 rsc/mksf2db.sh $(RESOURCE)
	@install -m 755 rsc/$(APP).sh $(APPDIR)/$(APP)
	@install -m 755 src/$(APP) $(CONTENT)
	@utils/install_lib.sh $(FRAMEWORK) src/$(APP)
	@utils/install_lib.sh -l $(FRAMEWORK) ./fluidsynth/build/src/libfluidsynth*so.*

clean:
	@rm -rf $(APPDIR) fluidsynth
	for dir in $(MODULES); do \
		(cd $$dir; $(MAKE) clean ; cd ..); \
	done

tarball: clean
	@cd .. && tar cvf noizebox.tgz \
                    noizebox/src \
		    noizebox/rsc \
		    noizebox/Makefile \
		    noizebox/utils \
		    noizebox/Run.sh 
