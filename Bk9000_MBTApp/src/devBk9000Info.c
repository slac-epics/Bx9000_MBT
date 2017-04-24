/***************************************************************************
 *   File:		devBk9000Info.c
 *   Author:		Sheng Peng
 *   Institution:	Oak Ridge National Laboratory / SNS Project
 *   Date:		12/2004
 *   Version:		1.1
 *
 *   EPICS device layer support for Beckhoff Bk9000 Coupler static info
 *
 *   Hardware addressing:	
 *   @cplrname:slot:func
 *   For most of records, slot will be 0, so far only for function "BTTYPE",
 *   we need slot not 0.
 ****************************************************************************/

/*	Include header files	*/
#include <Bk9000_MBT_Common.h>
#include <longinRecord.h>
#include <mbbiRecord.h>
#include <stringinRecord.h>

/* The infomation we try to get here either static after initialization or atomic */
/* So we don't need semaphore protection */
/* Some info matters coupler ready, some info doesn't */
extern  SINT32  Bk9000_DEV_DEBUG;

/*	define function IDs, some of functions need slot	*/
typedef enum {
	Bk9000Info_LI_COWCNT,
	Bk9000Info_LI_CIWCNT,
	Bk9000Info_LI_DOBCNT,
	Bk9000Info_LI_DIBCNT,
	Bk9000Info_LI_BTTYPE,	/* slot could be vary */
	Bk9000Info_LI_NOFCONN,
	Bk9000Info_LI_NOFEXCP,
	Bk9000Info_LI_NOFPKTS,
	Bk9000Info_LI_NOFSIGS,
	Bk9000Info_LI_OPTHRDID,
	Bk9000Info_MBBI_CPLRRDY,
	Bk9000Info_MBBI_LINKSTAT,
	Bk9000Info_SI_CPLRNAME,
	Bk9000Info_SI_CPLRIP,
	Bk9000Info_SI_TLSTCONN,
	Bk9000Info_SI_TSETCONN,
	Bk9000Info_SI_TLASTTRY,
	Bk9000Info_SI_CPLRID,
	Bk9000Info_SI_OPTHRDNM
}	E_Bk9000Info_FUNC;

typedef struct	Bk9000_INFO_REQ
{
	Bk9000_COUPLER		* pcoupler;
	UINT16			slot;
	E_Bk9000Info_FUNC	funcid;
}	Bk9000_INFO_REQ;

