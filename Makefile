#
#  $Id$
# 
MAKE=gmake
MODULES = fluidsynth src rsc
package: all noizebox.pkg
all:
	for dir in $(MODULES); do \
		(cd $$dir; ${MAKE} ); \
	done
package:
	@utils/install.sh
clean:
	for dir in $(MODULES); do \
		(cd $$dir; ${MAKE} clean); \
	done
