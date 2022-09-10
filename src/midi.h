/*
 * Copyright (c)2017-2021, Luc Hondareyte
 * 
 * All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 
 *  * Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 *  * Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER
 * OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 */
/*
 *  Librairie MIDI pour AVR
 *  $Id: midi.h,v 1.17 2012/04/15 11:27:16 luc Exp luc $
 *
 *  (c)2011 - Luc HONDAREYTE 
 *  All rights reserved
 */

 
#ifndef __NOIZEBOX_MIDI_H__
 #define __NOIZEBOX_MIDI_H__
#include <errno.h>
#include <sys/types.h>

#define MIDI_FALSE          0
#define MIDI_TRUE	    1

#ifndef bit_is_set
 #define bit_is_set(var,pos) ((var) & (1<<(pos)))
#endif

#define MIDI_DATA1		1
#define MIDI_DATA2		2

  /* channel messages */
#define  MIDI_UNKNOWN           0x00

#define  MIDI_NOTE_OFF          0x80
#define  MIDI_NOTE_ON           0x90
#define  MIDI_KEY_PRESSURE      0xa0
#define  MIDI_CONTROL_CHANGE    0xb0
#define  MIDI_PROGRAM_CHANGE    0xc0
#define  MIDI_CHANNEL_PRESSURE  0xd0
#define  MIDI_PITCH_BEND        0xe0
  /* system exclusive */
#define  MIDI_SYSEX             0xf0
  /* system common - never in midi files */
#define  MIDI_TIME_CODE         0xf1
#define  MIDI_SONG_POSITION     0xf2
#define  MIDI_SONG_SELECT       0xf3
#define  MIDI_TUNE_REQUEST      0xf6
#define  MIDI_EOX               0xf7
  /* system real-time - never in midi files */
#define  MIDI_SYNC              0xf8
#define  MIDI_TICK              0xf9
#define  MIDI_START             0xfa
#define  MIDI_CONTINUE          0xfb
#define  MIDI_STOP              0xfc
#define  MIDI_ACTIVE_SENSING    0xfe
#define  MIDI_SYSTEM_RESET      0xff
  /* MIDI Controller Numbers */
#define  MIDI_BANKSEL_CTRL	0x00
#define  MIDI_MODWHEEL_CTRL	0x01
#define  MIDI_BREATH_CTRL	0x02
#define  MIDI_FOOT_CTRL		0x04
#define  MIDI_PORTMTO_CTRL	0x05
#define  MIDI_DATAENTRY_CTRL	0x06
#define  MIDI_VOLUME_CTRL	0x07
#define  MIDI_BALANCE_CTRL	0x08
#define  MIDI_PAN_CTRL		0x0A
#define  MIDI_EXPRESSION_CTRL	0x0B
#define  MIDI_EFFECT1_CTRL	0x0C
#define  MIDI_EFFECT2_CTRL	0x0D
#define  MIDI_GENPURP1_CTRL	0x10
#define  MIDI_GENPURP2_CTRL	0x11
#define  MIDI_GENPURP3_CTRL	0x12
#define  MIDI_GENPURP4_CTRL	0x13
#define  MIDI_BREATH_FINE_CTRL	0x22

#ifndef BREATH_MSB
#define BREATH_MSB 0x02
#endif
#ifndef BREATH_LSB
#define BREATH_LSB 0x22
#endif

/*
 * SysEx message buffer
 */

#if defined (MIDI_MANUFACTURER_ID)
#endif

/*
 * Pseudo fonctions 
 */
 
#define	MIDI_SetRcvChannel(c)	activechannel=c
#define	MIDI_Ack()		ready=MIDI_FALSE;data1=data2=status=MIDI_UNKNOWN
#define	MIDI_Init(c)		status=data1=data2=rsbuff=MIDI_UNKNOWN;activechannel=c;
#define	MIDI_MsgIsReady()	ready==MIDI_TRUE

#endif /* __NOIZEBOX_MIDI_H__ */
