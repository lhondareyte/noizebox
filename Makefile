#
#  $Id$
# 
MAKE=gmake
MODULES = fluidsynth noizebox Resources SF2 utils nanobsd
all:
	for dir in $(MODULES); do \
		(cd $$dir; ${MAKE} ); \
	done
clean:
	for dir in $(MODULES); do \
		(cd $$dir; ${MAKE} clean); \
	done
