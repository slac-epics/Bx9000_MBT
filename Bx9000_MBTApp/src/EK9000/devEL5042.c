#include <EK9000_MBT_Common.h>
#include <aiRecord.h>
/* EL5042 is 2-channel BiSS-C terminal with configurable serial params and resolution up to 64-bit */


extern	SINT32	EK9000_DEV_DEBUG;
	
static long init_ai_EL5042(struct aiRecord * pai)
{
	if (pai->inp.type!=INST_IO)
	{
		recGblRecordError(S_db_badField, (void *)pai,
			"devAiEL5042 Init_record, Illegal INP");
		pai->pact=TRUE;
		return (S_db_badField);
	}

	if(EK9000_Signal_Init((dbCommon *) pai, EPICS_RTYPE_AI, pai->inp.value.instio.string, BT_TYPE_EL5042, EK9000_Dft_ProcFunc, NULL) != 0)
	{
		if(EK9000_DEV_DEBUG)	errlogPrintf("Fail to init signal for record %s!!", pai->name);
		recGblRecordError(S_db_badField, (void *) pai, "Init signal Error");
		pai->pact = TRUE;
		return (S_db_badField);
	}

	pai->eslo = (pai->eguf - pai->egul)/(float)0x10000;
	pai->roff = 0x8000;

	return 0;
}

static long read_ai_EL5042(struct aiRecord * pai)
{
	EK9000_SIGNAL	* psignal = (EK9000_SIGNAL *) (pai->dpvt);

	if (!pai->pact)
	{
		if(epicsMessageQueueTrySend(psignal->pdevdata->pcoupler->msgQ_id, (void *)&psignal, sizeof(EK9000_SIGNAL *)) == -1)
		{
			recGblSetSevr(pai, READ_ALARM, INVALID_ALARM);
			if(EK9000_DEV_DEBUG)	errlogPrintf("Send Message to Operation Thread Error [%s]", pai->name);
			return -1;
		}
		else
		{
			pai->pact = TRUE;
		}
	}
	else
	{
		if ( (!psignal->pdevdata->op_done) || psignal->pdevdata->err_code )
		{
			recGblSetSevr(pai, READ_ALARM, INVALID_ALARM);
			if(EK9000_DEV_DEBUG)	errlogPrintf("Record [%s] receive error code [0x%08x]!\n", pai->name, psignal->pdevdata->err_code);
			return -1;
		}
		else
		{
			pai->udf = FALSE;
			/* psignal->pdevdata->pbusterm_sig_def->data_type is a little bit overkill */
			/* We don't check term_reg_exist, because we know we do */
			if(psignal->pdevdata->pcoupler->installedBusTerm[psignal->pdevdata->slot].term_r32_value & 0x8)/* check bit 3 */
			{/* signed amount */
				pai->rval = ( (psignal->pdevdata->value)&0x7FFF ) * ( ((psignal->pdevdata->value)&0x8000)?-1:1 );
			}
			else
			{/* two's complement */
				pai->rval = (SINT16)(psignal->pdevdata->value);
			}
		}
	}
	return (CONVERT);
}

static long lincvt_ai_EL5042(struct aiRecord	*pai, int after)
{

	if(!after) return(0);
	/* set linear conversion slope*/
	pai->eslo = (pai->eguf - pai->egul)/(float)0x10000;
	pai->roff = 0x8000;
	return(0);
}


struct {
	long            number;
	DEVSUPFUN       report;
	DEVSUPFUN       init;
	DEVSUPFUN       init_ai;
	DEVSUPFUN       get_ioint_info;
	DEVSUPFUN       read_ai;
	DEVSUPFUN       special_linconv;
}	devAiEL5042 =
{
	6,
	NULL,
	NULL,
	init_ai_EL5042,
	NULL,
	read_ai_EL5042,
	lincvt_ai_EL5042
};
epicsExportAddress(dset, devAiEL5042);

