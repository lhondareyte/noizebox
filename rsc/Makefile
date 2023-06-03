#
# SPDX-License-Identifier: BSD-2-Clause
#
# Copyright (c)2016,  Luc Hondareyte 
# All rights reserved.
#

CFG   = noizebox.conf
SQL   = noizebox.sql
SF2DB = soundfont.conf

all: nzconf sf2conf

sf2conf:
	@ ./mksf2db.sh --empty
nzconf:
	@printf "Building configuration database ..."
	@rm -f $(CFG)
	@sqlite3 $(CFG) < $(SQL)
	@echo " done."
clean:
	@rm -f $(CFG) $(SF2DB)
install:
	
