#ifndef	_EK9000_MBT_COMMON_H_
#define	_EK9000_MBT_COMMON_H_

/* This is the header file of Beckhoff EK9000 EPICS driver */
/* Because we use OSI interface, so we need EPICS 3.14 or above */
#ifdef _WIN32
#include <winsock2.h>
#pragma pack(push, 1)
#else

#ifdef vxWorks
#include <vxWorks.h>
#include <sysLib.h>
#include <sys/types.h>
#include <sys/times.h>
#include <sys/socket.h>
#include <sockLib.h>
#include <sys/ioctl.h>
#include <errno.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <arpa/inet.h>
#include <net/if.h>
#include <inetLib.h>
#include <ioLib.h>
#include <hostLib.h>
#include <selectLib.h>
#include <ctype.h>
#include <tickLib.h>
#include <unistd.h>
#include <rebootLib.h>
#else
/* Unix settings */
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <sys/ioctl.h>
#include <sys/fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <arpa/inet.h>
#include <netdb.h>
#ifdef SOLARIS
#include <sys/filio.h>
#endif
/* end of Unix settings */
#endif
#endif

#include "string.h"
#include "stdio.h"
#include "stdlib.h"

#include <epicsVersion.h>
#if EPICS_VERSION>=3 && EPICS_REVISION>=14
#include <epicsExport.h>
#include <alarm.h>
#include <dbDefs.h>
#include <dbAccess.h>
#include <recSup.h>
#include <recGbl.h>
#include <devSup.h>
#include <drvSup.h>
#include <link.h>
#include <ellLib.h>
#include <errlog.h>
#include <special.h>
#include <epicsTime.h>
#include <epicsMutex.h>
#include <epicsMessageQueue.h>
#include <epicsThread.h>
#else
#error "We need EPICS 3.14 or above to support OSI calls!"
#endif

#include "ModBusTCPClnt.h"


#define	EK9000_MBT_DRV_VER_STRING	"EK9000_MBT driver version 0.1.0"
/******************************************************************************************/
/*********************         define general data type         ***************************/
/* we don't use epicsType because under vxWorks, char is unsigned by default              */
/******************************************************************************************/
#ifndef	vxWorks	/* because vxWorks already defined them */
typedef unsigned char		UINT8;
typedef unsigned short int	UINT16;
typedef unsigned int		UINT32;
#endif

typedef signed char		SINT8;
typedef signed short int	SINT16;
typedef signed int		SINT32;
typedef float			FLOAT32;
typedef double			DOUBLE64;
/******************************************************************************************/

/******************************************************************************************/
/* We have an opThread running for each BK9000/EK9000 coupler,                                   */
/* it picks up request from msgQ and line them up in link list and execute it             */
/* it also keep talking to coupler to avoid link drop                                     */
/******************************************************************************************/
#define OPTHREAD_PRIORITY       (50)		/* opthread Priority, make it a little lower than scan task to finish all request once */
#define OPTHREAD_STACK          (0x20000)	/* opthread Stack Size */
#define OPTHREAD_MSGQ_CAPACITY	(200)		/* This means we can support 200 signal records per coupler */
#define	OPTHREAD_MSGQ_TMOUT	(0.5)		/* Even no request, we timeout then check link to avoid link drop */
#define	OPTHREAD_RECON_INTVL	(30)		/* If coupler not ready, we try to reconnect every 30 seconds */
/******************************************************************************************/

/******************************************************************************************/
/*********************       EPICS device support return        ***************************/
/******************************************************************************************/
#define CONVERT			(0)
#define NO_CONVERT		(2)
#define MAX_CA_STRING_SIZE      (40)
/******************************************************************************************/

/******************************************************************************************/
/* From Beckhoff EK9000 manual it supports up to 255 terminals                            */
/******************************************************************************************/
#define	MAX_NUM_OF_BUSTERM	255	/* The max number of bus terminals per coupler */

/* For EK9000 the stated max image size is for combined I/O, putting that here for each image */
#define	MAX_WORDS_OF_INPIMG	720	/* The max number of word of combined I/O image */
#define MAX_WORDS_OF_OUTIMG	720    /* The max number of word of combined I/O image */

/* Below we define some special offset of memory based registers */
#define	INPUT_IMG_BASE		0
#define	OUTPUT_IMG_BASE		0x800
#define	COUPLER_MREG_START	0x1000

#define	COUPLER_ID_MREG		0x1000	/* Totally 7 words 0x1000 ~ 0x1006 */
#define	COUPLER_ID_SIZE		7		/* words */

