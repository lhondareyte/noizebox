#!/bin/sh
#
#
ARG="${1}"
SUDO=""
ID=$(id -u)
if [ $ID -ne 0 ] ; then
	SUDO="sudo"
fi
if [ "${ARG}" = "--local" ] ; then
	NZDIR="/Applications/Noizebox"
else
	NZDIR="./Noizebox"
fi

HOST="$(uname -s)"
ARCH="$(uname -m)"
APP="noizebox"
PLATFORM="${HOST}/${ARCH}"
BIN="${NZDIR}/Contents/${PLATFORM}/${APP}"
LOG="/tmp/nz.log"
rm -f ${LOG}

for i in /dev/umidi[0-9].[0-9]
do
	MIDI="${MIDI} ${i} "
done

#
# Set embbeded library path
LD_LIBRARY_PATH="${NZDIR}/Frameworks/${PLATFORM}"

if [ ! -d ${LD_LIBRARY_PATH} ] ; then
	echo "Error : cannot find ${LD_LIBRARY_PATH}"
	exit 1
fi

#
# Run application with high priority
if [ -x /usr/bin/rtprio ] ; then
	RT="/usr/sbin/rtprio"
fi
if [ -x ${BIN} ] ; then
	CMD="${SUDO} \
		LD_LIBRARY_PATH=${LD_LIBRARY_PATH} \
		NZDIR=${NZDIR} \
		LOG=${LOG} \
		${RT} ${BIN} ${MIDI} 2> ${LOG}"
	if [ "$1" = "--jack" ] ; then
		${CMD} &
	else
		${CMD}
	fi
	rc=${?}
else
	echo "Fatal error: ${BIN} not found!"
	rc="42"
fi
if [ "$rc" -ne 0 ] && [ "$rc" -ne 42 ]; then
	cat ${LOG} 
	exit ${rc}
fi

if [ "${ARG}" = "--jack" ] ; then
	jack_umidi connect jack_connect usb-umidi0.0:midi.TX noizebox:midi_00
	jack_connect noizebox:left system:playback_1
	jack_connect noizebox:right system:playback_2
fi
rm -f $LOG
