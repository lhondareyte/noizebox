--
-- SPDX-License-Identifier: BSD-2-Clause
--
-- Copyright (c)2016-2022,  Luc Hondareyte 
-- All rights reserved.
--
CREATE TABLE dsp (PARAM VARCHAR(12), VAL SMALLINT);
INSERT INTO dsp VALUES('device',1);
CREATE TABLE mixer (PARAM VARCHAR(12), VAL SMALLINT);
INSERT INTO mixer VALUES('left',100);
INSERT INTO mixer VALUES('right',100);
CREATE TABLE midi (PARAM VARCHAR(12), VAL SMALLINT);
INSERT INTO midi VALUES('sensitivity',0);
INSERT INTO midi VALUES('transpose',0);
INSERT INTO midi VALUES('mode',1);
INSERT INTO midi VALUES('breath_curve',1);
CREATE TABLE synth (PARAM VARCHAR(12), VAL SMALLINT);
INSERT INTO synth VALUES('detune',0);
INSERT INTO synth VALUES('reverb',0);
INSERT INTO synth VALUES('chorus',0);
CREATE TABLE soundfont (PARAM VARCHAR(12), VAL SMALLINT);
INSERT INTO soundfont VALUES('last_id',1);
INSERT INTO soundfont VALUES('last_preset',1);
CREATE TABLE paths (PARAM VARCHAR(20), VAL VARCHAR(200));
INSERT INTO paths VALUES('sf2_library_path', '/Library/Noizebox/SF2');
INSERT INTO paths VALUES('sf2_ramdisk_path', '/Ramdisk/SF2');
