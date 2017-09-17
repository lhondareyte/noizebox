#!/bin/sh
#
#  $Id$
# 

Done() { echo " done." 
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
install -m 755 -o root -g wheel ./rsc/noizebox.rc.d ${APP}/tmp
printf "Creating application tree ..."
mkdir -p ${RESOURCE}/SF2
mkdir -p ${CONTENT}
mkdir -p ${FRAMEWORK}
Done
if [ -d sf2 ] ; then
	printf "Installing SF2 files ..."
	cp sf2/*.sf2 ${RESOURCE}/SF2
	cp sf2/soundfont.conf  ${RESOURCE}
	Done
fi
printf "Installing Executable ..."
install -m 755 rsc/noizebox.${OSNAME} ${APP}/noizebox
install -m 755 src/noizebox ${CONTENT}
Done
printf "Installing System libraries ..."
ldd ${CONTENT}/noizebox | awk '!/not found/ && /=>/ {print $3}' | \
	 while read l 
	 do 
		cp $l ${FRAMEWORK}/ 
	 done
Done
printf "Installing Application libraries ..."
for lib in ./fluidsynth/build/src/libfluidsynth.so*
do
	if [ -L $lib ] ; then
		l_dest=$(ls -l $lib | awk '{print $NF}')
		cd $FRAMEWORK
		ln -s $l_dest $(basename $lib)
		cd -
	elif [ -f $lib ] ; then
		install -m 755 -o root -g wheel $lib ${FRAMEWORK}/
	fi
done
ldd ./fluidsynth/build/src/libfluidsynth*so.* | awk '!/not found/ && /=>/ {print $3}' | \
	while read lib 
	do 
		cp  $lib ${FRAMEWORK}/ 
	done
Done
chown -R root:wheel ${APP}
printf "Creating tar ball ..."
tar czf noizebox.pkg ${APP}
Done
printf "Creating signature file ..."
md5 noizebox.pkg > noizebox.md5
Done
