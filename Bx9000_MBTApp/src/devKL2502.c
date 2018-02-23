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
#include <aoRecord.h>

extern	SINT32	Bk9000_DEV_DEBUG;

/* The KL2502 duty cycle value is 0x0000 for 0% and 0x7FFF for 100% */
#define	RAW_DUTY_CYCLE_0	0x0000
#define	RAW_DUTY_CYCLE_100	0x7FFF

static long lincvt_ao_KL2502(struct aoRecord	*pao, int after)
{
	if(!after)
		return(0);

	/*
	 * Linear conversion slope calculated from eng units full - eng units low
	 */
	pao->eslo = (	(pao->eguf - pao->egul)
				/	(double)(RAW_DUTY_CYCLE_100 - RAW_DUTY_CYCLE_0) );
	pao->eoff = 0;
	pao->roff = RAW_DUTY_CYCLE_0;
	return(0);
}


static long init_ao_KL2502(struct aoRecord * pao)
{
	SINT32 status;
	Bk9000_SIGNAL * psignal;

	if (pao->out.type!=INST_IO)
	{
		recGblRecordError(S_db_badField, (void *)pao,
			"devAoKL2502 Init_record, Illegal OUT");
		pao->pact=TRUE;
		return (S_db_badField);
	}

	/*
	 * Update Linear conversion slope
	 */
	lincvt_ao_KL2502( pao, 1 );

	/*
	 * Initialize Signal
	 */
	if ( Bk9000_Signal_Init(	(dbCommon *) pao, EPICS_RTYPE_AO,
								pao->out.value.instio.string,
								BT_TYPE_KL2502, Bk9000_Dft_ProcFunc, NULL ) != 0 )
	{
		if(Bk9000_DEV_DEBUG)	errlogPrintf("Fail to init signal for record %s!!", pao->name);
		recGblRecordError(S_db_badField, (void *) pao, "Init signal Error");
		pao->pact = TRUE;
		return (S_db_badField);
	}

	psignal = (Bk9000_SIGNAL *) (pao->dpvt);
	status = Bk9000_Dft_OutInit(psignal);
	if(status != 0)
	{/* Init value failed */
		/* pao->udf = TRUE; */

		/* Leave UDF as is, don't set SEVERITY */
		/* recGblSetSevr(pao,READ_ALARM,INVALID_ALARM); */
		return -1;
	}
	else
	{
		/*	Feature register	*/
		/*	psignal->pdevdata->pcoupler->installedBusTerm[psignal->pdevdata->slot].term_r32_value	*/
		/*	Bit 0:	User scaling active								*/
		/*	Bit 1:	Manufacturer scaling active						*/
		/*	Bit 2:	Watchdog timer active							*/
		/*	Bits 13-15:	Mode										*/
		/*		000:	PWMH mode			250Hz to 20kHz			*/
		/*		001:	PWMH mode			2Hz   to 250Hz			*/
		/*		011:	Frq-Cnt PWM mode	2Hz   to 2kHz			*/
		/*		101:	Frq-Cnt pulse mode	2Hz   to 2kHz			*/
		/*		111:	Cnt-Cnt PWM mode	250Hz to 8kHz			*/

		pao->rval	= (SINT16)(psignal->pdevdata->value);
		pao->udf	= FALSE;
		pao->stat	= pao->sevr	= NO_ALARM;
	}

	return CONVERT;
}

static long write_ao_KL2502(struct aoRecord * pao)
{
	UINT16				rawDutyCycle;
	Bk9000_SIGNAL	*	psignal = (Bk9000_SIGNAL *) (pao->dpvt);

	if (!pao->pact)
	{
		/*	Limit range of raw value */
		if( pao->rval < RAW_DUTY_CYCLE_0 )
			rawDutyCycle = RAW_DUTY_CYCLE_0;
		else if( pao->rval > RAW_DUTY_CYCLE_100 )
			rawDutyCycle = RAW_DUTY_CYCLE_100;
		else
			rawDutyCycle = (UINT16)(pao->rval);

		psignal->pdevdata->value = rawDutyCycle;
		if ( epicsMessageQueueTrySend(	psignal->pdevdata->pcoupler->msgQ_id,
										(void *)&psignal,
										sizeof(Bk9000_SIGNAL *) ) == -1 )
		{
			recGblSetSevr( pao, WRITE_ALARM, INVALID_ALARM );
			if(Bk9000_DEV_DEBUG)	errlogPrintf("Send Message to Operation Thread Error [%s]", pao->name);
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
			if(Bk9000_DEV_DEBUG)	errlogPrintf("Record [%s] receive error code [0x%08x]!\n", pao->name, psignal->pdevdata->err_code);
			return -1;
		}
	}
	return 0;
}


struct {
	long            number;
	DEVSUPFUN       report;
	DEVSUPFUN       init;
	DEVSUPFUN       init_ao;
	DEVSUPFUN       get_ioint_info;
	DEVSUPFUN       write_ao;
	DEVSUPFUN       special_linconv;
}	devAoKL2502 =
{
	6,
	NULL,
	NULL,
	init_ao_KL2502,
	NULL,
	write_ao_KL2502,
	lincvt_ao_KL2502
};
epicsExportAddress(dset, devAoKL2502);

