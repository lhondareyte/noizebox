#
#  $Id$
# 
MAKE=gmake
MODULES = fluidsynth src rsc sf2 utils nanobsd
all:
	for dir in $(MODULES); do \
		(cd $$dir; ${MAKE} ); \
	done
clean:
	for dir in $(MODULES); do \
		(cd $$dir; ${MAKE} clean); \
	done
flash:
	dd if=
