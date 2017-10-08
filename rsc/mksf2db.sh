#!/bin/sh
LOG="/tmp/$(basename $0).$$"
FONTS="/Applications/Noizebox/Resources/SF2/*.lst"
SF2DB="/Applications/Noizebox/Resources/soundfont.conf"
if [ -z "$FONTS" ] ; then
	exit 0
fi

rm -f $LOG

Exec() {
	printf "$1 ..."
	shift ; $* >> $LOG 2>&1
	if [ $? -ne 0 ] ; then
		echo "failed!"
		exit 1
	fi
	echo "done."
}

Exec "Deleting $SF2DB" rm $SF2DB
Exec "Creating $SF2DB" sqlite3 $SF2DB "create table bank (id smallint, name varchar(12), file varchar(50) , key_offset smallint, pitch_offset smallint);"

i=1
for FONT in $FONTS
do
	printf "Building SF2 database for $FONT ... "
	grep -v ^# $FONT | while read n f k p junk
	do
		if [ -f "${f}" ] ; then
			sqlite3 $SF2DB "insert into bank values(${i}, '${n}', '${f}', '${k}', '${p}');"
			let i+=1 > /dev/null
		else
			printf "($(basename $f) does not exist, skipping) ..."
		fi
	done
	echo done
done
exit 0