/* Below two registers are used to access table/register in coupler, has nothing to do with PLC */
#define	PLC_READ_INTERFACE	0x100A
#define	PLC_WRITE_INTERFACE	0x110A

/* We can read back the image size to verify our mapping */
/* NB: EK9000 reports complex I/O size in bytes, NOT bits as the BK9000 does! */
#define	COMPLEX_OUT_IMG_BYTES_MREG	0x1010
#define	COMPLEX_IN_IMG_BYTES_MREG	0x1011
#define	DIGITAL_OUT_IMG_BITS_MREG	0x1012
#define	DIGITAL_IN_IMG_BITS_MREG	0x1013
/******************************************************************************************/

/* Beckhoff Diagnostic Sub-Functions */
#define	BT_DIAG_SUB_FUNC_ECHO		0x0000	/* Echos write data */
#define	BT_DIAG_SUB_FUNC_RESET		0x0001	/* Reset and self-test Coupler, clear diag counters */
#define	BT_DIAG_SUB_FUNC_CLR_CTRS	0x000A	/* Clear all diagnostic counters */
#define	BT_DIAG_SUB_FUNC_RD_ANS_CTR	0x000B	/* Read Bus comm. answer counter */
#define	BT_DIAG_SUB_FUNC_RD_ANS_ERR	0x000D	/* Read Answer error counter */
#define	BT_DIAG_SUB_FUNC_RD_SLV_ANS	0x000E	/* Read Slave answer counter */
#define	BT_DIAG_SUB_FUNC_RD_UNANS	0x000F	/* Read Slave unanswered telegram counter */
#define	BT_DIAG_SUB_FUNC_RD_SLV_ERR	0x0010	/* Read Slave error answer counter */

/******************************************************************************************/
/* We define the default time out for ModBusTCP transaction and when we issue reset       */
/******************************************************************************************/
#define	DFT_MBT_TOUT	3	/* 3 seconds */
#define	N_EXC_TO_RST	20	/* How many exception PDU will cause us to reset coupler */
/******************************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif  /* __cplusplus */

/******************************************************************************************/
/* Below we defined all currently existing terminals,                                     */
/* you might add some new entries once Beckhoff releases some new terminals               */
/* The reason we use number to indicate bus terminal type is EK9000 table 9 does same     */
/******************************************************************************************/
typedef enum EC_BUSTERM_TYPE
{
	BT_TYPE_EK9000 = 9000,	/* This is coupler */
	BT_TYPE_EL9011 = 9011,	/* This is terminator */
        BT_TYPE_EK1110 = 1110,	/* This is extender */
	BT_TYPE_EL9080 = 9080,	/* Blank/separator */
	BT_TYPE_EL9505 = 9505,	/* 5V Power Supply Module */
	BT_TYPE_EL9508 = 9508,	/* 8V Power Supply Module */
	BT_TYPE_EL9510 = 9510,	/* 10V Power Supply Module */
	BT_TYPE_EL9512 = 9512,	/* 12V Power Supply Module */
	BT_TYPE_EL9515 = 9515,	/* 15V Power Supply Module */

	BT_TYPE_EL5042 = 5042,  /* 2-channel BiSS-C interface, 5V or 9V supply */

}	EC_E_BUSTERM_TYPE;

/******************************************************************************************/
/* We try to describe bus terminal here, some info maybe overkill                         */
/* We can't use input/output to describe terminal because some terminals have both        */
/* We don't use analog/digital to describe terminal because complex is more accurate      */
/* complex_in_bytes is NOT always same as complex_out_bytes for EtherCAT terminals        */
/* This definition is for image mapping only, EK9000_BTDef.h defines terminals            */
/******************************************************************************************/
typedef	struct EC_BUSTERM_IMG_DEF
{
	char			busterm_string[8];	/* String name of bus terminal, must be 6 characters */
	EC_E_BUSTERM_TYPE	busterm_type;		/* number name of bus terminal */
	UINT32			term_reg_exist;		/* 1: This terminal has registers, for coupler, this is not used, stay 0 */
	UINT16			complex_in_words;	/* how many words in complex input processing image */
	UINT16			complex_out_words;	/* how many words in complex output processing image */
	UINT16			digital_in_bits;	/* how many bits in digital input processing image */
	UINT16			digital_out_bits;	/* how many bits in digital output processing image */
}	EC_BUSTERM_IMG_DEF;

