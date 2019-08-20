#!/bin/sh
#
#  $Id$
# 

OSNAME=$(uname -s)
ARCH=$(uname -p)
SYS="/${OSNAME}/${ARCH}"
APP=$1
shift
EXTRALIBS=$*

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

app=$(echo $APP | tr [A-Z] [a-z])
RESOURCE="${APP}/Resources/"
CONTENT="${APP}/Contents/${SYS}"
FRAMEWORK="${APP}/Frameworks/${SYS}"

if [ -e "${FRAMEWORK}/*so" ] ; then
	chflags noschg ${FRAMEWORK}/*
fi
rm -rf ${APP}

Exec "Creating application folder" mkdir -p ${RESOURCE}/etc/devd ${CONTENT} ${FRAMEWORK}
Exec "Installing $1" install -m 755 src/$app ${CONTENT}

ldd ${CONTENT}/noizebox | awk '!/not found/ && /=>/ {print $3}' | while read lib 
do 
	if [ ! -f "${FRAMEWORK}/$(basename $lib)" ] ; then
		Exec "Installing $(basename $lib)" cp $lib ${FRAMEWORK}/ 
	fi
done

# Adding extra libraries
if [ ! -z "$EXTRALIBS" ] ; then
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
					Exec "Installing $(basename $lib)" install -m 755 $lib ${FRAMEWORK}/
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
fi
