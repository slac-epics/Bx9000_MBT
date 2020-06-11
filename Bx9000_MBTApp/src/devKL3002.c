#include <Bk9000_MBT_Common.h>
#include <aiRecord.h>

extern	SINT32	Bk9000_DEV_DEBUG;
	
static long init_ai_KL3002(struct aiRecord * pai)
{
	if (pai->inp.type!=INST_IO)
	{
		recGblRecordError(S_db_badField, (void *)pai,
			"devAiKL3002 Init_record, Illegal INP");
		pai->pact=TRUE;
		return (S_db_badField);
	}

	if(Bk9000_Signal_Init((dbCommon *) pai, EPICS_RTYPE_AI, pai->inp.value.instio.string, BT_TYPE_KL3002, Bk9000_Dft_ProcFunc, NULL) != 0)
	{
		if(Bk9000_DEV_DEBUG)	errlogPrintf("Fail to init signal for record %s!!", pai->name);
		recGblRecordError(S_db_badField, (void *) pai, "Init signal Error");
		pai->pact = TRUE;
		return (S_db_badField);
	}

	pai->eslo = (pai->eguf - pai->egul)/(float)0x1000;
	pai->roff = 0x800;

	return 0;
}

static long read_ai_KL3002(struct aiRecord * pai)
{
	Bk9000_SIGNAL	* psignal = (Bk9000_SIGNAL *) (pai->dpvt);

	if (!pai->pact)
	{
		if(epicsMessageQueueTrySend(psignal->pdevdata->pcoupler->msgQ_id, (void *)&psignal, sizeof(Bk9000_SIGNAL *)) == -1)
		{
			recGblSetSevr(pai, READ_ALARM, INVALID_ALARM);
			if(Bk9000_DEV_DEBUG)	errlogPrintf("Send Message to Operation Thread Error [%s]", pai->name);
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
			if(Bk9000_DEV_DEBUG)	errlogPrintf("Record [%s] receive error code [0x%08x]!\n", pai->name, psignal->pdevdata->err_code);
			return -1;
		}
		else
		{
			pai->udf = FALSE;
			/* psignal->pdevdata->pbusterm_sig_def->data_type is a little bit overkill */
			/* We don't check term_reg_exist, because we know we do */
			if(psignal->pdevdata->pcoupler->installedBusTerm[psignal->pdevdata->slot].term_r32_value & 0x8)/* check bit 3 */
			{/* signed amount */
				pai->rval = ( (psignal->pdevdata->value)&0x7FF ) * ( ((psignal->pdevdata->value)&0x800)?-1:1 );
			}
			else
			{/* two's complement */
				pai->rval = (SINT16)(psignal->pdevdata->value);
			}
		}
	}
	return (CONVERT);
}

static long lincvt_ai_KL3002(struct aiRecord	*pai, int after)
{

	if(!after) return(0);
	/* set linear conversion slope*/
	pai->eslo = (pai->eguf - pai->egul)/(float)0x1000;
	pai->roff = 0x800;
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
}	devAiKL3002 =
{
	6,
	NULL,
	NULL,
	init_ai_KL3002,
	NULL,
	read_ai_KL3002,
	lincvt_ai_KL3002
};
epicsExportAddress(dset, devAiKL3002);

