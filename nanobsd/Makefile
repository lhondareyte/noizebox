#
# $Id$
#
DEVICE     = /dev/ada1
MAKE       = /usr/local/bin/gmake
NANOSCRIPT = /usr/src/tools/tools/nanobsd/nanobsd.sh
MACHINE    = $(shell uname -m)
KERNEL     = kernel.$(MACHINE)
NANOCFG    = nanobsd.conf
DISKIMAGE  = /usr/obj/nanobsd.noizebox/_.disk.full 
NZ_ROOTSRC = $(shell dirname $(pwd))

all: 
	@/bin/sh $(NANOSCRIPT) -b -c $(NANOCFG)

world:
	@/bin/sh $(NANOSCRIPT) -k -i -c $(NANOCFG)

kernel:
	@echo "Building kernel for $(MACHINE)"
	@cp $(KERNEL) /usr/src/sys/$(MACHINE)/conf/NOIZEBOX
	@/bin/sh $(NANOSCRIPT) -w -i -c $(NANOCFG)

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
	@mkdir -p /mnt/rc.d
	@install -m 755 ../rsc/noizebox.rc.d /mnt/rc.d/noizebox
	@cp ../rsc/rc.conf /mnt
	@cp ../rsc/sysctl.conf /mnt
	@cp ../rsc/noizebox.conf /mnt
	@cp ../rsc/noizebox.conf /mnt/noizebox.conf.safe
	@cp ../rsc/ramdisk.conf /mnt
	@sync;sync;sync
	@echo "done."
	@umount /mnt
	@printf "Populating configuration slice..."
	@mount /dev/$(MD)s4 /mnt
	@find Noizebox | cpio -pdvmu /mnt
	@sync;sync;sync
	@umount /mnt
	@mdconfig -d -u $(MD)
	@echo "done."
	@printf "Writing image to disk..."
	@dd if=$(DISKIMAGE) of=$(DEVICE) bs=64k > /dev/null 2>&1
	@echo "done."
clean:
	rm -rf Noizebox/*
