#!/bin/sh
#
#
. /etc/rc.subr

# KEYWORD: shutdown

name=noizebox
rcvar=noizebox_enable

start_cmd="${name}_start"
stop_cmd="${name}_stop"

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
	if [ -f /etc/ramdisk.conf ] ; then
		. /etc/ramdisk.conf
	else
		RAMDISK_SIZE=512m
		RAMDISK_LUN=3
	fi
	mdconfig -a -t swap -s ${RAMDISK_SIZE} -u ${RAMDISK_LUN}
	newfs -U md${RAMDISK_LUN} > /dev/null 2>&1
	mount -t ufs /dev/md${RAMDISK_LUN} /Ramdisk
	echo " done!"
	printf "Copying SF2 fonts ..."
	mkdir -p /Ramdisk/SF2
	for f in ${nzdir}/Resources/SF2/*.sf2
	do
		_f=$(basename $f)
       		cp "$f" "/Ramdisk/SF2/${_f}.$$"
		if [ $? -eq 0 ] ; then
			mv "/Ramdisk/SF2/${_f}.$$" "/Ramdisk/SF2/${_f}"
		fi
	done	
	echo " done!"
}


noizebox_start()
{
	RC_PID=$(getPID)
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
	rc_pid=$(getPID)
	printf $rc_pid > /var/run/${name}.pid
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
		printf "c" > $nztty
	done
	sleep 2
	sync;sync;sync
	echo " done."
	if [ -d /cfg ] ; then
		printf "Saving ${name} configuration..."
		sync;sync;sync
		mount /cfg
		if [ $? -eq 0 ] ; then
			for f in /cfg/*.conf
			do
				_f=${basename "$f"}
				if [ -f "/etc/${_f}" ] ; then
					cp "/etc/${_f}" /cfg
				fi
			done
			umount_cfg
			echo " done."
		else
			echo " failed!"
		fi
	fi
	umount /Ramdisk
	mdconfig -d -u ${RAMDISK_LUN} > /dev/null 2>&1
	rm -f /var/run/${name}.pid
}

load_rc_config $name
run_rc_command "$1"
