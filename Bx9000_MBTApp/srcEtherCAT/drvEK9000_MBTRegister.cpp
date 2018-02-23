//////////////////////////////////////////////////////////////////////////////
// This file is part of 'Bx9000_MBT'.
// It is subject to the license terms in the LICENSE.txt file found in the 
// top-level directory of this distribution and at: 
//    https://confluence.slac.stanford.edu/display/ppareg/LICENSE.html. 
// No part of 'Bx9000_MBT', including this file, 
// may be copied, modified, propagated, or distributed except according to 
// the terms contained in the LICENSE.txt file.
//////////////////////////////////////////////////////////////////////////////
/* drvEK9000_MBTRegister.cpp */

#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "iocsh.h"
#include "EK9000_MBT_Common.h"

#ifdef __cplusplus
extern "C" {
#endif	/* __cplusplus */

extern SINT32 EK9000_DRV_DEBUG;
extern SINT32 EK9000_DEV_DEBUG;

static const iocshArg EK9000_DRV_DEBUGArg0 = {"value", iocshArgInt};
static const iocshArg *const EK9000_DRV_DEBUGArgs[1] = {&EK9000_DRV_DEBUGArg0};
static const iocshFuncDef EK9000_DRV_DEBUGDef = {"EK9000_DRV_DEBUG", 1, EK9000_DRV_DEBUGArgs};
static void EK9000_DRV_DEBUGCall(const iocshArgBuf * args)
{
	EK9000_DRV_DEBUG = args[0].ival;
}
 
static const iocshArg EK9000_DEV_DEBUGArg0 = {"value", iocshArgInt};
static const iocshArg *const EK9000_DEV_DEBUGArgs[1] = {&EK9000_DEV_DEBUGArg0};
static const iocshFuncDef EK9000_DEV_DEBUGDef = {"EK9000_DEV_DEBUG", 1, EK9000_DEV_DEBUGArgs};
static void EK9000_DEV_DEBUGCall(const iocshArgBuf * args)
{
	EK9000_DEV_DEBUG = args[0].ival;
}

static const iocshArg EK9000_Coupler_AddArg0 = {"cplrname", iocshArgString};
static const iocshArg EK9000_Coupler_AddArg1 = {"ipaddr", iocshArgString};
static const iocshArg EK9000_Coupler_AddArg2 = {"init_string", iocshArgString};
static const iocshArg * const EK9000_Coupler_AddArgs[3] = {&EK9000_Coupler_AddArg0, &EK9000_Coupler_AddArg1, &EK9000_Coupler_AddArg2};
static const iocshFuncDef EK9000_Coupler_AddDef = {"EK9000_Coupler_Add", 3, EK9000_Coupler_AddArgs};
static void EK9000_Coupler_AddCall(const iocshArgBuf * args)
{
	EK9000_Coupler_Add( args[0].sval, args[1].sval, args[2].sval );
}

static const iocshArg EK9000_Terminal_AddArg0 = {"cplrname", iocshArgString};
static const iocshArg EK9000_Terminal_AddArg1 = {"slot", iocshArgInt};
static const iocshArg EK9000_Terminal_AddArg2 = {"btname", iocshArgString};
static const iocshArg EK9000_Terminal_AddArg3 = {"init_string", iocshArgString};
static const iocshArg * const EK9000_Terminal_AddArgs[4] = {&EK9000_Terminal_AddArg0, &EK9000_Terminal_AddArg1, &EK9000_Terminal_AddArg2, &EK9000_Terminal_AddArg3};
static const iocshFuncDef EK9000_Terminal_AddDef = {"EK9000_Terminal_Add", 4, EK9000_Terminal_AddArgs};
static void EK9000_Terminal_AddCall(const iocshArgBuf * args)
{
	EK9000_Terminal_Add( args[0].sval, (UINT16)args[1].ival, args[2].sval, args[3].sval );
}

void drvEK9000_MBT_Register()
{
	static int firstTime = 1;
	if  (!firstTime)
	    return;
	firstTime = 0;
	iocshRegister(&EK9000_DRV_DEBUGDef, EK9000_DRV_DEBUGCall);
	iocshRegister(&EK9000_DEV_DEBUGDef, EK9000_DEV_DEBUGCall);
	iocshRegister(&EK9000_Coupler_AddDef, EK9000_Coupler_AddCall);
	iocshRegister(&EK9000_Terminal_AddDef, EK9000_Terminal_AddCall);
}

epicsExportRegistrar(drvEK9000_MBT_Register);

#ifdef __cplusplus
}
#endif	/* __cplusplus */

/*
 * Register commands on application startup
 * In the funture we might change this to xxx = drvEK9000_MBT_Register(); to guarantee link
 */
class drvEK9000_MBT_iocshReg
{
public:
    drvEK9000_MBT_iocshReg()
	{
		drvEK9000_MBT_Register();
    }
};
static drvEK9000_MBT_iocshReg drvEK9000_MBT_iocshRegObj;