/* This function will be called by all Bk9000Info device support */
static int Bk9000_Info_Req_Init(dbCommon * precord, E_EPICS_RTYPE epics_rtype, char * ioString)
{
	SINT32				count;
	char				cplrname[MAX_CA_STRING_SIZE];
	char				func[MAX_CA_STRING_SIZE];
	SINT32				slotnum;
	Bk9000_COUPLER	*	pcoupler;
	E_Bk9000Info_FUNC	functionID;
	Bk9000_INFO_REQ	*	pinforeq;

	if(precord == NULL)
	{
		errlogPrintf("No legal record pointer in Bk9000_Info_Req_Init!\n");
		return -1;
	}

	if(ioString == NULL)
	{
		errlogPrintf("No INP/OUT field for record %s!\n", precord->name);
		return -1;
	}

	count = sscanf(ioString, "%[^:]:%i:%[^:]", cplrname, &slotnum, func);
	if (count != 3)
	{
		errlogPrintf("Record %s INP/OUT string %s format is illegal!\n", precord->name, ioString);
		return -1;
	}

	pcoupler = Bk9000_Get_Coupler_By_Name(cplrname);
	if(pcoupler == NULL)
	{
		errlogPrintf("Can't find coupler %s for record %s!\n", cplrname, precord->name);
		return -1;
	}

	/* For most of records, the slot number doesn't matter and should be 0, but we don't enforce */
	if( slotnum < 0 || slotnum > MAX_NUM_OF_BUSTERM )
	{
		errlogPrintf("Terminal must be on slot 1~%d for record %s!\n", MAX_NUM_OF_BUSTERM, precord->name);
		return -1;
	}

	switch(epics_rtype)
	{
	case EPICS_RTYPE_LI:
		if( 0 == strcmp(func, "COWCNT") )	functionID = Bk9000Info_LI_COWCNT;
		else if( 0 == strcmp(func, "CIWCNT") )	functionID = Bk9000Info_LI_CIWCNT;
		else if( 0 == strcmp(func, "DOBCNT") )	functionID = Bk9000Info_LI_DOBCNT;
		else if( 0 == strcmp(func, "DIBCNT") )	functionID = Bk9000Info_LI_DIBCNT;
		else if( 0 == strcmp(func, "BTTYPE") )	functionID = Bk9000Info_LI_BTTYPE;
		else if( 0 == strcmp(func, "NOFCONN") )	functionID = Bk9000Info_LI_NOFCONN;
		else if( 0 == strcmp(func, "NOFEXCP") )	functionID = Bk9000Info_LI_NOFEXCP;
		else if( 0 == strcmp(func, "NOFPKTS") )	functionID = Bk9000Info_LI_NOFPKTS;
		else if( 0 == strcmp(func, "NOFSIGS") )	functionID = Bk9000Info_LI_NOFSIGS;
		else if( 0 == strcmp(func, "OPTHRDID") )functionID = Bk9000Info_LI_OPTHRDID;
		else
		{
			errlogPrintf("Unsupported function for Bk9000Info of record %s!\n", precord->name);
			return -1;
		}
		break;
	case EPICS_RTYPE_MBBI:
		if ( 0 == strcmp(func, "CPLRRDY") )
			functionID = Bk9000Info_MBBI_CPLRRDY;
		else if( 0 == strcmp(func, "LINKSTAT") )
			functionID = Bk9000Info_MBBI_LINKSTAT;
		else
		{
			errlogPrintf("Unsupported function for Bk9000Info of record %s!\n", precord->name);
			return -1;
		}
		break;
	case EPICS_RTYPE_SI:
		if( 0 == strcmp(func, "CPLRNAME") )	functionID = Bk9000Info_SI_CPLRNAME;
		else if( 0 == strcmp(func, "CPLRIP") )	functionID = Bk9000Info_SI_CPLRIP;
		else if( 0 == strcmp(func, "TLSTCONN") )functionID = Bk9000Info_SI_TLSTCONN;
		else if( 0 == strcmp(func, "TSETCONN") )functionID = Bk9000Info_SI_TSETCONN;
		else if( 0 == strcmp(func, "TLASTTRY") )functionID = Bk9000Info_SI_TLASTTRY;
		else if( 0 == strcmp(func, "CPLRID") )	functionID = Bk9000Info_SI_CPLRID;
		else if( 0 == strcmp(func, "OPTHRDNM") )functionID = Bk9000Info_SI_OPTHRDNM;
		else
		{
			errlogPrintf("Unsupported function for Bk9000Info of record %s!\n", precord->name);
			return -1;
		}
		break;
	default:
		errlogPrintf("Unsupported record type for Bk9000Info of record %s!\n", precord->name);
		return -1;
	}

	pinforeq = (Bk9000_INFO_REQ *)malloc(sizeof(Bk9000_INFO_REQ));
	if(pinforeq == NULL)
	{
		errlogPrintf("Fail to malloc memory for record %s!\n", precord->name);
		return -1;
	}

	memset( (char *)pinforeq, 0, sizeof(Bk9000_INFO_REQ) );

	pinforeq->pcoupler = pcoupler;
	pinforeq->slot = slotnum;
	pinforeq->funcid = functionID;

	precord->dpvt = (void *)pinforeq;
	return 0;
}

/* function prototypes */
static long init_li_Bk9000Info(struct longinRecord *pli);
static long read_li_Bk9000Info(struct longinRecord *pli);

static long init_mbbi_Bk9000Info(struct mbbiRecord *pmbbi);
static long read_mbbi_Bk9000Info(struct mbbiRecord *pmbbi);

static long init_si_Bk9000Info(struct stringinRecord *psi);
static long read_si_Bk9000Info(struct stringinRecord *psi);

/* global struct for devSup */
typedef struct
{
	long		number;
	DEVSUPFUN	report;
	DEVSUPFUN	init;
	DEVSUPFUN	init_record;
	DEVSUPFUN	get_ioint_info;
	DEVSUPFUN	read_write;
	DEVSUPFUN	special_linconv;
}	Bk9000Info_DEV_SUP_SET;

Bk9000Info_DEV_SUP_SET devLiBk9000Info= {6, NULL, NULL, init_li_Bk9000Info, NULL, read_li_Bk9000Info, NULL};
Bk9000Info_DEV_SUP_SET devMbbiBk9000Info= {6, NULL, NULL, init_mbbi_Bk9000Info, NULL, read_mbbi_Bk9000Info, NULL};
Bk9000Info_DEV_SUP_SET devSiBk9000Info= {6, NULL, NULL, init_si_Bk9000Info, NULL, read_si_Bk9000Info, NULL};

