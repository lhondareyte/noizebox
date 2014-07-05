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


noizebox_start()
{
	rc_pid=$(getPID)
	if [ ! -z "$rc_pid" ] ; then
		echo "$name already running? ($rc_pid)"
		return 1
	fi
	echo "Starting ${name}."
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
	echo "c" > /dev/cuaU0
	sleep 2
	sync;sync;sync
	echo " done."
	if [ -d /cfg ] ; then
		printf "Saving ${name} configuration..."
		sync;sync;sync
		mount /cfg
		if [ $? -eq 0 ] ; then
			cp /etc/noizebox.conf /cfg/noizebox.conf
			umount_cfg
			echo " done."
		else
			echo " failed!"
		fi
	fi
	rm -f /var/run/${name}.pid
}

load_rc_config $name
run_rc_command "$1"