/******************************************************************************************/
/* We defined all possible operations, and default function only handle these             */
/* CIMG means complex image, all offset is word, DIMG is digital image and bit based      */
/******************************************************************************************/
typedef enum    EC_BUSTERM_OPTYPE
{
	BT_OPTYPE_READ_INPUT_CIMG,
	BT_OPTYPE_READ_OUTPUT_CIMG,
	BT_OPTYPE_WRITE_OUTPUT_CIMG,
	BT_OPTYPE_READ_INPUT_DIMG,
	BT_OPTYPE_READ_OUTPUT_DIMG,
	BT_OPTYPE_WRITE_OUTPUT_DIMG,
	BT_OPTYPE_READ_CPLR_MREG,
	BT_OPTYPE_WRITE_CPLR_MREG,
	BT_OPTYPE_READ_CPLR_REG,
	BT_OPTYPE_WRITE_CPLR_REG,
	BT_OPTYPE_CPLR_DIAG,
	BT_OPTYPE_READ_TERM_REG,
	BT_OPTYPE_WRITE_TERM_REG
}	EC_E_BUSTERM_OPTYPE;
/******************************************************************************************/

/******************************************************************************************/
/*********************       Record type we support             ***************************/
/******************************************************************************************/
typedef	enum	EPICS_RTYPE
{
	EPICS_RTYPE_NONE,
	EPICS_RTYPE_AI,
	EPICS_RTYPE_AO,
	EPICS_RTYPE_BI,
	EPICS_RTYPE_BO,
	EPICS_RTYPE_LI,
	EPICS_RTYPE_LO,
	EPICS_RTYPE_MBBI,
	EPICS_RTYPE_MBBO,
	EPICS_RTYPE_MBBID,
	EPICS_RTYPE_MBBOD,
	EPICS_RTYPE_SI,
	EPICS_RTYPE_SO,
	EPICS_RTYPE_WF
}	E_EPICS_RTYPE;
/******************************************************************************************/

/******************************************************************************************/
/* Data type of signal, it only means scalar value. For more complicated case             */
/* user should have special function to process it                                        */
/******************************************************************************************/
typedef	enum	DATA_TYPE
{
	DTYP_UINT8,
	DTYP_UINT16,
	DTYP_UINT32,
	DTYP_SINT8,
	DTYP_SINT16,
	DTYP_SINT32,
	DTYP_FLOAT32,
	DTYP_DOUBLE64
}	E_DATA_TYPE;
/******************************************************************************************/

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
/*	BT_OPTYPE_CPLR_DIAG                                                               */
/* sub-function, 0, 0                                                                     */
/*	BT_OPTYPE_READ_TERM_REG/BT_OPTYPE_WRITE_TERM_REG:                                 */
/* C/S word offset in complex input image, C/S word offset in complex output image, reg   */ 
/******************************************************************************************/
typedef	struct BUSTERM_SIG_PREDEF
{/* We use this one to define signals */
	char				busterm_string[8];		/* String name of bus terminal, must be 6 characters */
	EC_E_BUSTERM_TYPE	busterm_type;			/* number name of bus terminal */
	char				function[MAX_CA_STRING_SIZE];	/* Function name, should be the third part of INP/OUT field */
	EC_E_BUSTERM_OPTYPE	busterm_optype;			/* The operation type, one of above list */
	E_EPICS_RTYPE		epics_rtype;			/* EPICS record type to use this signal, EPICS_RTYP_NONE means not for EPICS or waive check */
	E_DATA_TYPE		data_type;
	UINT32			arg1;
	UINT32			arg2;
	UINT32			arg3;
}	BUSTERM_SIG_PREDEF;

typedef	struct BUSTERM_SIG_DEF
{/* We use this one to really handle signals */
	char				busterm_string[8];		/* String name of bus terminal, must be 6 characters */
	EC_E_BUSTERM_TYPE	busterm_type;			/* number name of bus terminal */
	char				function[MAX_CA_STRING_SIZE];	/* Function name, should be the third part of INP/OUT field */
	EC_E_BUSTERM_OPTYPE	busterm_optype;			/* The operation type, one of above list */
	E_EPICS_RTYPE		epics_rtype;			/* EPICS record type to use this signal, EPICS_RTYP_NONE means not for EPICS or waive check */
	E_DATA_TYPE		data_type;
	union ARGS
	{
		struct CIMG_rw
		{
			UINT32	woffset;
			UINT32	nwords;
			UINT32	effbits;
		}cimg_rw;
		struct DIMG_rw
		{
			UINT32	boffset;
			UINT32	nbits;
			UINT32	not_used1;
		}dimg_rw;
		struct CPLR_MREG
		{
			UINT32	woffset;
			UINT32	reset;
			UINT32	not_used1;
		}cplr_mreg;
		struct CPLR_REG
		{
			UINT32	table;
			UINT32	reg;
			UINT32	reset;
		}cplr_reg;
		struct CPLR_DIAG
		{
			UINT32	subfunc;
			UINT32	not_used1;
			UINT32	not_used2;
		}cplr_diag;
		struct TERM_REG
		{
			UINT32	Rwoffset;
			UINT32	Wwoffset;
			UINT32	reg;
		}term_reg;

	}	args;
}	BUSTERM_SIG_DEF;
/******************************************************************************************/

