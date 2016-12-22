#
# This file is part of the Noizebox Project.
#
# Copyright (c)2016,  Luc Hondareyte <lhondareyte_AT_laposte.net>.
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

CFG = noizebox.conf
all: conf

conf:
	@printf "Building configuration database ..."
	@rm -f $(CFG)
	@sqlite3 $(CFG) "create table dsp (param varchar(12), val smallint);"
	@sqlite3 $(CFG) "insert into dsp values('device',1);"
	@sqlite3 $(CFG) "create table mixer (param varchar(12), val smallint);"
	@sqlite3 $(CFG) "insert into mixer values('left',100);"
	@sqlite3 $(CFG) "insert into mixer values('right',100);"
	@sqlite3 $(CFG) "create table midi (param varchar(12), val smallint);"
	@sqlite3 $(CFG) "insert into midi values('sensitivity',0);"
	@sqlite3 $(CFG) "insert into midi values('transpose',0);"
	@sqlite3 $(CFG) "insert into midi values('mode',1);"
	@sqlite3 $(CFG) "insert into midi values('breath_curve',1);"
	@sqlite3 $(CFG) "create table synth (param varchar(12), val smallint);"
	@sqlite3 $(CFG) "insert into synth values('detune',0);"
	@sqlite3 $(CFG) "create table soundfont (param varchar(12), val smallint);"
	@sqlite3 $(CFG) "insert into soundfont values('last_id',1);"
	@echo " done."
clean:
	@rm -f $(CFG)
install:
