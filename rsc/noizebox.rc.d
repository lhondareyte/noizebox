#!/bin/sh
#
# This file is part of the Noizebox Project.
#
# Copyright (c)2016,  Luc Hondareyte <lhondareyte_AT_laposte.net>.
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
. /etc/rc.subr

# KEYWORD: shutdown

name=noizebox
rcvar=noizebox_enable
start_cmd="${name}_start"
stop_cmd="${name}_stop"

if [ -f /etc/ramdisk.conf ] ; then
	. /etc/ramdisk.conf
else
	RAMDISK_SIZE=512m
	RAMDISK_LUN=3
fi

nzdir="/Applications/Noizebox"
nztty="/dev/cuaU0"
nz=$(basename $nzdir)
PATH=$nzdir:$PATH

getPID ()
{
	ps ax | awk '/'"${nz}"'/ && !/awk/ && /'"${name}"'/ {print $1}'
}

umount_cfg ()
{
	m=$(df /cfg | awk '!/Filesystem/ { print $6 }')
	[ "$m" == "/cfg" ] && umount /cfg
}

mkramdisk ()
{
	printf "Building RAMDISK ..."
	mdconfig -a -t swap -s ${RAMDISK_SIZE} -u ${RAMDISK_LUN}
	newfs -U md${RAMDISK_LUN} > /dev/null 2>&1
	mount -t ufs /dev/md${RAMDISK_LUN} /Ramdisk
	echo " done!"
	printf "Copying SF2 fonts ..."
	mkdir -p /Ramdisk/SF2
	cd ${nzdir}/Resources/SF2/*.sf2
	for f *.sf2
	do
       		cp "$f" "/Ramdisk/SF2/${f}.$$"
		if [ $? -eq 0 ] ; then
			mv "/Ramdisk/SF2/${f}.$$" "/Ramdisk/SF2/${f}"
		else
       			rm "/Ramdisk/SF2/${f}.$$"
		fi
	done	
	cd -
	echo " done!"
}


noizebox_start()
{
	rc_pid=$(getPID)
	if [ ! -z "$rc_pid" ] ; then
		echo "$name already running? ($rc_pid)"
		return 1
	fi
	printf "Waiting for $nztty to be ready ..."
	while [ ! -c $nztty ] 
	do
		:	
	done
	printf "done.\nStarting ${name}.\n"
	$name &
	mkramdisk
	rc_pid=$(getPID)
	echo $rc_pid > /var/run/${name}.pid
}

noizebox_stop()
{
	rc_pid=$(getPID)
	if [ -z "$rc_pid" ] ; then
		_run_rc_notrunning
		return 1
	fi
	printf "Stopping ${name}..."
	kill $rc_pid 
	#
	# Effacement du curseur
	for i in 1 2
	do
		printf "\033c" > $nztty
	done
	sleep 2
	sync;sync;sync
	echo " done."
	if [ -d /cfg ] ; then
		printf "Saving ${name} configuration..."
		sync;sync;sync
		mount /cfg
		if [ $? -eq 0 ] ; then
			cp /etc/noizebox.conf /cfg
			sync;sync;sync
			umount_cfg
			echo " done."
		else
			echo " failed!"
		fi
	fi
	umount /Ramdisk
	mdconfig -d -u${RAMDISK_LUN} > /dev/null 2>&1
	rm -f /var/run/${name}.pid
}

load_rc_config $name
run_rc_command "$1"
