/* drvBk9000_MBTRegister.cpp */

#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "iocsh.h"
#include "Bk9000_MBT_Common.h"

#ifdef __cplusplus
extern "C" {
#endif	/* __cplusplus */

extern SINT32 Bk9000_DRV_DEBUG;
extern SINT32 Bk9000_DEV_DEBUG;

static const iocshArg Bk9000_DRV_DEBUGArg0 = {"value", iocshArgInt};
static const iocshArg *const Bk9000_DRV_DEBUGArgs[1] = {&Bk9000_DRV_DEBUGArg0};
static const iocshFuncDef Bk9000_DRV_DEBUGDef = {"Bk9000_DRV_DEBUG", 1, Bk9000_DRV_DEBUGArgs};
static void Bk9000_DRV_DEBUGCall(const iocshArgBuf * args)
{
	Bk9000_DRV_DEBUG = args[0].ival;
}
 
static const iocshArg Bk9000_DEV_DEBUGArg0 = {"value", iocshArgInt};
static const iocshArg *const Bk9000_DEV_DEBUGArgs[1] = {&Bk9000_DEV_DEBUGArg0};
static const iocshFuncDef Bk9000_DEV_DEBUGDef = {"Bk9000_DEV_DEBUG", 1, Bk9000_DEV_DEBUGArgs};
static void Bk9000_DEV_DEBUGCall(const iocshArgBuf * args)
{
	Bk9000_DEV_DEBUG = args[0].ival;
}

static const iocshArg Bk9000_Coupler_AddArg0 = {"cplrname", iocshArgString};
static const iocshArg Bk9000_Coupler_AddArg1 = {"ipaddr", iocshArgString};
static const iocshArg Bk9000_Coupler_AddArg2 = {"init_string", iocshArgString};
static const iocshArg * const Bk9000_Coupler_AddArgs[3] = {&Bk9000_Coupler_AddArg0, &Bk9000_Coupler_AddArg1, &Bk9000_Coupler_AddArg2};
static const iocshFuncDef Bk9000_Coupler_AddDef = {"Bk9000_Coupler_Add", 3, Bk9000_Coupler_AddArgs};
static void Bk9000_Coupler_AddCall(const iocshArgBuf * args)
{
	Bk9000_Coupler_Add( args[0].sval, args[1].sval, args[2].sval );
}

static const iocshArg Bk9000_Terminal_AddArg0 = {"cplrname", iocshArgString};
static const iocshArg Bk9000_Terminal_AddArg1 = {"slot", iocshArgInt};
static const iocshArg Bk9000_Terminal_AddArg2 = {"btname", iocshArgString};
static const iocshArg Bk9000_Terminal_AddArg3 = {"init_string", iocshArgString};
static const iocshArg * const Bk9000_Terminal_AddArgs[4] = {&Bk9000_Terminal_AddArg0, &Bk9000_Terminal_AddArg1, &Bk9000_Terminal_AddArg2, &Bk9000_Terminal_AddArg3};
static const iocshFuncDef Bk9000_Terminal_AddDef = {"Bk9000_Terminal_Add", 4, Bk9000_Terminal_AddArgs};
static void Bk9000_Terminal_AddCall(const iocshArgBuf * args)
{
	Bk9000_Terminal_Add( args[0].sval, (UINT16)args[1].ival, args[2].sval, args[3].sval );
}

void drvBk9000_MBT_Register()
{
	static int firstTime = 1;
	if  (!firstTime)
	    return;
	firstTime = 0;
	iocshRegister(&Bk9000_DRV_DEBUGDef, Bk9000_DRV_DEBUGCall);
	iocshRegister(&Bk9000_DEV_DEBUGDef, Bk9000_DEV_DEBUGCall);
	iocshRegister(&Bk9000_Coupler_AddDef, Bk9000_Coupler_AddCall);
	iocshRegister(&Bk9000_Terminal_AddDef, Bk9000_Terminal_AddCall);
}

epicsExportRegistrar(drvBk9000_MBT_Register);

#ifdef __cplusplus
}
#endif	/* __cplusplus */

/*
 * Register commands on application startup
 * In the funture we might change this to xxx = drvBk9000_MBT_Register(); to guarantee link
 */
class drvBk9000_MBT_iocshReg
{
public:
    drvBk9000_MBT_iocshReg()
	{
		drvBk9000_MBT_Register();
    }
};
static drvBk9000_MBT_iocshReg drvBk9000_MBT_iocshRegObj;
