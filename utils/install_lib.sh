#!/bin/sh
#
#  $Id$
# 
Exec () { 
	$* 
	if [ $? -ne 0 ] ; then
		echo "failed!"
		exit 1
	else
		echo " done."
	fi
}

Install_lib () {
	_app=$2
	_framework=$1
	ldd -a ${_app} | awk '!/not found/ && /=>/ {print $3}' | while read lib 
	do 
		_lib=$(basename ${lib})
		if ([ -f "${lib}" ] && [ ! -f "${_framework}/${_lib}" ])  ; then
			printf "Installing $(basename $lib)" 
			Exec install -m 755 "${lib}" "${_framework}/"
		fi
		unset _lib
	done
	unset _app _framework 
}

Install_lib "$1" "$2"
