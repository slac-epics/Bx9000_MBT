TOP=../..

include $(TOP)/configure/CONFIG
#----------------------------------------
#  ADD MACRO DEFINITIONS AFTER THIS LINE
#=============================
#==================================================
# The following will built and instaled:
# dbd/Bk9000_MBT.dbd
# include/Bk9000_MBT.h
# bin/<arch>/Bk9000_MBTLib.munch
# lib/<arch>/libBk9000_MBT.a
#=============================
# build an ioc application

USR_CFLAGS += -DUSE_TYPED_RSET -DUSE_TYPED_DSET -DUSE_TYPED_DRVET

# This is what we will be building from here:
# create library for OSI type driver
LIBRARY_IOC += Bk9000_MBT

# This is for vxWorks specific drivers.
# Don't forget to substitute your driver name.
# create library for linking will install in <lib>
# LIBRARY_vxWorks += Bk9000_MBT

# create munch file for dynamic loading will install in <bin>
PROD_IOC_vxWorks += Bk9000_MBTLib

INC += Bk9000_Watchdog.h

# The following line makes the Bk9000_MBT.h file
# This is needed for Record support. So uncomment
# the following line if you are making record support.
##DBDINC += Bk9000_MBT
# Bk9000_MBT.dbd will be created from Bk9000_MBTInclude.dbd
DBD += Bk9000_MBT.dbd

# Add locally compiled object code
Bk9000_MBT_SRCS += Bk9000_Watchdog.cpp
Bk9000_MBT_SRCS += drvBk9000_MBT.c
Bk9000_MBT_SRCS += Bk9000_MBT_Ext.c
Bk9000_MBT_SRCS += devBk9000Info.c
Bk9000_MBT_SRCS += devKL1104.c
Bk9000_MBT_SRCS += devKL1002.c
Bk9000_MBT_SRCS += devKL1124.c
Bk9000_MBT_SRCS += devKL1408.c
Bk9000_MBT_SRCS += devKL1488.c
Bk9000_MBT_SRCS += devKL2012.c
Bk9000_MBT_SRCS += devKL2114.c
Bk9000_MBT_SRCS += devKL2124.c
Bk9000_MBT_SRCS += devKL2408.c
Bk9000_MBT_SRCS += devKL2502.c
Bk9000_MBT_SRCS += devKL2798.c
Bk9000_MBT_SRCS += devKL2808.c
Bk9000_MBT_SRCS += devKL2828.c
Bk9000_MBT_SRCS += devKL2872.c
Bk9000_MBT_SRCS += devKL2512.c
Bk9000_MBT_SRCS += devKL2622.c
Bk9000_MBT_SRCS += devKL3002.c
Bk9000_MBT_SRCS += devKL3012.c
Bk9000_MBT_SRCS += devKL3022.c
Bk9000_MBT_SRCS += devKL3064.c
Bk9000_MBT_SRCS += devKL3102.c
Bk9000_MBT_SRCS += devKL3162.c
Bk9000_MBT_SRCS += devKL3202.c
Bk9000_MBT_SRCS += devKL3204.c
Bk9000_MBT_SRCS += devKL3222.c
Bk9000_MBT_SRCS += devKL3312.c
Bk9000_MBT_SRCS += devKL3314.c
Bk9000_MBT_SRCS += devKL3404.c
Bk9000_MBT_SRCS += devKL3408.c
Bk9000_MBT_SRCS += devKL3464.c
Bk9000_MBT_SRCS += devKL3468.c
Bk9000_MBT_SRCS += devKL4002.c
Bk9000_MBT_SRCS += devKL4004.c
Bk9000_MBT_SRCS += devKL4132.c
Bk9000_MBT_SRCS += devKM1004.c
Bk9000_MBT_SRCS += devKM2004.c
Bk9000_MBT_SRCS += drvBk9000_MBTRegister.cpp

#Adding support for LCLS Analog Support 
DBD +=  analogSub.dbd
Bk9000_MBT_SRCS += analogIn.c

# The following builds sncExample as a component of Bk9000_MBT
# Also in Bk9000_MBTInclude.dbd uncomment #registrar(sncExampleRegistrar)
#Bk9000_MBT_SRCS += sncExample.stt
#Bk9000_MBT_LIBS += seq pv

# The PROD_IOC_vxWorks is built from the same srcs and must include
# the registry routines.
Bk9000_MBTLib_SRCS += $(Bk9000_MBT_SRCS)
Bk9000_MBTLib_SRCS += Bk9000_MBT_registerRecordDeviceDriver.cpp

#===========================

include $(TOP)/configure/RULES
#----------------------------------------
#  ADD RULES AFTER THIS LINE

