#!/bin/sh
#
ARG="${1}"
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

Error() {
	echo "Error: ${*}!"
	exit 1
}

APP="noizebox"
HOST="$(uname -s)"
ARCH="$(uname -m)"
export NZDIR
export PLATFORM="${HOST}/${ARCH}"
export BIN="${NZDIR}/Contents/${PLATFORM}/${APP}"
export LOG="/tmp/nz.log"
${SUDO} rm -f ${LOG}

if [ "${HOST}" = "FreeBSD" ] ; then
	MIDIDEVS="/dev/umidi[0-9].[0-9]"
	# Run application with high priority
	if kldstat -qm mac_priority ; then
		export RT="/usr/sbin/rtprio 0"
	fi
elif [ "${HOST}" = "Linux" ] ; then
	if ! grep -qw snd_rawmidi /proc/modules ; then
		Error "snd_rawmidi kernel module not loaded"
	fi
	MIDIDEVS="/dev/midi[1-9]"
elif [ "${HOST}" = "NetBSD" ] ; then
	MIDIDEVS="/dev/rmidi[1-9]"
elif [ "${HOST}" = "OpenBSD" ] ; then
	MIDIDEVS="/dev/rmidi[0-9]"
else
	Error "unsupported operating system"
fi

for i in ${MIDIDEVS}
do
	export MIDI="${MIDI} ${i} "
done

#
# Set embbeded library path
LD_LIBRARY_PATH="${NZDIR}/Frameworks/${PLATFORM}"

if [ ! -d ${LD_LIBRARY_PATH} ] ; then
	Error "cannot find ${LD_LIBRARY_PATH}"
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
	Error "${BIN} not found"
fi

if [ "${ARG}" = "--jack" ] ; then
	jack_umidi connect jack_connect usb-umidi0.0:midi.TX noizebox:midi_00
	jack_connect noizebox:left system:playback_1
	jack_connect noizebox:right system:playback_2
fi