epicsExportAddress(dset, devLiBk9000Info);
epicsExportAddress(dset, devMbbiBk9000Info);
epicsExportAddress(dset, devSiBk9000Info);

static long init_li_Bk9000Info(struct longinRecord * pli)
{
	if (pli->inp.type!=INST_IO)
	{
		recGblRecordError(S_db_badField, (void *)pli,
				"devLiBk9000Info Init_record, Illegal INP");
		pli->pact=TRUE;
		return (S_db_badField);
	}

	if(Bk9000_Info_Req_Init((dbCommon *) pli, EPICS_RTYPE_LI, pli->inp.value.instio.string) != 0)
	{
		if(Bk9000_DEV_DEBUG)
			errlogPrintf("Fail to init info_req for record %s!!", pli->name);
		recGblRecordError(S_db_badField, (void *) pli, "Init info_req Error");
		pli->pact = TRUE;
		return (S_db_badField);
	}

	return 0;
}

static long read_li_Bk9000Info(struct longinRecord * pli)
{
	Bk9000_INFO_REQ	*	pinforeq = (Bk9000_INFO_REQ *) (pli->dpvt);
	UINT32				temp;

	switch((int)(pinforeq->funcid))
	{
	case Bk9000Info_LI_COWCNT:
		pli->val = (pinforeq->pcoupler->complex_out_bits)/16;
		if( !(pinforeq->pcoupler->couplerReady) )
			recGblSetSevr(pli, READ_ALARM, INVALID_ALARM);
		break;
	case Bk9000Info_LI_CIWCNT:
		pli->val = (pinforeq->pcoupler->complex_in_bits)/16;
		if( !(pinforeq->pcoupler->couplerReady) )
			recGblSetSevr(pli, READ_ALARM, INVALID_ALARM);
		break;
	case Bk9000Info_LI_DOBCNT:
		pli->val = pinforeq->pcoupler->digital_out_bits;
		if( !(pinforeq->pcoupler->couplerReady) )
			recGblSetSevr(pli, READ_ALARM, INVALID_ALARM);
		break;
	case Bk9000Info_LI_DIBCNT:
		pli->val = pinforeq->pcoupler->digital_in_bits;
		if( !(pinforeq->pcoupler->couplerReady) )
			recGblSetSevr(pli, READ_ALARM, INVALID_ALARM);
		break;
	case Bk9000Info_LI_BTTYPE:   /* slot could be vary */
		if( pinforeq->pcoupler->installedBusTerm[pinforeq->slot].pbusterm_img_def )
		{
			pli->val = pinforeq->pcoupler->installedBusTerm[pinforeq->slot].pbusterm_img_def->busterm_type;
			if( !(pinforeq->pcoupler->couplerReady) )
				recGblSetSevr(pli, READ_ALARM, INVALID_ALARM);
		}
		else
		{
			recGblSetSevr(pli, READ_ALARM, INVALID_ALARM);
		}
		break;
	case Bk9000Info_LI_NOFCONN:
		MBT_GetNthOfConn(pinforeq->pcoupler->mbt_link, &temp);
		pli->val = temp;
		if( !(pinforeq->pcoupler->couplerReady) )
			recGblSetSevr(pli, READ_ALARM, INVALID_ALARM);
		break;
	case Bk9000Info_LI_NOFEXCP:
		MBT_GetRemoteErrCnt(pinforeq->pcoupler->mbt_link, &temp);
		pli->val = temp;
		if( !(pinforeq->pcoupler->couplerReady) )
			recGblSetSevr(pli, READ_ALARM, INVALID_ALARM);
		break;
	case Bk9000Info_LI_NOFPKTS:
		MBT_GetNofPackets(pinforeq->pcoupler->mbt_link, &temp);
					pli->val = temp;
		if( !(pinforeq->pcoupler->couplerReady) )
			recGblSetSevr(pli, READ_ALARM, INVALID_ALARM);
		break;
	case Bk9000Info_LI_NOFSIGS:
		pli->val = ellCount( (ELLLIST *)&(pinforeq->pcoupler->sigptr_list) );
		break;
	case Bk9000Info_LI_OPTHRDID:
		pli->val = (UINT32)((long)(pinforeq->pcoupler->opthread_id));
		break;
	}
	pli->udf=FALSE;
	return 0;
}

