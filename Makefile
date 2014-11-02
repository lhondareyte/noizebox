#
#  $Id$
# 
MAKE=gmake
MODULES = fluidsynth src rsc sf2
package: all noizebox.pkg
all:
	for dir in $(MODULES); do \
		(cd $$dir; ${MAKE} ); \
	done
noizebox.pkg:
	@./install.sh
clean:
	for dir in $(MODULES); do \
		(cd $$dir; ${MAKE} clean); \
	done
