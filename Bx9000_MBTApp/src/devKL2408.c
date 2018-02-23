//////////////////////////////////////////////////////////////////////////////
// This file is part of 'Bx9000_MBT'.
// It is subject to the license terms in the LICENSE.txt file found in the 
// top-level directory of this distribution and at: 
//    https://confluence.slac.stanford.edu/display/ppareg/LICENSE.html. 
// No part of 'Bx9000_MBT', including this file, 
// may be copied, modified, propagated, or distributed except according to 
// the terms contained in the LICENSE.txt file.
//////////////////////////////////////////////////////////////////////////////
#include <Bk9000_MBT_Common.h>
#include <boRecord.h>

extern	SINT32	Bk9000_DEV_DEBUG;
	
static long init_bo_KL2408(struct boRecord * pbo)
{
	SINT32 status;
	Bk9000_SIGNAL * psignal;

	if (pbo->out.type!=INST_IO)
	{
		recGblRecordError(S_db_badField, (void *)pbo,
			"devBoKL2408 Init_record, Illegal OUT");
		pbo->pact=TRUE;
		return (S_db_badField);
	}

        pbo->mask = 0x0;

	if(Bk9000_Signal_Init((dbCommon *) pbo, EPICS_RTYPE_BO, pbo->out.value.instio.string, BT_TYPE_KL2408, Bk9000_Dft_ProcFunc, NULL) != 0)
	{
		if(Bk9000_DEV_DEBUG)	errlogPrintf("Fail to init signal for record %s!!", pbo->name);
		recGblRecordError(S_db_badField, (void *) pbo, "Init signal Error");
		pbo->pact = TRUE;
		return (S_db_badField);
	}

	psignal = (Bk9000_SIGNAL *) (pbo->dpvt);
	status = Bk9000_Dft_OutInit(psignal);
	if(status != 0)
	{/* Init value failed */
		/* pbo->udf = TRUE; */

		/* Leave UDF as is, don't set SEVERITY */
		/* recGblSetSevr(pbo,READ_ALARM,INVALID_ALARM); */
		return -1;
	}
	else
	{
		pbo->rval = (psignal->pdevdata->value)?1:0;
		pbo->udf = FALSE;
		pbo->stat = pbo->sevr = NO_ALARM;
	}

	return CONVERT;
}

static long write_bo_KL2408(struct boRecord * pbo)
{
	Bk9000_SIGNAL	* psignal = (Bk9000_SIGNAL *) (pbo->dpvt);

	if (!pbo->pact)
	{
		/* psignal->pdevdata->pbusterm_sig_def->data_type is a little bit overkill */
                /* We don't check term_reg_exist, because we know it doesn't exist */
		psignal->pdevdata->value = (pbo->rval)?1:0;
		if(epicsMessageQueueTrySend(psignal->pdevdata->pcoupler->msgQ_id, (void *)&psignal, sizeof(Bk9000_SIGNAL *)) == -1)
		{
			recGblSetSevr(pbo, WRITE_ALARM, INVALID_ALARM);
			if(Bk9000_DEV_DEBUG)	errlogPrintf("Send Message to Operation Thread Error [%s]", pbo->name);
			return -1;
		}
		else
		{
			pbo->pact = TRUE;
		}
	}
	else
	{
		if ( (!psignal->pdevdata->op_done) || psignal->pdevdata->err_code )
		{
			recGblSetSevr(pbo, WRITE_ALARM, INVALID_ALARM);
			if(Bk9000_DEV_DEBUG)	errlogPrintf("Record [%s] receive error code [0x%08x]!\n", pbo->name, psignal->pdevdata->err_code);
			return -1;
		}
	}
	return 0;
}

struct {
	long            number;
	DEVSUPFUN       report;
	DEVSUPFUN       init;
	DEVSUPFUN       init_bo;
	DEVSUPFUN       get_ioint_info;
	DEVSUPFUN       write_bo;
}	devBoKL2408 =
{
	5,
	NULL,
	NULL,
	init_bo_KL2408,
	NULL,
	write_bo_KL2408,
};
epicsExportAddress(dset, devBoKL2408);

