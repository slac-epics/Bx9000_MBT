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

# This is what we will be building from here:
# create library for OSI type driver
LIBRARY_IOC += Bk9000_MBT

# This is for vxWorks specific drivers.
# Don't forget to substitute your driver name.
# create library for linking will install in <lib>
# LIBRARY_vxWorks += Bk9000_MBT

INC += Bk9000_Watchdog.h

# The following line makes the Bk9000_MBT.h file
# This is needed for Record support. So uncomment
# the following line if you are making record support.
##DBDINC += Bk9000_MBT
# Bk9000_MBT.dbd will be created from Bk9000_MBTInclude.dbd
DBD += Bk9000_MBT.dbd

# Add locally compiled object code
Bk9000_MBT_SRCS += EK9000_MBT_Ext.c
Bk9000_MBT_SRCS += EK9000_Watchdog.cpp
Bk9000_MBT_SRCS += drvEK9000_MBT.c
Bk9000_MBT_SRCS += devEK9000Info.c
Bk9000_MBT_SRCS += devEL5042.c
Bk9000_MBT_SRCS += drvEK9000_MBTRegister.cpp

#Adding support for LCLS Analog Support 
DBD +=  analogSub.dbd
Bk9000_MBT_SRCS += analogIn.c

# The following builds sncExample as a component of Bk9000_MBT
# Also in Bk9000_MBTInclude.dbd uncomment #registrar(sncExampleRegistrar)
#Bk9000_MBT_SRCS += sncExample.stt
#Bk9000_MBT_LIBS += seq pv

# The PROD_IOC_vxWorks is built from the same srcs and must include
# the registry routines.
#Bk9000_MBTLib_SRCS += $(Bk9000_MBT_SRCS)
Bk9000_MBTLib_SRCS += EK9000_MBT_registerRecordDeviceDriver.cpp

#===========================

include $(TOP)/configure/RULES
#----------------------------------------
#  ADD RULES AFTER THIS LINE

