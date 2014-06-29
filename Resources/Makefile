#!/bin/sh

CFG = noizebox.conf
all: conf

conf:
	@rm -f $(CFG)
	@sqlite3 $(CFG) "create table mixer (param varchar(12), val smallint);"
	@sqlite3 $(CFG) "insert into mixer values('left',100);"
	@sqlite3 $(CFG) "insert into mixer values('right',100);"
	@sqlite3 $(CFG) "create table midi (param varchar(12), val smallint);"
	@sqlite3 $(CFG) "insert into midi values('sensitivity',0);"
	@sqlite3 $(CFG) "insert into midi values('transpose',0);"
	@sqlite3 $(CFG) "insert into midi values('mode',1);"
	@sqlite3 $(CFG) "create table synth (param varchar(12), val smallint);"
	@sqlite3 $(CFG) "insert into synth values('detune',0);"
	@sqlite3 $(CFG) "create table soundfont (param varchar(12), val smallint);"
	@sqlite3 $(CFG) "insert into soundfont values('last_id',1);"

clean:
	@rm -f $(CFG)
install:
	