/******************************************************************************************/
/* Below we define the run-time control structures                                        */
/******************************************************************************************/
typedef ELLLIST EK9000_INIT_LIST;
typedef ELLLIST EK9000_COUPLER_LIST;
typedef ELLLIST EK9000_SIGNAL_LIST;
typedef	ELLLIST	EK9000_SIGPTR_LIST;

typedef	struct EK9000_INIT
{
	ELLNODE			node;	/* Linked List Node */
	
	BUSTERM_SIG_DEF		* pbusterm_sig_def;
	UINT16			init_value;
}	EK9000_INIT;

typedef	struct INSTALLED_BUSTERM
{
	EC_BUSTERM_IMG_DEF	* pbusterm_img_def;
	EK9000_INIT_LIST	init_list;
	UINT32			complex_in_wordoffset;	/* complex input word offset of this module in input processing image */
	UINT32			complex_out_wordoffset;	/* complex output word offset of this module in output processing image */
	UINT32			digital_in_bitoffset;	/* digital input bit offset of this module in input processing image */
	UINT32			digital_out_bitoffset;	/* digital output bit offset of this module in output processing image */
}	INSTALLED_BUSTERM;

/* EK9000 EtherCAT coupler information */
typedef struct EK9000_COUPLER
{
	ELLNODE			node;	/* Linked List Node */

	ModBusTCP_Link		mbt_link;	/* The link to device via ModBusTCP protocol */

	epicsMutexId		mutex_lock;

	INSTALLED_BUSTERM	installedBusTerm[MAX_NUM_OF_BUSTERM+2];	/* +2 means include coupler 9000 and terminator 9011 */
	UINT32			terminated;		/* Is EL9011 already installed */

	/* Below 4 we calculate first, then we read back from memory imaged register of coupler to verify*/
	UINT16			complex_out_bytes;
	UINT16			complex_in_bytes;
	UINT16			digital_out_bits;
	UINT16			digital_in_bits;
	/* Below 2 we calculate by software */
	UINT32			total_out_words; /* This must be equal to (complex_out_bytes*8 + digital_out_bits +15)/16 */
	UINT32			total_in_words; /* This must be equal to (complex_in_bytes*8 + digital_in_bits +15)/16 */

	UINT32			couplerReady;	/* This indicate if coupler really usable */
	UINT32			needReset;	/* This is used to bundle all register ops that need reset, so we only reset once to apply all */

	epicsTimeStamp		time_lost_conn;	/* Last time we found we lost connection */
	epicsTimeStamp		time_set_conn;	/* Last time we successfully set up connection */
	epicsTimeStamp		time_last_try;	/* Last time we try to connect */

	char				couplerID[COUPLER_ID_SIZE*2+2];
	
	/* We use hardcoded size, this wastes a little bit memory, but no malloc and error check */
        /* Note this might need to be modified if there is ever the need for byte access */
	UINT16			outputImage[MAX_WORDS_OF_OUTIMG];
	UINT16			inputImage[MAX_WORDS_OF_INPIMG];

	EK9000_SIGPTR_LIST	sigptr_list;	/* All signals' pointer link list */

	epicsMessageQueueId	msgQ_id;	/* Through this message queue, record processing sends request to opthread */

	epicsThreadId		opthread_id;	/* operation thread ID for this EK9000 */
	char				opthread_name[MAX_CA_STRING_SIZE];

}	EK9000_COUPLER;

/* EK9000 device suport record information */
typedef	struct EK9000_DEVDATA
{
	dbCommon       		* precord;
				
	EK9000_COUPLER		* pcoupler;
	UINT16			slot;							
	BUSTERM_SIG_DEF		* pbusterm_sig_def;

	UINT16			value;	/* the value of input or ouput, 16 bits fits most of signals */

	int				op_done;
	UINT32			err_code;	/* high 16 will be local error, low 16 will copy MBTC err code, 0 means no error */

}	EK9000_DEVDATA;

