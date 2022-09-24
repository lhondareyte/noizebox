#!/bin/sh
#
# This file is part of the Noizebox Project.
#
# Copyright (c)2016-2017,  Luc Hondareyte
# All rights reserved.
#
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions
# are met:
#
# 1. Redistributions of source code must retain the above copyright
#    notice, this list of conditions and the following disclaimer.
#
# 2. redistributions in binary form must reproduce the above copyright
#    notice, this list of conditions and the following disclaimer in 
#    the documentation and/or other materials provided with the distribution.   
#
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" 
# AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
# IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE 
# ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
# FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
# DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
# OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
# HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
# LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
# OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
# SUCH DAMAGE.
#


app=noizebox
appdir=/Applications/Noizebox
osname=$(uname -s)
arch=$(uname -p)
platform="${osname}/${arch}"
app_bin=$appdir/Contents/$platform/$app
export PATH=$appdir:$PATH
ramdisk=YES
export NZDIR=$appdir

rc=1	

_pid=$(pgrep $app)

quiet () {
	$* > /dev/null 2>&1 ; return $?
}

save_config () {
	if [ -d /cfg ] ; then
		quiet mount /cfg
		if [ $? -eq 0 ] ; then
			quiet cp /etc/${app}.conf /cfg
			sync;sync;sync
			quiet umount /cfg
		fi
	fi
}

make_ramdisk () {
	if [ "$ramdisk" != "YES" ] && [ "$ramdisk" != "yes" ] ;  then
		return 0
	fi
	if [ -f /etc/ramdisk.conf ] ; then
		. /etc/ramdisk.conf
	else
		RAMDISK_SIZE=256m
	fi
	MD=$(mdconfig -a -t swap -s $RAMDISK_SIZE 2>/dev/null)
	[ $? -ne 0 ] && return 0
	quiet newfs -U /dev/$MD
	mount -t ufs /dev/$MD /Ramdisk
	mkdir -p /Ramdisk/SF2
	cd /Ramdisk/SF2
	for f in $(ls -S ${appdir}/Resources/SF2/*.sf2)
	do
		_f=$(basename $f)
       		quiet cp "$f" "${_f}.$$"
		if [ $? -eq 0 ] ; then
			quiet mv "${_f}.$$" "${_f}"
		else
       			rm -f "${_f}.$$"
		fi
	done	
	cd -
}

delete_ramdisk () {
	MD=$(mount | awk '/\/Ramdisk/ {print $1}')
	[ -z $MD ] && return 0
	quiet umount -f /Ramdisk
	quiet mdconfig -d -u ${MD} 
}


app_start () {
	if [ ! -z "$_pid" ] ; then
		# App is already running
		return 1
	fi
	if [ ! -f /etc/${app}.conf ] ; then
		quiet install -m 644 ${appdir}/Resources/${app}.conf /etc
	fi
	make_ramdisk &
	export LD_LIBRARY_PATH=${appdir}/Frameworks/${platform}
	echo $LD_LIBRARY_PATH
	if [ -x /usr/sbin/rtprio ] ; then
		/usr/sbin/rtprio 0 $app_bin $(ls /dev/umidi* 2> /dev/null) 2> /tmp/noizebox.log
	else
		$app_bin $(ls /dev/umidi* 2> /dev/null) 2>/dev/null
	fi
	# wait for make_ramdisk
	wait
	delete_ramdisk
}

app_stop () {
	if [ -z "$_pid" ] ; then
		return 0
	fi
	for p in $_pid 
	do
		kill $p
	done
	sleep 1
	save_config
	delete_ramdisk
}

case $1 in
	'start')
		app_start
		rc=$?
		;;
	'stop')
		app_stop 
		rc=$?
		;;
	'status')
		if [ ! -z $_pid ] ; then
			rc=0
		fi
		;;
esac
exit $rc
