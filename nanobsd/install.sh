#!/bin/sh
#
NANOSCRIPT="/usr/src/tools/tools/nanobsd/nanobsd.sh"
KCONF=NOIZEBOX.konf
IMAGE="/usr/obj/nanobsd.noizebox/_.disk.full"

cp ${KCONF} /usr/src/sys/$(uname -m)/conf/NOIZEBOX

if [ -f $IMAGE ] ; then
	sh ${NANOSCRIPT} -b -c NOIZEBOX.conf
else
	sh ${NANOSCRIPT} -c NOIZEBOX.conf
fi
MD=$(mdconfig -f $IMAGE)
printf "y\ny\n" > /tmp/rep.$$
fdisk -B  /dev/${MD} < /tmp/rep.$$ > /dev/null 2>&1
rm -f /tmp/rep.$$
mount /dev/${MD}s3 /mnt
cp ../noizebox.conf /mnt
cp ../noizebox.conf /mnt/noizebox.conf.install
sync;sync;sync
umount /mnt
mdconfig -d -u ${MD}
printf "Please enter destination device (ex: /dev/ada1) :"
read device
if [ -c ${device} ] ; then
 dd if=${IMAGE} of=${device} bs=64k
fi
