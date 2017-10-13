#!/bin/sh
LOG="/tmp/$(basename $0).$$"
FONTSDIR="/Library/Noizebox/Resources/SF2/"
FONTS="${FONTSDIR}/*.lst"
SF2DB="/Library/Noizebox/Resources/soundfont.conf"

ls $FONTS > /dev/null 2>&1
if [ $? -ne 0 ] ; then
	echo "No SF2 to register."
	exit 0
fi

Exec() {
	printf "$1 ..."
	shift ; 
	eval $* >> $LOG 2>&1
	if [ $? -ne 0 ] ; then
		echo "failed!"
		exit 1
	fi
	echo "done."
}

Exec "Deleting $SF2DB" rm -f $SF2DB
Exec "Creating $SF2DB" sqlite3 $SF2DB \"create table bank \(id smallint, name varchar\(12\), file varchar\(50\) , key_offset smallint, pitch_offset smallint\)\;\"

i=1
for FONT in $FONTS
do
	grep -v ^# $FONT | while read n f k p junk
	do
		if [ -f "${FONTSDIR}/${f}" ] ; then
			Exec "Registering $f" sqlite3 $SF2DB \"insert into bank values\(${i}, \'${n}\', \'${f}\', \'${k}\', \'${p}\'\)\;\"
			let i+=1 > /dev/null
		else
			echo "($(basename $f) does not exist, skipping) ..."
		fi
	done
done
rm -f $LOG
exit 0
