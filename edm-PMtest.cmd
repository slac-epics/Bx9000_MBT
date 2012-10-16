#! /bin/bash

export EDMDATAFILES="$EDMDATAFILES:.:..:Bx9000_MBTApp/srcDisplay"

edm -x -m "apc=LCLS:PM:BHC:TEST" -eolc Bx9000_MBTApp/srcDisplay/Bx9000Info-lcls.edl Bx9000_MBTApp/srcDisplay/Bx9000InstalledBTs-lcls.edl &
edm -x -m "DOUT=TEST:PM:DOUT:,LOC='B34 racks'" -eolc Bx9000_MBTApp/srcDisplay/do.edl &
edm -x -m "DIN=TEST:PM:DIN:,LOC=B34 racks" -eolc Bx9000_MBTApp/srcDisplay/di.edl &