/* This is the one dpvt ponits to and send in message queue */
typedef	SINT32	(* EK9000_FPTR)(EK9000_DEVDATA * pdevdata, void * pextra_arg);
typedef	struct EK9000_SIGNAL
{
	ELLNODE			node;	/* Linked list node */

	EK9000_FPTR		process_fptr;

	EK9000_DEVDATA	* pdevdata;
											
	void			* pextra_arg;
}	EK9000_SIGNAL;

/* This is the one that in the coupler's SIGPTR list */
typedef	struct EK9000_SIGPTR
{
	ELLNODE			node;	/* Linked list node */
	EK9000_SIGNAL		* psignal;
}	EK9000_SIGPTR;

/* This is for device support to malloc memory easier */
typedef	struct EK9000_DEVSUPDATA
{
	EK9000_SIGPTR	sigptr;
	EK9000_SIGNAL	signal;
	EK9000_DEVDATA	devdata;
}	EK9000_DEVSUPDATA;
/******************************************************************************************/
/* Define err_code here                                                                   */
/******************************************************************************************/
#define	ERR_CODE_NO_ERROR		0x00000000
#define	ERR_CODE_CPLR_NOT_READY		0x00010000
#define	ERR_CODE_SYNC_IMG_FAIL		0x00020000
#define	ERR_CODE_PROC_NOT_SUPT		0x00030000
#define ERR_CODE_OUTINIT_NOT_SUPT       0x00040000
/******************************************************************************************/
/* Below we define the function prototype                                                 */
/******************************************************************************************/

/******************************************************************************************/
/* Below functions are from EK9000_MBT_Ext.c                                              */
/******************************************************************************************/

/******************************************************************************************/
/* By default, return value 0 means OK, return value -1 means something wrong             */
/* In this file, all offsets for Mreg are absolute value                                  */
/* All offsets for IO image is relative                                                   */
/* We don't check any EK9000 related parameter range, caller should prepare it            */
/******************************************************************************************/

/***********************************************************************************************/
/*********************** Request PDU and Response PDU definition *******************************/
/* We use xXYYYoffset to name offset, x is b(bit) or w(word), X is R(read) or W(Write)         */
/* YYY is the combination of I(input image) O(output image) R(registers)                       */
/* Dworddata means 16 bits scalar value for Diag, and WByteData means byte array to write      */
/***********************************************************************************************/

/* This function uses MBT function 8 to reset EK9000 coupler */
/* We need this in EK9000 connection monitor task if too many exception PDUs */
int EK9000_MBT_Reset(ModBusTCP_Link mbt_link, unsigned int toutsec);

/* This function uses MBT function 8 to do echo to test connection */
/* We keep calling this in EK9000 connection monitor task to avoid link drop */
int EK9000_MBT_TestLink(ModBusTCP_Link mbt_link, unsigned int toutsec);

/* This function uses MBT function 8 to read diagnostic counters */
int EK9000_MBT_Diag(ModBusTCP_Link mbt_link, unsigned int toutsec);

/* This function uses MBT function 3 to read coupler id from image */
/* ID must be a pre-malloced buffer with size bytes */
int	EK9000_MBT_Read_Cplr_ID(ModBusTCP_Link mbt_link, char * ID, int size, unsigned int toutsec);

/* This function uses MBT function 3 to read image size info from image */
/* Then verify if the calculated size is correct */
int	EK9000_MBT_Verify_Image_Size(ModBusTCP_Link mbt_link, unsigned short int cal_complex_out_bytes, unsigned short int cal_complex_in_bytes,
								 unsigned short int cal_digital_out_bits, unsigned short int cal_digital_in_bits, unsigned int toutsec);

/* This function uses MBT function 3 to read memory image based register of EK9000 coupler */
/* Now it reads only one word, user might use it as template to build function to read more */
/* Because we read only one register(word), we don't worry about oversize here */
int EK9000_MBT_Read_Cplr_MReg(ModBusTCP_Link mbt_link, unsigned short int wRRoffset, unsigned short int *pRWordData, unsigned int toutsec);

