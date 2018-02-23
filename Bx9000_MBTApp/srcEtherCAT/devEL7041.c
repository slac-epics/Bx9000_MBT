//////////////////////////////////////////////////////////////////////////////
// This file is part of 'Bx9000_MBT'.
// It is subject to the license terms in the LICENSE.txt file found in the 
// top-level directory of this distribution and at: 
//    https://confluence.slac.stanford.edu/display/ppareg/LICENSE.html. 
// No part of 'Bx9000_MBT', including this file, 
// may be copied, modified, propagated, or distributed except according to 
// the terms contained in the LICENSE.txt file.
//////////////////////////////////////////////////////////////////////////////
#include <EK9000_MBT_Common.h>
#include <aiRecord.h>
#include <aoRecord.h>

/* EL7041 is a Stepper motor terminal w/ incremental encoder input */
/* Can be viewed as analog I/O  */

extern	SINT32	EK9000_DEV_DEBUG;

static long init_ai_EL7041(struct aiRecord * pai)
{
	if (pai->inp.type!=INST_IO)
	{
		recGblRecordError(S_db_badField, (void *)pai,
			"devAiEL7041 Init_record, Illegal INP");
		pai->pact=TRUE;
		return (S_db_badField);
	}

	if(Bk9000_Signal_Init((dbCommon *) pai, EPICS_RTYPE_AI, pai->inp.value.instio.string, BT_TYPE_EL7041, Bk9000_Dft_ProcFunc, NULL) != 0)
	{
		if(Bk9000_DEV_DEBUG)	errlogPrintf("Fail to init signal for record %s!!", pai->name);
		recGblRecordError(S_db_badField, (void *) pai, "Init signal Error");
		pai->pact = TRUE;
		return (S_db_badField);
	}

	/* This is a bipolar 12 bits(12b<<3) module */
	pai->eslo = (pai->eguf - pai->egul)/(float)0x10000;
	pai->roff = 0x8000;

	return 0;
}
static long init_ao_EL7041(struct aoRecord * pao)
{
	SINT32 status;
	EK9000_SIGNAL * psignal;

	if (pao->out.type!=INST_IO)
	{
		recGblRecordError(S_db_badField, (void *)pao,
			"devAoEL7041 Init_record, Illegal OUT");
		pao->pact=TRUE;
		return (S_db_badField);
	}

        pao->eslo = (pao->eguf - pao->egul)/(float)65534;
        pao->roff = 32767;

	if(EK9000_Signal_Init((dbCommon *) pao, EPICS_RTYPE_AO, pao->out.value.instio.string, BT_TYPE_EL7041, EK9000_Dft_ProcFunc, NULL) != 0)
	{
		if(EK9000_DEV_DEBUG)	errlogPrintf("Fail to init signal for record %s!!", pao->name);
		recGblRecordError(S_db_badField, (void *) pao, "Init signal Error");
		pao->pact = TRUE;
		return (S_db_badField);
	}

	psignal = (EK9000_SIGNAL *) (pao->dpvt);
	status = EK9000_Dft_OutInit(psignal);
	if(status != 0)
	{/* Init value failed */
		/* pao->udf = TRUE; */

		/* Leave UDF as is, don't set SEVERITY */
		/* recGblSetSevr(pao,READ_ALARM,INVALID_ALARM); */
		return -1;
	}
	else
	{
		pao->rval = (SINT16)(psignal->pdevdata->value);
		pao->udf = FALSE;
		pao->stat = pao->sevr = NO_ALARM;
	}

	return CONVERT;
}

static long read_ai_EL7041(struct aiRecord * pai)
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

static long write_ao_EL7041(struct aoRecord * pao)
{
	SINT16		temp;
	EK9000_SIGNAL	* psignal = (EK9000_SIGNAL *) (pao->dpvt);

	if (!pao->pact)
	{
		if( pao->rval < -32767 )	temp = -32767;
		else if( pao-> rval > 32767 )	temp = 32767;
		else	temp = (SINT16)(pao->rval);
		/* psignal->pdevdata->pbusterm_sig_def->data_type is a little bit overkill */
		psignal->pdevdata->value = (UINT16)temp;
		if(epicsMessageQueueTrySend(psignal->pdevdata->pcoupler->msgQ_id, (void *)&psignal, sizeof(EK9000_SIGNAL *)) == -1)
		{
			recGblSetSevr(pao, WRITE_ALARM, INVALID_ALARM);
			if(EK9000_DEV_DEBUG)	errlogPrintf("Send Message to Operation Thread Error [%s]", pao->name);
			return -1;
		}
		else
		{
			pao->pact = TRUE;
		}
	}
	else
	{
		if ( (!psignal->pdevdata->op_done) || psignal->pdevdata->err_code )
		{
			recGblSetSevr(pao, WRITE_ALARM, INVALID_ALARM);
			if(EK9000_DEV_DEBUG)	errlogPrintf("Record [%s] receive error code [0x%08x]!\n", pao->name, psignal->pdevdata->err_code);
			return -1;
		}
	}
	return 0;
}

static long lincvt_ao_EL7041(struct aoRecord	*pao, int after)
{

	if(!after) return(0);
	/* set linear conversion slope*/
	pao->eslo = (pao->eguf - pao->egul)/(float)65534;
	pao->roff = 32767;
	return(0);
}


struct {
	long            number;
	DEVSUPFUN       report;
	DEVSUPFUN       init;
	DEVSUPFUN       init_ao;
	DEVSUPFUN       get_ioint_info;
	DEVSUPFUN       write_ao;
	DEVSUPFUN       special_linconv;
}	devAoEL7041 =
{
	6,
	NULL,
	NULL,
	init_ao_EL7041,
	NULL,
	write_ao_EL7041,
	lincvt_ao_EL7041
};

epicsExportAddress(dset, devAiEL7041);
epicsExportAddress(dset, devAoEL7041);

