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
Exec "Installing noizebox.rc.d" install -m 755 -o root -g wheel ./rsc/noizebox.rc.d ${APP}/tmp
Exec "Installing mksf2db.sh" install -m 755 -o root -g wheel ./rsc/mksf2db.sh ${RESOURCE}

Exec "Installing Executable" install -m 755 rsc/noizebox.${OSNAME} ${APP}/noizebox
Exec "Installing Executable" install -m 755 src/noizebox ${CONTENT}

ldd ${CONTENT}/noizebox | awk '!/not found/ && /=>/ {print $3}' | \
while read l 
	do 
		Exec "Installing $l" cp $l ${FRAMEWORK}/ 
	done
for lib in ./fluidsynth/build/src/libfluidsynth.so*
do
	if [ -L $lib ] ; then
		l_dest=$(ls -l $lib | awk '{print $NF}')
		cd $FRAMEWORK
		ln -s $l_dest $(basename $lib)
		cd -
	elif [ -f $lib ] ; then
		Exec "Installing $lib" install -m 755 -o root -g wheel $lib ${FRAMEWORK}/
	fi
done
ldd ./fluidsynth/build/src/libfluidsynth*so.* | awk '!/not found/ && /=>/ {print $3}' | \
while read lib 
do 
	Exec "Installing $lib" cp  $lib ${FRAMEWORK}/ 
done
Exec "Applying owner" chown -R root:wheel ${APP}
