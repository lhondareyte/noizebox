#!/bin/sh
#
#  $Id$
# 

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

OSNAME=$(uname -s)
ARCH=$(uname -m)
SYS="/${OSNAME}/${ARCH}"
APP=Noizebox
RESOURCE="${APP}/Resources/"
CONTENT="${APP}/Contents/${SYS}"
FRAMEWORK="${APP}/Frameworks/${SYS}"
if [ -e ${FRAMEWORK}/*so ] ; then
	chflags noschg ${FRAMEWORK}/*
fi
rm -rf ${APP}
mkdir -p ${APP}/tmp
Exec "Creating application tree" mkdir -p ${RESOURCE} ${CONTENT} ${FRAMEWORK}
Exec "Installing mksf2db.sh" install -m 755 -o root -g wheel ./rsc/mksf2db.sh ${RESOURCE}
Exec "Installing configuration file" install -m 644 -o root -g wheel ./rsc/noizebox.conf ${RESOURCE}

Exec "Installing Launch Script" install -m 755 rsc/noizebox.sh ${APP}/noizebox
Exec "Installing Executable" install -m 755 src/noizebox ${CONTENT}

ldd ${CONTENT}/noizebox | awk '!/not found/ && /=>/ {print $3}' | \
while read lib 
do 
	if [ ! -f ${FRAMEWORK}/$(basename $lib) ] ; then
		Exec "Installing $(basename $lib)" cp $lib ${FRAMEWORK}/ 
	fi
done
for lib in ./fluidsynth/build/src/libfluidsynth.so*
do
	if [ -L $lib ] ; then
		l_dest=$(ls -l $lib | awk '{print $NF}')
		cd $FRAMEWORK
		ln -s $l_dest $(basename $lib)
		cd -
	elif [ -f $lib ] ; then
		if [ ! -f ${FRAMEWORK}/$(basename $lib) ] ; then
			Exec "Installing $(basename $lib)" install -m 755 -o root -g wheel $lib ${FRAMEWORK}/
		fi
	fi
done
ldd ./fluidsynth/build/src/libfluidsynth*so.* | awk '!/not found/ && /=>/ {print $3}' | \
while read lib 
do 
	if [ ! -f ${FRAMEWORK}/$(basename $lib) ] ; then
		Exec "Installing $(basename $lib)" cp  $lib ${FRAMEWORK}/ 
	fi
done
Exec "Applying owner" chown -R root:wheel ${APP}
