#!/bin/sh
#
#
ID=$(id -u)
if [ $ID -ne 0 ] ; then
	echo "You must be root to run this script."
	exit 1
fi
if [ "$1" = "--local" ] ; then
	export DIR="/Applications/Noizebox"
else
	export DIR="./Noizebox"
fi

HOST=$(uname -s)
ARCH=$(uname -m)
APP=noizebox
PLATFORM="${HOST}/${ARCH}"
BIN="${DIR}/Contents/${PLATFORM}/${APP}"
LOG=/tmp/nz.log
export NZDIR=$DIR
> $LOG

for i in /dev/umidi[0-9].[0-9]
do
	MIDI="$MIDI $i "
done

#
# Set embbeded library path
LD_LIBRARY_PATH="${DIR}/Frameworks/${PLATFORM}"
export LD_LIBRARY_PATH

#
# Run application with high priority
if [ -x /usr/bin/rtprio ] ; then
	RT="/usr/sbin/rtprio"
fi
if [ -x ${BIN} ] ; then
	if [ "$1" = "--jack" ] ; then
		${RT} ${BIN} ${MIDI} 2> $LOG &
	else
		${RT} ${BIN} ${MIDI} 2> $LOG
	fi
	rc=$?
else
	echo "Fatal error: ${BIN} not found!"
	rc=42
fi
if [ "$rc" -ne 0 ]  && [ "$rc" -ne 42 ]; then
	cat $LOG 
	exit $rc
fi

if [ "$1" = "--jack" ] ; then
	jack_umidi connect jack_connect usb-umidi0.0:midi.TX noizebox:midi_00
	jack_connect noizebox:left system:playback_1
	jack_connect noizebox:right system:playback_2
fi
rm -f $LOG
