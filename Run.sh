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
#MIDI="/dev/umidi0.0 /dev/umidi1.0 /dev/umidi2.0"
MIDI="/dev/umidi2.0 /dev/umidi1.0"
export NZDIR=$DIR

#
# Set embbeded library path
LD_LIBRARY_PATH="${DIR}/Frameworks/${PLATFORM}"
export LD_LIBRARY_PATH

#
# Run application
RT="/usr/sbin/rtprio"
if [ -x ${BIN} ] ; then
	if [ -x ${RT} ] ; then
		exec ${RT} 0 ${BIN} ${MIDI} 2> $LOG
	else
		exec ${BIN} ${MIDI} 2> $LOG
	fi
fi
exit 0
