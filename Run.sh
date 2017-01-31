#!/bin/sh
#
#
ID=$(id -u)
if [ $ID -ne 0 ] ; then
	echo "You must be root to run this scipt."
	exit 1
fi
export DIR="/home/luc/noizebox/Noizebox"

HOST=$(uname -s)
ARCH=$(uname -p)
APP=noizebox
PLATFORM="${HOST}/${ARCH}"
BIN="${DIR}/Contents/${PLATFORM}/${APP}"
LOG=/tmp/nz.log
MIDI="/dev/umidi0.0 /dev/umidi1.0 /dev/umidi2.0"
#MIDI="/dev/umidi2.0 /dev/umidi1.0"
export NZDIR=$DIR

#
# Set embbeded library path
LD_LIBRARY_PATH="${DIR}/Frameworks/${PLATFORM}"
export LD_LIBRARY_PATH

#
# Run application with high priority
RT="/usr/sbin/rtprio"
if [ -x ${BIN} ] ; then
	if [ -x ${RT} ] ; then
		${RT} 0 ${BIN} ${MIDI} 2> $LOG
		rc=$?
	else
		${BIN} ${MIDI} 2> $LOG
		rc=$?
	fi
else
	echo "Fatal error: ${BIN} not found!"
	rc=42
fi
if [ $rc -ne 0 ] ; then
	cat $LOG
fi
exit $rc
