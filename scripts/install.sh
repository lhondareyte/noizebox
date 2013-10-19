#!/bin/sh
USERID=$(id -u)
OSNAME=$(uname -s)
CPU=$(uname -m)
ARCH="${OSNAME}/${CPU}"
if [ $USERID -ne 0 ] ; then
	echo "You must be superuser to install this application."
	exit 1
fi
PREFIX=${PREFIX}/Applications
CONTENTS=${PREFIX}/Contents/${ARCH}
FRAMEWORKS=${PREFIX}/Frameworks/${ARCH}
RESOURCES=${PREFIX}/Resources/
SF2=${PREFIX}/Resources/SF2
printf "Creating Application hierarchy..."
mkdir -p $SF2 $CONTENTS $FRAMEWORKS
echo "done."

InstalLibs()
{
	ldd $BINARY | awk '/local/ {print $3}' | while read lib
	do
		install -m 755 $lib ${FRAMEWORKS}/
	done
}

InstalConfiguration()
{

	sqlite3 noizebox.conf << !EOF!
	create table mixer (param varchar(10), val smallint);
	insert into mixer values('left',90);
	insert into mixer values('right',90);
	create table midi (param varchar(10), val smallint);
	insert into midi values('sens',0);
	insert into midi values('transpose',0);
	create table soundfont (param varchar(10), val smallint);
	insert into soundfont values('last_id',1);
	.exit
	!EOF!
	if [ $? -eq 0 ] ; then
		install -m 640 noizebox.conf ${RESOURCES}
		rm -f noizebox.conf
	fi

	sqlite3 soundfont.conf << !EOF!
	create table bank (id smallint, name varchar(8), file varchar(50));
	insert into bank values(1,'Accord.1','Accordion EWI-1.sf2');
	insert into bank values(2,'Accord.2','Accordion EWI.sf2');
	insert into bank values(3,'Diato1  ','Diato1-EWI.sf2');
	insert into bank values(4,'Diato2  ','Diato2-EWI.sf2');
	insert into bank values(5,'Melodeon','Melodeon EWI.sf2');
	insert into bank values(6,'Flute1  ','Cambridge-Flute-EWI.sf2');
	insert into bank values(7,'Flute2  ','GoodFlute-EWI.sf2');
	insert into bank values(8,'Flute3  ','WT_Flute-EWI.sf2');
	insert into bank values(9,'Oboe    ','Oboe EWI.sf2');
	insert into bank values(10,'A.Sax1  ','Alto Sax Hard EWI.sf2');
	insert into bank values(11,'A.Sax2  ','Alto Sax Soft EWI.sf2');
	insert into bank values(12,'A.Sax3  ','alto sax 2.sf2');
	insert into bank values(13,'T.Sax1  ','Tenor Sax Hard EWI.sf2');
	insert into bank values(14,'T.Sax2  ','Tenor Sax Soft EWI.sf2');
	insert into bank values(15,'Bombard1','bombarde 3Oct EWI.sf2');
	insert into bank values(16,'Bombard2','bombarde-EWI.sf2');
	insert into bank values(17,'Bombard3','bombardetenor-EWI.sf2');
	insert into bank values(18,'Kazoo   ','Kazoo-EWI.sf2');
	.exit
	!EOF!
	if [ $? -eq 0 ] ; then
		install -m 640 soundfont.conf ${RESOURCES}
		rm -f soundfont.conf
	fi
}

install -m 755 noizebox ${CONTENTS}/noizebox.bin
install -m 660 noizebox.${OSNAME} ${CONTENTS}/noizebox.env

