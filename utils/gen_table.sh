#!/bin/sh
#
#
low=$1
if [ -z "$1" ] ; then
	echo "Syntax error!"
	exit 1
fi
b=$(expr 127 - $low)
for i in $(seq 0 127)
do
	echo "scale=10;($b/16129)*$i*$i+$low" | bc | awk -F"." '{printf "%s, \n",$1}'
done
