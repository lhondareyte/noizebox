#!/bin/sh
#
#
ARG="${1}"
SUDO=""
ID=$(id -u)
if [ $ID -ne 0 ] ; then
	export SUDO="sudo -E"
fi
if [ "${ARG}" = "--local" ] ; then
	NZDIR="/Applications/Noizebox"
	shift
else
	NZDIR="./Noizebox"
fi

HOST="$(uname -s)"
ARCH="$(uname -m)"
APP="noizebox"
export NZDIR
export PLATFORM="${HOST}/${ARCH}"
export BIN="${NZDIR}/Contents/${PLATFORM}/${APP}"
export LOG="/tmp/nz.log"
${SUDO} rm -f ${LOG}

for i in /dev/umidi[0-9].[0-9]
do
	export MIDI="${MIDI} ${i} "
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
if kldstat -qm mac_priority ; then
	export RT="/usr/sbin/rtprio 0"
fi

if [ -x ${BIN} ] ; then
	if [ "$1" = "--jack" ] ; then
		${SUDO} LD_LIBRARY_PATH=${LD_LIBRARY_PATH}\
			 ${RT} ${BIN} ${MIDI} 2> ${LOG} &
	else
		${SUDO} LD_LIBRARY_PATH=${LD_LIBRARY_PATH} \
			 ${RT} ${BIN} ${MIDI} 2> ${LOG}
		rc=${?}
	fi
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
${SUDO} rm -f $LOG