/* This function uses MBT function 16 to write memory image based register of EK9000 coupler */
/* Now it writes only one word, user might use it as template to build function to write more */
/* That is the reason we use function 16 instead of function 6 and use pWWordData to pass even single data */
/* Because we read only one register(word), we don't worry about oversize here */
int EK9000_MBT_Write_Cplr_MReg(ModBusTCP_Link mbt_link, unsigned short int wWRoffset, unsigned short int *pWWordData, unsigned int toutsec);

/* Below two functions use MBT function 3 and function 16 to operate the PLC register in EK9000 coupler */
/* This allow you read/write all table/register in coupler, we only operate one register(word) here  */
/* Caution: Don't get confused, I call PLC register here just because it's listed in manual like this way */
/* There is nothing to do with PLC. It is only for register access purpose */
/* Because it is accessing coupler register, so terminal number will be always 0 */
/* If you add terminal number as a parameter, you can use same technique to access terminal registers */
/* But because we have another faster way to access terminal register, so we define other functions */
int EK9000_MBT_Read_Cplr_Reg(ModBusTCP_Link mbt_link, unsigned char table, unsigned char reg, unsigned short int *pRWordData, unsigned int toutsec);
int EK9000_MBT_Write_Cplr_Reg(ModBusTCP_Link mbt_link, unsigned char table, unsigned char reg, unsigned short int *pWWordData, unsigned int toutsec);

/* Below two functions use MBT function 3 and function 16 to operate the terminal registers */
/* Here we don't use PLC register in coupler, we use C/S byte mapped in image */
/* To be general, we use two parameters to specify the offset of C/S in input and output image */
/* But so far as we know, they should be always same */
/* This allow you read/write all registers in terminal, we only operate one register(word) here  */
int EK9000_MBT_Read_Term_Reg(ModBusTCP_Link mbt_link, unsigned short int wRIoffset, unsigned short int wWOoffset, unsigned char reg, unsigned short int *pRWordData, unsigned int toutsec);
int EK9000_MBT_Write_Term_Reg(ModBusTCP_Link mbt_link, unsigned short int wRIoffset, unsigned short int wWOoffset, unsigned char reg, unsigned short int *pRWordData, unsigned int toutsec);

/* We need this function to init whole local image of output, it is based on function 3 */
int EK9000_MBT_Read_Output_Image(ModBusTCP_Link mbt_link, unsigned short int *pimage, unsigned short int wimg_size, unsigned int toutsec);

/* We try to combine all analong/digital signal operations based on memory image to one function 23 */
/* pinpimage and poutimage will be always point to the begin of the image */
/* But if nothing to write, we will use function 4 */
/* If nothing to read, we will use function 16 */
int EK9000_MBT_Sync_Both_Image(ModBusTCP_Link mbt_link, unsigned short int wRIoffset, unsigned short int RWordCount, unsigned short int *pinpimage,
				unsigned short int wWOoffset, unsigned short int WWordCount, unsigned short int *poutimage,unsigned int toutsec);

/******************************************************************************************/
/* Below functions are from drvEK9000_MBT.c                                               */
/******************************************************************************************/

/* This function returns the pointer to the coupler with name */
EK9000_COUPLER	* EK9000_Get_Coupler_By_Name( const char * cplrname );

/* This must be called in st.cmd first before any operation to the coupler */
/* name must be unique, and ipaddr is not necessary to be unique */
/* This function can be only called in st.cmd */
/* init_string will be "signame1=1234,signame2=0x2345" */
int	EK9000_Coupler_Add( char * cplrname, char * ipaddr, char * init_string);

/* This function add a bus terminal to an existing coupler */
/* init_string will be "signame1=1234,signame2=0x2345" */
int	EK9000_Terminal_Add( char * cplrname, UINT16 slot, char * btname, char * init_string);

/* This function will be called by all device support */
/* The memory for EK9000_SIGNAL will be malloced inside */
int	EK9000_Signal_Init(dbCommon * precord, E_EPICS_RTYPE epics_rtype, char * ioString, EC_E_BUSTERM_TYPE bttype, EK9000_FPTR process_fptr, void * pextra_arg);

/* This is the default process function, it deals with coupler reg/Mreg and terminal reg */
/* For image based operation, it supports single bit op and single word op only */
/* For the op needs more words or bits, it will put ERR_CODE_PROC_NOT_SUPT, you need your own function */
int	EK9000_Dft_ProcFunc(EK9000_DEVDATA * pdevdata, void * pextra_arg);

/* This is the function will be called in device support init function to init output record */
int	EK9000_Dft_OutInit(EK9000_SIGNAL * psignal);

#ifdef __cplusplus
}
#endif  /* __cplusplus */

#endif
