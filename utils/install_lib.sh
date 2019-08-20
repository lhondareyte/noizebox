#!/bin/sh
#
#  $Id$
# 
echo $*

Exec () { 
	printf "$1 ... "
	shift 
	$* 
	if [ $? -ne 0 ] ; then
		echo "failed!"
		exit 1
	else
		echo "done."
	fi
}

Cp_lib () {
	APP=$2
	FRAMEWORK=$1
	ldd $APP | awk '!/not found/ && /=>/ {print $3}' | while read lib 
	do 
		if [ ! -f "${FRAMEWORK}/$(basename $lib)" ] ; then
			Exec "Installing $(basename $lib)" cp $lib ${FRAMEWORK}/ 
		fi
	done
}

Extra_lib () {
	FRAMEWORK=$1
	shift
	EXTRALIBS=$*
	# Adding extra libraries
	for LIB in $EXTRALIBS
	do
		for lib in $LIB
		do
			if [ -L $lib ] ; then
				l_dest=$(ls -l $lib | awk '{print $NF}')
				cd $FRAMEWORK
				ln -s $l_dest $(basename $lib)
				cd -
			elif [ -f $lib ] ; then
				if [ ! -f ${FRAMEWORK}/$(basename $lib) ] ; then
					Exec "Installing $(basename $lib)" install -m 755 $lib ${FRAMEWORK}
				fi
			fi
		done
		ldd $LIB | awk '!/not found/ && /=>/ {print $3}' | while read lib
		do
			if [ ! -f ${FRAMEWORK}/$(basename $lib) ] ; then
				Exec "Installing $(basename $lib)" install -m 755 $lib ${FRAMEWORK}/
			fi
		done
	done
}

if [ $1 == "-l" ] ; then
	shift
	ARG1=$1
	shift
	Extra_lib "$ARG1" "$*"
else
	Cp_lib "$1" "$2"
fi
