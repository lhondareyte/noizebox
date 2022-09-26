--
-- Copyright (c)2016-2022,  Luc Hondareyte 
-- All rights reserved.
--
-- Redistribution and use in source and binary forms, with or without
-- modification, are permitted provided that the following conditions
-- are met:
--
-- 1. Redistributions of source code must retain the above copyright
--    notice, this list of conditions and the following disclaimer.
--
-- 2. redistributions in binary form must reproduce the above copyright
--    notice, this list of conditions and the following disclaimer in 
--    the documentation and/or other materials provided with the distribution.   
--
-- THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" 
-- AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
-- IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE 
-- ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
-- FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
-- DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
-- OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
-- HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
-- LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
-- OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
-- SUCH DAMAGE.
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
