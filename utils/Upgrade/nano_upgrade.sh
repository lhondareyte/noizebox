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
	NZ_dialog "$1" "$2" < $tty > $tty
	r=$?
	for i in 1 2
	do
		printf "c" > $tty
	done
	return $r
}

_exit ()
{
	cd
	umount $mnt
	/usr/sbin/service noizebox start 
	exit 0
}

Exec ()
{
	$* 
	if [ $? -ne 0 ] ; then
		printf "E Erreur lors de la mise\n a jour. Abandon!" > $tty
		sleep 2
		_exit 0
	fi
}

PostExec ()
{
	r=0
	post="/Upgrade/postexec.sh"
	if [ -x $post ] ;  then
		$post
		r=$?
		rm -f $post
	fi
	sync ; sync; sync
	return $r
}

mount -t msdosfs -o ro $device $mnt
if [ $? -ne 0 ] ; then
	exit 0
fi

if [ -f ${mnt}/noizebox.tgz -a  -f ${mnt}/noizebox.md5 ] ; then
	. ${mnt}/noizebox.md5
	/sbin/md5 -c $MD5 ${mnt}/noizebox.tgz 
	if [ $? -ne 0 ] ; then
		_exit 0
	fi
else
	_exit 0
fi

#
# Arret de l'application
/usr/sbin/service noizebox stop 
printf "0" > $tty

Ask_user  "Confirmer la mise a jour" "Oui                  Non"
if [ $? -ne 0 ] ; then
	printf  "E  Mise a jour annulee\n  Redemarrage en cours" > $tty
	sleep 2
	_exit 0
fi


printf "E  Mise a jour en cours  \n  Veuillez patientez... " > $tty

#
# Demontage de /Applications read-only
Exec umount /Applications

#
# Montage de /Upgrade read-write
Exec mount /Upgrade
cd /Upgrade
mv Noizebox Noizebox.sav
tar xzf ${mnt}/noizebox.tgz && PostExec
if [ $? -ne 0 ] ; then
	rm -rf Noizebox
	mv Noizebox.sav Noizebox
	cd
	umount /Upgrade
	mount /Applications
	sync;sync;sync
	printf "E Erreur lors de la mise\n a jour. Abandon!" > $tty
	sleep 2
	_exit 0
else
	cd
	Exec umount /Upgrade
	Exec mount /Applications

	printf "E    Mise a jour OK.  \n  Redemarrage en cours" > $tty
	sleep 3
	_exit 0
fi
exit 0
