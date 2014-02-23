#
# $Id$
#
DEVICE     = /dev/ada1
MAKE       = /usr/local/bin/gmake
NANOSCRIPT = /usr/src/tools/tools/nanobsd/nanobsd.sh
MACHINE    = $(shell (uname -m))
KERNEL     = kernel.$(MACHINE)
NANOCFG    = nanobsd.conf
DISKIMAGE  = /usr/obj/nanobsd.noizebox/_.disk.full 
NZ_ROOTSRC = $(shell (dirname $(pwd)))

all: 
	@/bin/sh $(NANOSCRIPT) -b -c $(NANOCFG)

world:
	@/bin/sh $(NANOSCRIPT) -k -i -c $(NANOCFG)

kernel:
	@cp ${KERNEL} /usr/src/sys/${MACHINE}/conf/NOIZEBOX
	@/sbin/sh $(NANOSCRIPT) -w -i -c $(NANOCFG)

install: 
	@printf "Add loopback device ..."
	@$(eval MD = $(shell mdconfig -f $(DISKIMAGE)))
	@printf "y\ny\n" > /tmp/rep.nz
	@echo "done."
	@printf "Adding bootloader..."
	@fdisk -B  /dev/${MD} < /tmp/rep.nz > /dev/null 2>&1
	@rm -f /tmp/rep.nz
	@echo "done."
	@printf "Populating configuration slice..."
	@mount /dev/$(MD)s3 /mnt
	@cp ../Resources/noizebox.conf /mnt
	@cp ../Resources/noizebox.conf /mnt/noizebox.conf.safe
	@sync;sync;sync
	@umount /mnt
	@mdconfig -d -u $(MD)
	@echo "done."
	@printf "Writing image to disk..."
	@dd if=$(DISKIMAGE) of=$(DEVICE) bs=64k > /dev/null 2>&1
	@echo "done."
