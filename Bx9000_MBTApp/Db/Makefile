TOP=../..
include $(TOP)/configure/CONFIG
#----------------------------------------
#  ADD MACRO DEFINITIONS AFTER THIS LINE

#----------------------------------------------------
#  Optimization of db files using dbst (DEFAULT: NO)
#DB_OPT = YES

#----------------------------------------------------
# Create and install (or just install)
# databases, templates, substitutions like this
DB += Bx9000Info.template
DB += Bx9000InstalledBTs.template
DB += KL1104.template
DB += KL1002.template
DB += KL1124.template
DB += KL1408.template
DB += KL2114.template
DB += KL2124.template
DB += KL2408.template
DB += KL2808.template
DB += KL2502.template
DB += KL2512.template
DB += KL2622.template
DB += KL3064.template
DB += KL3102.template
DB += KL3202.template
DB += KL3204.template
DB += KL3222.template
DB += KL3312.template
DB += KL3314.template
DB += KL3408.template
DB += KL3468.template
DB += KL4002.template
DB += KL4132.template
# Use soft linked tpl-db files to install
# both .template and .tpl-db variants
DB += KL3314.tpl-db
DB += KL4132.tpl-db


# Also install substitutions files for easy access
DB += Bx9000.substitutions
DB += KL1104.substitutions
DB += KL1002.substitutions
DB += KL1124.substitutions
DB += KL1408.substitutions
DB += KL2114.substitutions
DB += KL2124.substitutions
DB += KL2408.substitutions
DB += KL2808.substitutions
DB += KL2502.substitutions
DB += KL2512.substitutions
DB += KL2622.substitutions
DB += KL3064.substitutions
DB += KL3102.substitutions
DB += KL3202.substitutions
DB += KL3204.substitutions
DB += KL3222.substitutions
DB += KL3312.substitutions
DB += KL3314.substitutions
DB += KL3408.substitutions
DB += KL3468.substitutions
DB += KL4002.substitutions
DB += KL4132.substitutions

# Use soft linked tpl-db files to install
# both .template and .tpl-db variants
DB += KL3314.sub-db
DB += KL4132.sub-db

#----------------------------------------------------
# If <anyname>.db template is not named <anyname>*.template add
# <anyname>_TEMPLATE = <templatename>

# Install Archive files
#ARCHIVE += Bx9000.archive
#ARCHIVE += KL3314-signal.archive
#ARCHIVE += KL4132-signal.archive

# Install Archive templates
ARCHIVE += Bx9000.tpl-arch
ARCHIVE += KL2502-signal.tpl-arch
ARCHIVE += KL2512-signal.tpl-arch
ARCHIVE += KL3064-signal.tpl-arch
ARCHIVE += KL3102-signal.tpl-arch
ARCHIVE += KL3202-signal.tpl-arch
ARCHIVE += KL3312-signal.tpl-arch
ARCHIVE += KL3314-signal.tpl-arch
ARCHIVE += KL3408-signal.tpl-arch
ARCHIVE += KL4002-signal.tpl-arch
ARCHIVE += KL4132-signal.tpl-arch

# Install Autosave templates
REQ += Bx9000.tpl-req
REQ += KL2502-signal.tpl-req
REQ += KL2512-signal.tpl-req
REQ += KL3064-signal.tpl-req
REQ += KL3102-signal.tpl-req
REQ += KL3202-signal.tpl-req
REQ += KL3312-signal.tpl-req
REQ += KL3314-signal.tpl-req
REQ += KL3408-signal.tpl-req
REQ += KL4002-signal.tpl-req
REQ += KL4132-signal.tpl-req

include $(TOP)/configure/RULES
#----------------------------------------
#  ADD RULES AFTER THIS LINE

