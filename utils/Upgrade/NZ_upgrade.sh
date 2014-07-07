#!/bin/sh
#
# $Id$


export TERM=tinyVT

export PATH="/bin:/sbin:/usr/bin:/usr/sbin"

mnt="/media/flashdrive"
tty="/dev/cuaU0"
device="$1"

Ask_user ()
{
	NZ_dialog $* < $tty > $tty
	return $?
}

StopApp ()
{
	/usr/sbin/service noizebox stop > /dev/null 2>&1
	printf "0" > $tty
}

StartApp ()
{
	/usr/sbin/service noizebox start > /dev/null 2>&1
}

_exit ()
{
	cd
	umount $mnt
	StartApp
	exit 0
}

Exec ()
{
	$* > /dev/null 2>&1
	if [ $? -ne 0 ] ; then
		printf "E Erreur lors de la mise\n a jour. Abandon!" > $tty
		sleep 2
		_exit 0
	fi
}


mount -t msdosfs -o ro $device $mnt
if [ $? -ne 0 ] ; then
	exit 0
fi

if [ -f ${mnt}/noizebox.tgz -a  -f ${mnt}/noizebox.md5 ] ; then
	MD5=$(. ${mnt}/noizebox.md5)
	/sbin/md5 -c $MD5 ${mnt}/noizebox.tgz 
	if [ $? -ne 0 ] ; then
		_exit 0
	fi
else
	_exit 0
fi

StopApp

printf  "EConfirmer la mise a jour\nOui                  Non"  &&  NZ_ask4key
if [ $? -ne 0 ] ; then
	printf  "E   Mise a jour annulee\n  Redemarrage en cours"
	sleep 1
	_exit 0
fi


Exec umount /Applications
Exec mount /Upgrade
cd /Upgrade
mv Noizebox Noizebox.sav
tar xvzf ${mnt}/noizebox.tgz
if [ $? -ne 0 ] ; then
	rm -rf Noizebox
	mv Noizebox.sav Noizebox
	cd
	umount /Upgrade
	mount /Applications
	printf "E Erreur lors de la mise\n a jour. Abandon!" > $tty
	sleep 2
	_exit 0
else
	cd
	Exec umount /Upgrade
	Exec mount /Applications

	printf "E    Mise a jour OK.  \n    Redemarrage..." > $tty
	sleep 3
	_exit 0
fi

exit 0
