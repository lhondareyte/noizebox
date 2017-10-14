#!/bin/sh
#
# This file is part of the Noizebox Project.
#
# Copyright (c)2016-2017,  Luc Hondareyte <lhondareyte_AT_laposte.net>.
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


app=$(basename $0)
osname=$(uname -s)
arch=$(uname -p)
platform="${osname}/${arch}"
appdir="/Applications/$(echo $app | awk '{print toupper(substr($0,0,1))tolower(substr($0,2))}')"
appdir="/Library/$(echo $app | awk '{print toupper(substr($0,0,1))tolower(substr($0,2))}')"
export PATH=$appdir:$PATH
timeout=30
ramdisk=YES

rc=1	

_pid=$(pgrep $app)

quiet () {
	$* > /dev/null 2>&1
	return $?
}

save_config () {
	if [ -d /cfg ] ; then
		sync;sync;sync
		quiet mount /cfg
		if [ $? -eq 0 ] ; then
			quiet cp /etc/${app}.conf /cfg
			sync;sync;sync
			quiet umount /cfg
		fi
	fi
}

make_ramdisk () {
	if [ "$RAMDISK" != "YES" ] || [ "$RAMDISK" != "yes" ] ;  then
		return 0
	fi
	if [ -f /etc/ramdisk.conf ] ; then
		. /etc/ramdisk.conf
	else
		RAMDISK_SIZE=512m
		RAMDISK_LUN=3
	fi
	quiet mdconfig -a -t swap -s ${RAMDISK_SIZE} -u ${RAMDISK_LUN}
	quiet newfs -U md${RAMDISK_LUN} > /dev/null 2>&1
	quiet mount -t ufs /dev/md${RAMDISK_LUN} /Ramdisk
	mkdir -p /Ramdisk/SF2
	for f in "${applib}/Resources/SF2/*.sf2"
	do
       		quiet cp "$f" "/Ramdisk/SF2/${f}.$$"
		if [ $? -eq 0 ] ; then
			mv "/Ramdisk/SF2/${f}.$$" "/Ramdisk/SF2/${f}"
		else
       			rm -f "/Ramdisk/SF2/${f}.$$"
		fi
	done	
}

delete_ramdisk () {
	i=1
	while :
	do
		quiet umount /Ramdisk
		if [ $? -eq 0 ] || [ $i == $timeout ] ; then
			break
		else
			let i+=1 > /dev/null 2>&1
			sleep 1
		fi
	done
	quiet mdconfig -d -u${RAMDISK_LUN} 
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
	if [ -x /usr/sbin/trprio ] ; then
		/usr/sbin/rtprio 0 $app_bin $midi
	else
		$app_bin $midi
	fi
}

app_stop () {
	if [ -z "$_pid" ] ; then
		return 0
	fi
	kill $_pid 
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
