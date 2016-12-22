#
#  $Id$
# 
MAKE=gmake
MODULES = fluidsynth src rsc
package: all noizebox.pkg
all:
	for dir in $(MODULES); do \
		(cd $$dir; $(MAKE) ; cd ..); \
	done
package:
	@utils/install.sh
clean:
	for dir in $(MODULES); do \
		(cd $$dir; $(MAKE) clean); \
	done
