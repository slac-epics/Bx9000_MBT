#include <EK9000_MBT_Common.h>
#include <longinRecord.h>
/* EL5042 is 2-channel BiSS-C terminal with configurable serial params and resolution up to 64-bit */
/* Note that prior to EPICS R3.16.1 longin records only seem to handle up to 32-bit integers */

extern	SINT32	EK9000_DEV_DEBUG;
	
static long init_li_EL5042(struct longinRecord * pli)
{
	if (pli->inp.type!=INST_IO)
	{
		recGblRecordError(S_db_badField, (void *)pli,
			"devLiEL5042 Init_record, Illegal INP");
		pli->pact=TRUE;
		return (S_db_badField);
	}

	if(EK9000_Signal_Init((dbCommon *) pli, EPICS_RTYPE_LI, pli->inp.value.instio.string, BT_TYPE_EL5042, EK9000_Dft_ProcFunc, NULL) != 0)
	{
		if(EK9000_DEV_DEBUG)	errlogPrintf("Fail to init signal for record %s!!", pli->name);
		recGblRecordError(S_db_badField, (void *) pli, "Init signal Error");
		pli->pact = TRUE;
		return (S_db_badField);
	}

	return 0;
}

static long read_li_EL5042(struct longinRecord * pli)
{
	EK9000_SIGNAL	* psignal = (EK9000_SIGNAL *) (pli->dpvt);
        if(sizeof(pli->val) <= 4) {
		errlogPrintf("Warning, only 32-bit resolution supported in EPICS < 3.16.1"); }
	pli->val = psignal->pdevdata->value;

	if (!pli->pact)
	{
		if(epicsMessageQueueTrySend(psignal->pdevdata->pcoupler->msgQ_id, (void *)&psignal, sizeof(EK9000_SIGNAL *)) == -1)
		{
			recGblSetSevr(pli, READ_ALARM, INVALID_ALARM);
			if(EK9000_DEV_DEBUG)	errlogPrintf("Send Message to Operation Thread Error [%s]", pli->name);
			return -1;
		}
		else
		{
			pli->pact = TRUE;
		}
	}
	else
	{
		if ( (!psignal->pdevdata->op_done) || psignal->pdevdata->err_code )
		{
			recGblSetSevr(pli, READ_ALARM, INVALID_ALARM);
			if(EK9000_DEV_DEBUG)	errlogPrintf("Record [%s] receive error code [0x%08x]!\n", pli->name, psignal->pdevdata->err_code);
			return -1;
		}
		else
		{
			pli->udf = FALSE;
		}
	}
	return (0);
}

struct {
	long            number;
	DEVSUPFUN       report;
	DEVSUPFUN       init;
	DEVSUPFUN       init_li;
	DEVSUPFUN       get_ioint_info;
	DEVSUPFUN       read_li;
}	devLiEL5042 =
{
	6,
	NULL,
	NULL,
	init_li_EL5042,
	NULL,
	read_li_EL5042,
};
epicsExportAddress(dset, devLiEL5042);

