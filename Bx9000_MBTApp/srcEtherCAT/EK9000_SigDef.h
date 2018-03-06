//////////////////////////////////////////////////////////////////////////////
// This file is part of 'Bx9000_MBT'.
// It is subject to the license terms in the LICENSE.txt file found in the 
// top-level directory of this distribution and at: 
//    https://confluence.slac.stanford.edu/display/ppareg/LICENSE.html. 
// No part of 'Bx9000_MBT', including this file, 
// may be copied, modified, propagated, or distributed except according to 
// the terms contained in the LICENSE.txt file.
//////////////////////////////////////////////////////////////////////////////
#ifndef	_EK9000_SIGDEF_H_
#define	_EK9000_SIGDEF_H_

#include "EK9000_MBT_Common.h"

#ifdef __cplusplus
extern "C" {
#endif  /* __cplusplus */

#if	0	/* Must be 0, just for reference here */
/******************************************************************************************/
/* The signal definition, the busterm_string/type and function combination must be unique */
/* The definition of arg1 -> arg3 is different between different operation                */
/*	BT_OPTYPE_READ_INPUT_CIMG/BT_OPTYPE_READ_OUTPUT_CIMG/BT_OPTYPE_WRITE_OUTPUT_CIMG: */
/* word offset to start of terminal, number of words, effective bits                      */
/*	BT_OPTYPE_READ_INPUT_DIMG/BT_OPTYPE_READ_OUTPUT_DIMG/BT_OPTYPE_WRITE_OUTPUT_DIMG: */
/* bit offset to start of terminal, number of bits, 0                                     */
/*	BT_OPTYPE_READ_CPLR_MREG/BT_OPTYPE_WRITE_CPLR_MREG:                               */
/* word offset in image above COUPLER_MREG_START, Reset to apply, 0?                      */
/*	BT_OPTYPE_READ_CPLR_REG/BT_OPTYPE_WRITE_CPLR_REG:                                 */
/* table, register, Reset to apply?                                                       */
/*      BT_OPTYPE_CPLR_DIAG                                                               */
/* sub-function, 0, 0                                                                     */
/*	BT_OPTYPE_READ_TERM_REG/BT_OPTYPE_WRITE_TERM_REG:                                 */
/* C/S word offset in complex input image, C/S word offset in complex output image, reg   */ 
/******************************************************************************************/
typedef	struct BUSTERM_SIG_PREDEF
{/* We use this one to define signals */
	UINT8				busterm_string[8];		/* String name of bus terminal, must be 6 characters */
	E_BUSTERM_TYPE			busterm_type;			/* number name of bus terminal */
	UINT8				function[MAX_CA_STRING_SIZE];	/* Function name, should be the third part of INP/OUT field */
	E_BUSTERM_OPTYPE		busterm_optype;			/* The operation type, one of above list */
	E_EPICS_RTYPE			epics_rtype;			/* EPICS record type to use this signal, EPICS_RTYP_NONE means not for EPICS or waive check */
	E_DATA_TYPE			data_type;
	UINT32				arg1;
	UINT32				arg2;
	UINT32				arg3;
}	BUSTERM_SIG_PREDEF;
#endif

/* Most of signals could be processed by default function, but if you have anything longer than 32 bits, you might need your own function */
static	BUSTERM_SIG_PREDEF	busterm_sig_predef[]= {
#ifdef DBR_INT64
	{"EL5042", BT_TYPE_EL5042, "R_LONG_IN_1",        BT_OPTYPE_READ_INPUT_CIMG,      EPICS_RTYPE_LI, DTYP_UINT64,    1, 1, 64},
	
	{"EL5042", BT_TYPE_EL5042, "R_LONG_IN_2",        BT_OPTYPE_READ_INPUT_CIMG,      EPICS_RTYPE_LI, DTYP_UINT64,    9, 1, 64},
#else
	{"EL5042", BT_TYPE_EL5042, "R_LONG_IN_1",        BT_OPTYPE_READ_INPUT_CIMG,      EPICS_RTYPE_LI, DTYP_UINT32,    1, 1, 32},
	
	{"EL5042", BT_TYPE_EL5042, "R_LONG_IN_2",        BT_OPTYPE_READ_INPUT_CIMG,      EPICS_RTYPE_LI, DTYP_UINT32,    5, 1, 32},
#endif

	{"EL7041", BT_TYPE_EL7041, "R_ANALOG_IN_1",        BT_OPTYPE_READ_INPUT_CIMG,      EPICS_RTYPE_AI, DTYP_SINT32,    1, 1, 32},
	
	{"EL7041", BT_TYPE_EL7041, "R_ANALOG_OUT_1",       BT_OPTYPE_WRITE_INPUT_CIMG,      EPICS_RTYPE_AO, DTYP_SINT32,    1, 1, 32},


};

#define	N_BT_SIG_PREDEF	(sizeof(busterm_sig_predef)/sizeof(BUSTERM_SIG_PREDEF))

#ifdef __cplusplus
}
#endif  /* __cplusplus */

#endif
