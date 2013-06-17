#ifndef	_Bx9000_BTDEF_H_
#define	_Bx9000_BTDEF_H_

#include "Bx9000_MBT_Common.h"

#ifdef __cplusplus
extern "C" {
#endif  /* __cplusplus */

#if	0	/* Must be 0, just for reference here */
typedef	struct BUSTERM_IMG_DEF
{
	UINT8			busterm_string[8];	/* String name of bus terminal, must be 6 characters */
	E_BUSTERM_TYPE	busterm_type;		/* number name of bus terminal */
	UINT32          term_reg_exist;     /* 1: This terminal has registers, for coupler, this is not used, stay 0 */
	UINT16          term_r32_dft;       /* default value of feature register */
	UINT16			complex_in_words;	/* how many words in complex input processing image */
	UINT16			complex_out_words;	/* how many words in complex output processing image */
	UINT16			digital_in_bits;	/* how many bits in digital input processing image */
	UINT16			digital_out_bits;	/* how many bits in digital output processing image */
}	BUSTERM_IMG_DEF;
#endif

static BUSTERM_IMG_DEF	busterm_img_def[]={
	{"Bx9000",		BT_TYPE_Bx9000,	0,		0,		0,		0,		0,		0},
	{"KL1104",      BT_TYPE_KL1104, 0,      0,      0,      0,      4,      0},
	{"KL1124",      BT_TYPE_KL1124, 0,      0,      0,      0,      4,      0},
	{"KL1408",      BT_TYPE_KL1408, 0,      0,      0,      0,      8,      0},
	{"KL1488",      BT_TYPE_KL1488, 0,      0,      0,      0,      8,      0},
	{"KL2012",		BT_TYPE_KL2012,	0,		0,		0,		0,		0,		2},
	{"KL2114",		BT_TYPE_KL2114,	0,		0,		0,		0,		0,		4},
	{"KL2124",		BT_TYPE_KL2124,	0,		0,		0,		0,		0,		4},
	{"KL2408",      BT_TYPE_KL2408, 0,      0,      0,      0,      0,      8},
	{"KL2502",      BT_TYPE_KL2502, 1,      0x0004, 4,      4,      0,      0},
	{"KL2512",      BT_TYPE_KL2512, 1,      0x0004, 4,      4,      0,      0},
	{"KL2622",      BT_TYPE_KL2622, 0,      0,      0,      0,      0,      2},
	{"KL2872",      BT_TYPE_KL2872, 0,      0,      0,      0,      0, 		16},
	{"KL3064",      BT_TYPE_KL3064, 1,      0x1106, 8,      8,      0,      0},
	{"KL3102",		BT_TYPE_KL3102,	1,		0x0000,	4,		4,		0,		0},
/* per files Access from User Program-Register Overveiw.pdf and Access from User Program-Mapping.pdf; "Complete evaluation in Intel format with word alignment" --AC */
	{"KL3162",		BT_TYPE_KL3162,	1,		0x0180,	4,		4,		0,		0},
	{"KL3202",      BT_TYPE_KL3202, 1,      0x1006, 4,      4,      0,      0},
/* per KL320xen.pdf, top of p. 14 -- AC */
	{"KL3204",      BT_TYPE_KL3204, 1,      0x0106, 8,     	8,      0,      0},
	{"KL3222",      BT_TYPE_KL3222, 1,      0x00A4, 4,      4,      0,      0},
	{"KL3312",      BT_TYPE_KL3312, 1,      0x1006, 4,      4,      0,      0},
	{"KL3314",      BT_TYPE_KL3314, 1,      0x1006, 8,      8,      0,      0},
	{"KL3408",      BT_TYPE_KL3408, 1,      0x1106, 16,     16,     0,      0},
	{"KL3468",      BT_TYPE_KL3468, 1,      0x1106, 16,     16,     0,      0},
	{"KL4002",		BT_TYPE_KL4002,	1,		0x0006,	4,		4,		0,		0},
	{"KL4132",		BT_TYPE_KL4132,	1,		0x0006,	4,		4,		0,		0},
	{"KM1004",      BT_TYPE_KM1004, 0,      0,      0,      0,      32,     0},
	{"KM2004",      BT_TYPE_KM2004, 0,      0,      0,      0,      0, 		32},
	{"KL9505",		BT_TYPE_KL9505,	0,	0,	0,	0,	0,	0},	/* Power Supply Module */
	{"KL9020",		BT_TYPE_KL9020,	0,	0,	0,	0,	0,	0},
	{"KL9080",		BT_TYPE_KL9080,	0,	0,	0,	0,	0,	0}, /* Separation terminal, just a blank */
	{"KL9010",		BT_TYPE_KL9010,	0,	0,	0,	0,	0,	0}	/* This must be last one */
};

#define	N_BT_IMG_DEF	(sizeof(busterm_img_def)/sizeof(BUSTERM_IMG_DEF))

#ifdef __cplusplus
}
#endif  /* __cplusplus */

#endif