/******* mbbi record *************/
static long init_mbbi_Bk9000Info(struct mbbiRecord *pmbbi)
{
	if (pmbbi->inp.type!=INST_IO)
	{
		recGblRecordError(S_db_badField, (void *)pmbbi,
				"devMbbiBk9000Info Init_record, Illegal INP");
		pmbbi->pact=TRUE;
		return (S_db_badField);
	}

	if(Bk9000_Info_Req_Init((dbCommon *) pmbbi, EPICS_RTYPE_MBBI, pmbbi->inp.value.instio.string) != 0)
	{
		if(Bk9000_DEV_DEBUG)
			errlogPrintf("Fail to init info_req for record %s!!", pmbbi->name);
		recGblRecordError(S_db_badField, (void *) pmbbi, "Init info_req Error");
		pmbbi->pact = TRUE;
		return (S_db_badField);
	}

	pmbbi->shft = 0;
	return 0;
}

static long read_mbbi_Bk9000Info(struct mbbiRecord *pmbbi)
{
	Bk9000_INFO_REQ         * pinforeq = (Bk9000_INFO_REQ *) (pmbbi->dpvt);
	SINT32                  temp;

	switch((int)(pinforeq->funcid))
	{
	case Bk9000Info_MBBI_CPLRRDY:
		pmbbi->rval = pinforeq->pcoupler->couplerReady;
		break;
	case Bk9000Info_MBBI_LINKSTAT:
		MBT_GetLinkStat(pinforeq->pcoupler->mbt_link, &temp);
		pmbbi->rval = temp;
		break;
	}

	pmbbi->udf=FALSE;
	return  CONVERT;
}

/******* stringin  record *************/
static long init_si_Bk9000Info(struct stringinRecord *psi)
{
	if (psi->inp.type!=INST_IO)
	{
		recGblRecordError(S_db_badField, (void *)psi,
				"devSiBk9000Info Init_record, Illegal INP");
		psi->pact=TRUE;
		return (S_db_badField);
	}

	if(Bk9000_Info_Req_Init((dbCommon *) psi, EPICS_RTYPE_SI, psi->inp.value.instio.string) != 0)
	{
		if(Bk9000_DEV_DEBUG)
			errlogPrintf("Fail to init info_req for record %s!!", psi->name);
		recGblRecordError(S_db_badField, (void *) psi, "Init info_req Error");
		psi->pact = TRUE;
		return (S_db_badField);
	}

	return 0;
}

static long read_si_Bk9000Info(struct stringinRecord *psi)
{
	Bk9000_INFO_REQ	*	pinforeq = (Bk9000_INFO_REQ *) (psi->dpvt);
	struct sockaddr_in	addr;
	switch ((int)(pinforeq->funcid))
	{
	case Bk9000Info_SI_CPLRNAME:
		strncpy(psi->val, MBT_GetName(pinforeq->pcoupler->mbt_link), MAX_CA_STRING_SIZE-1);
		psi->val[MAX_CA_STRING_SIZE-1] = '\0';
		break;
	case Bk9000Info_SI_CPLRIP:
		MBT_GetAddr(pinforeq->pcoupler->mbt_link, &addr);
		strncpy(psi->val, inet_ntoa(addr.sin_addr), MAX_CA_STRING_SIZE-1);
		psi->val[MAX_CA_STRING_SIZE-1] = '\0';
		break;
	case Bk9000Info_SI_TLSTCONN:
		epicsTimeToStrftime(psi->val, MAX_CA_STRING_SIZE-1, "%Y/%m/%d %H:%M:%S.%06f",
							&(pinforeq->pcoupler->time_lost_conn));
		break;
	case Bk9000Info_SI_TSETCONN:
		epicsTimeToStrftime(psi->val, MAX_CA_STRING_SIZE-1, "%Y/%m/%d %H:%M:%S.%06f",
							&(pinforeq->pcoupler->time_set_conn));
		break;
	case Bk9000Info_SI_TLASTTRY:
		epicsTimeToStrftime(psi->val, MAX_CA_STRING_SIZE-1, "%Y/%m/%d %H:%M:%S.%06f",
							&(pinforeq->pcoupler->time_last_try));
		break;
	case Bk9000Info_SI_CPLRID:
		strcpy(psi->val, pinforeq->pcoupler->couplerID);
		if( !(pinforeq->pcoupler->couplerReady) )
			recGblSetSevr(psi, READ_ALARM, INVALID_ALARM);
		break;
	case Bk9000Info_SI_OPTHRDNM:
		strcpy(psi->val, pinforeq->pcoupler->opthread_name);
		break;
	}
	
	psi->udf=FALSE;
	return 0;
}
