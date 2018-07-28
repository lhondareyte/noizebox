/*
 * Copyright (c)2017, Luc Hondareyte
 * 
 * All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 * 
 *     * Redistributions of source code must retain the above copyright notice,
 *       this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright notice,
 *       this list of conditions and the following disclaimer in the documentation
 *       and/or other materials provided with the distribution.
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

/*
 *  Channel messages
 */

#define MIDI_UNKNOW_MSG		0xFF
#define MIDI_NOTOFF_MSG		0x80
#define MIDI_NOTEON_MSG		0x90		
#define MIDI_POLYAF_MSG		0xA0		
#define MIDI_CTRLCHG_MSG	0xB0
#define MIDI_MODECHG_MSG	0xB0
#define MIDI_PROGCH_MSG		0xC0
#define MIDI_CHANAF_MSG		0xD0
#define MIDI_PITCHB_MSG		0xE0

/*
 *  System common messages
 */

#define MIDI_SYSEX_MSG		0xF0
#define MIDI_QUARTER_MSG	0xF1
#define MIDI_SONGPOS_MSG	0xF2
#define MIDI_SONGSEL_MSG	0xF3

/*
 *  One-byte System common messages
 */

#define MIDI_UNDEF1_MSG		0xF4
#define MIDI_UNDEF2_MSG		0xF5
#define MIDI_TUNERQ_MSG		0xF6
#define MIDI_EOX_MSG		0xF7

/*
 *  System Real Time messages 
 */

#define MIDI_TIMECLK_MSG	0xF8
#define MIDI_UNDEF3_MSG		0xF9
#define MIDI_START_MSG		0xFA
#define MIDI_CONTINUE_MSG	0xFB
#define MIDI_STOP_MSG		0xFC
#define MIDI_UNDEF4_MSG		0xFD
#define MIDI_ACTIVE_MSG		0xFE
#define MIDI_RESET_MSG		0xFF

/*
 *  MIDI Controller Numbers
 */

#define MIDI_BANKSEL_CTRL	0x00
#define MIDI_MODWHEEL_CTRL	0x01
#define MIDI_BREATH_CTRL	0x02
#define MIDI_FOOT_CTRL		0x04
#define MIDI_PORTMTO_CTRL	0x05
#define MIDI_DATAENTRY_CTRL	0x06
#define MIDI_VOLUME_CTRL	0x07
#define MIDI_BALANCE_CTRL	0x08
#define MIDI_PAN_CTRL		0x0A
#define MIDI_EXPRESSION_CTRL	0x0B
#define MIDI_EFFECT1_CTRL	0x0C
#define MIDI_EFFECT2_CTRL	0x0D
#define MIDI_GENPURP1_CTRL	0x10
#define MIDI_GENPURP2_CTRL	0x11
#define MIDI_GENPURP3_CTRL	0x12
#define MIDI_GENPURP4_CTRL	0x13

volatile uint8_t buffer;	// Buffer de reception MIDI
volatile uint8_t status;
volatile uint8_t channel;	// Canal MIDI du message recu
volatile uint8_t next;		// Prochaine valeur attendue
volatile int     k;	
volatile uint8_t data1;	
volatile uint8_t data2;
volatile uint8_t ready;		// Message prêt ou non 
volatile uint8_t rsbuff;	// Buffer pour Running status
uint8_t activechannel;		// Canal MIDI actif


/*
 * SysEx message buffer
 */

#if defined (MIDI_MANUFACTURER_ID)
#endif

/*
 * Pseudo fonctions 
 */
 
#define	MIDI_SetRcvChannel(c)	activechannel=c
#define	MIDI_Ack()		ready=MIDI_FALSE;data1=data2=status=MIDI_UNKNOW_MSG
#define	MIDI_Init(c)		status=data1=data2=rsbuff=MIDI_UNKNOW_MSG;activechannel=c;
#define	MIDI_MsgIsReady()	ready==MIDI_TRUE

#endif /* __NOIZEBOX_MIDI_H__ */
