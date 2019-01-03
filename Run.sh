#!/bin/sh
#
#
ID=$(id -u)
if [ $ID -ne 0 ] ; then
	echo "You must be root to run this script."
	exit 1
fi
export DIR="./Noizebox"

HOST=$(uname -s)
ARCH=$(uname -p)
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
if [ "$rc" -ne 0 ]  && [ "$rc" -ne 42 ]; then
	cat $LOG 
fi
#rm -f $LOG
exit $rc
