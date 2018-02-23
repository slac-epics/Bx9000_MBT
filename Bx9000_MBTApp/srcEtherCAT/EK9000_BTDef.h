//////////////////////////////////////////////////////////////////////////////
// This file is part of 'Bx9000_MBT'.
// It is subject to the license terms in the LICENSE.txt file found in the 
// top-level directory of this distribution and at: 
//    https://confluence.slac.stanford.edu/display/ppareg/LICENSE.html. 
// No part of 'Bx9000_MBT', including this file, 
// may be copied, modified, propagated, or distributed except according to 
// the terms contained in the LICENSE.txt file.
//////////////////////////////////////////////////////////////////////////////
#ifndef	_EK9000_BTDEF_H_
#define	_EK9000_BTDEF_H_

#include "EK9000_MBT_Common.h"

#ifdef __cplusplus
extern "C" {
#endif  /* __cplusplus */

#if	0	/* Must be 0, just for reference here */
typedef	struct EC_BUSTERM_IMG_DEF
{
	UINT8			busterm_string[8];	/* String name of ECAT bus terminal, must be 6 characters */
	EC_E_BUSTERM_TYPE	busterm_type;		/* number name of ECAT bus terminal */
	UINT16			complex_in_words;	/* how many words in complex input processing image */
	UINT16			complex_out_words;	/* how many words in complex output processing image */
	UINT16			digital_in_bits;	/* how many bits in digital input processing image */
	UINT16			digital_out_bits;	/* how many bits in digital output processing image */
}	EC_BUSTERM_IMG_DEF;
#endif

static EC_BUSTERM_IMG_DEF	ec_busterm_img_def[]={
	{"EK9000",		BT_TYPE_EK9000,	0,	0,	0,	0},

	{"EL5042",		BT_TYPE_EL5042,	10,	0,	0,	0}, /* Biss-C module */
	{"EL7041",		BT_TYPE_EL7041,	4,	4,	0,	0}, /* 50V 5A stepper module w/ inc. encoder */

	{"EL9505",		BT_TYPE_EL9505,	0,	0,	0,	0}, /* Power Supply Module */
	{"EL9080",		BT_TYPE_EL9080,	0,	0,	0,	0}, /* Separation terminal, just a blank */
	{"EL9011",		BT_TYPE_EL9011,	0,	0,	0,	0} /* This must be last one */
};

#define	EC_N_BT_IMG_DEF	(sizeof(ec_busterm_img_def)/sizeof(EC_BUSTERM_IMG_DEF))

#ifdef __cplusplus
}
#endif  /* __cplusplus */

#endif
