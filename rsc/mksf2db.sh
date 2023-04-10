#!/bin/sh
#
# This file is part of the Noizebox Project.
#
# Copyright (c)2016-2022,  Luc Hondareyte 
# All rights reserved.
#
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions
# are met:
#
# 1. Redistributions of source code must retain the above copyright
#    notice, this list of conditions and the following disclaimer.
#
# 2. redistributions in binary form must reproduce the above copyright
#    notice, this list of conditions and the following disclaimer in 
#    the documentation and/or other materials provided with the distribution.   
#
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" 
# AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
# IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE 
# ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
# FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
# DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
# OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
# HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
# LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
# OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
# SUCH DAMAGE.
#
LOG="/tmp/$(basename $0).$$"
FONTSDIR="/Library/Noizebox/SF2/"
FONTSLIST="${FONTSDIR}/*.lst"
SF2DB="/Applications/Noizebox/Resources/soundfont.conf"
PATH=$PATH:/usr/local/bin

Exec() {
	printf "$1... "
	shift ; 
	eval $* >> $LOG 2>&1
	if [ $? -ne 0 ] ; then
		echo "failed!"
		exit 1
	fi
	echo "done."
}

CreateSF2DB() {
	DB=$1
	sqlite3 $DB "CREATE TABLE bank ( name VARCHAR(12) NOT NULL, file VARCHAR(50) NOT NULL, key_offset SMALLINT, pitch_offset SMALLINT);"
}

if [ "$1" = "--empty" ] ; then
	Exec "Creating empty database" CreateSF2DB soundfont.conf
	sqlite3 soundfont.conf "INSERT INTO bank VALUES('--NO-SOUND--', 'null', 0, 0);"
        rm -f $LOG
	exit 0
fi

Exec "Deleting $SF2DB" rm -f $SF2DB
Exec "Creating $SF2DB" CreateSF2DB $SF2DB


ls $FONTSLIST > /dev/null 2>&1
if [ $? -ne 0 ] ; then
	sqlite3 $SF2DB "INSERT INTO bank VALUES('--NO-SOUND--', 'null', 0, 0);"
	echo "No soundfont to register."
	exit 0
fi

for FONT in $FONTSLIST
do
	grep -v ^# $FONT | while read n f k p junk
	do
		if [ -f "${FONTSDIR}/${f}" ] ; then
			Exec "Registering $f" sqlite3 $SF2DB \"INSERT INTO bank VALUES\(\'${n}\', \'${f}\', \'${k}\', \'${p}\'\)\;\"
		else
			echo "($(basename $f) does not exist, skipping) ..."
		fi
	done
done
rm -f $LOG
exit 0
