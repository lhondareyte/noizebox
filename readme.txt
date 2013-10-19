Noizebox SF2 synthesizer based on Fluidsynth.
It can run in console terminal (curses linked) but it's goal is 
to be run with AVR-tinyVT in a computer appliance.

Needs libraries are:
/usr/local/lib/libfluidsynth.so.6
/usr/local/lib/libglib-2.0.so.0
/usr/local/lib/libgthread-2.0.so.0
/usr/local/lib/libiconv.so.3
/usr/local/lib/libintl.so.9
/usr/local/lib/libpcre.so.1
/usr/local/lib/libsqlite3.so.1

Fully tested with NanoBSD (i386 and amd64 platforms).

Building software:

	To build Noizebox on FreeBSD,  do the following step
	
		pkg_add -r sqlite3
		pkg_add -r glib20
		pkg_add -r fluidsynth
		pkg_add -r gmake

	Build binaries:
		cd ./src
		gmake &
	Install application:
		cd ../script
		./install.sh --prefix=/what/ever/you/want
	Running application:
		cd /what/ever/you/want/Applications/noizebox.app
		./noizebox

	Enjoy

Creating new presets for Noizebox:

	Since Noizebox cannot handle multiple instruments/presets in a soundbank, you
	should use soundfont with only one instrument/preset.
	
