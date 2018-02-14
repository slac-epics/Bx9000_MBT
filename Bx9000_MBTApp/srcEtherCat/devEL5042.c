#include <EK9000_MBT_Common.h>
#include <longinRecord.h>
//TODO: change to long input record
/* EL5042 is 2-channel BiSS-C terminal with configurable serial params and resolution up to 64-bit */


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

	if(EK9000_Signal_Init((dbCommon *) pli, EPICS_RTYPE_AI, pli->inp.value.instio.string, BT_TYPE_EL5042, EK9000_Dft_ProcFunc, NULL) != 0)
	{
		if(EK9000_DEV_DEBUG)	errlogPrintf("Fail to init signal for record %s!!", pli->name);
		recGblRecordError(S_db_badField, (void *) pli, "Init signal Error");
		pli->pact = TRUE;
		return (S_db_badField);
	}

	pli->eslo = (pli->eguf - pli->egul)/(float)0x10000;
	pli->roff = 0x8000;

	return 0;
}

static long read_li_EL5042(struct longinRecord * pli)
{
	EK9000_SIGNAL	* psignal = (EK9000_SIGNAL *) (pli->dpvt);

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
//  Commenting out these lines since there is no feature register in EtherCAT.  Will clean up later.
			/* psignal->pdevdata->pbusterm_sig_def->data_type is a little bit overkill */
			/* We don't check term_reg_exist, because we know we do */
//			if(psignal->pdevdata->pcoupler->installedBusTerm[psignal->pdevdata->slot].term_r32_value & 0x8)/* check bit 3 */
//			{/* signed amount */
//				pli->rval = ( (psignal->pdevdata->value)&0x7FFF ) * ( ((psignal->pdevdata->value)&0x8000)?-1:1 );
//			}
//			else
//			{/* two's complement */
//				pli->rval = (SINT16)(psignal->pdevdata->value);
//			}
		}
	}
	return (CONVERT);
}

static long lincvt_li_EL5042(struct longinRecord *pli, int after)
{

	if(!after) return(0);
	/* set linear conversion slope*/
	pli->eslo = (pli->eguf - pli->egul)/(float)0x10000;
	pli->roff = 0x8000;
	return(0);
}


struct {
	long            number;
	DEVSUPFUN       report;
	DEVSUPFUN       init;
	DEVSUPFUN       init_li;
	DEVSUPFUN       get_ioint_info;
	DEVSUPFUN       read_li;
	DEVSUPFUN       special_linconv;
}	devLiEL5042 =
{
	6,
	NULL,
	NULL,
	init_li_EL5042,
	NULL,
	read_li_EL5042,
	lincvt_li_EL5042
};
epicsExportAddress(dset, devLiEL5042);

