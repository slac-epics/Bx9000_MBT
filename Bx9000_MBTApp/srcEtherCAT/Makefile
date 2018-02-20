TOP=../..

include $(TOP)/configure/CONFIG
#----------------------------------------
#  ADD MACRO DEFINITIONS AFTER THIS LINE
#=============================
#==================================================
# The following will built and instaled:
# dbd/EK9000_MBT.dbd
# include/EK9000_MBT.h
# bin/<arch>/EK9000_MBTLib.munch
# lib/<arch>/libEK9000_MBT.a
#=============================
# build an ioc application

# This is what we will be building from here:
# create library for OSI type driver
LIBRARY_IOC += EK9000_MBT

# This is for vxWorks specific drivers.
# Don't forget to substitute your driver name.
# create library for linking will install in <lib>
# LIBRARY_vxWorks += EK9000_MBT

INC += EK9000_Watchdog.h

# The following line makes the EK9000_MBT.h file
# This is needed for Record support. So uncomment
# the following line if you are making record support.
##DBDINC += EK9000_MBT
# EK9000_MBT.dbd will be created from EK9000_MBTInclude.dbd
DBD += EK9000_MBT.dbd

# Add locally compiled object code
EK9000_MBT_SRCS += EK9000_MBT_Ext.c
EK9000_MBT_SRCS += EK9000_Watchdog.cpp
EK9000_MBT_SRCS += drvEK9000_MBT.c
#EK9000_MBT_SRCS += devEK9000Info.c
EK9000_MBT_SRCS += devEL5042.c
EK9000_MBT_SRCS += drvEK9000_MBTRegister.cpp

#Adding support for LCLS Analog Support 
#DBD +=  analogSub.dbd
#EK9000_MBT_SRCS += analogIn.c

# The following builds sncExample as a component of EK9000_MBT
# Also in EK9000_MBTInclude.dbd uncomment #registrar(sncExampleRegistrar)
#EK9000_MBT_SRCS += sncExample.stt
#EK9000_MBT_LIBS += seq pv

# The PROD_IOC_vxWorks is built from the same srcs and must include
# the registry routines.
EK9000_MBTLib_SRCS += $(EK9000_MBT_SRCS)
EK9000_MBTLib_SRCS += EK9000_MBT_registerRecordDeviceDriver.cpp

#===========================

include $(TOP)/configure/RULES
#----------------------------------------
#  ADD RULES AFTER THIS LINE

