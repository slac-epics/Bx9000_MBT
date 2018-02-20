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
DB += EK9000.db
#Why is it not built automatically?
DB += EL5042.db
DB += EK9000Info.template
DB += EK9000InstalledBTs.template
DB += EL5042.template

#DB += EL5042-signal.template


# Also install substitutions files for easy access
DB += EK9000.substitutions
DB += EL5042.substitutions

#DB += EK9000Info-lcls.template
#DB += analog-conversion-lcls.template

#----------------------------------------------------
# If <anyname>.db template is not named <anyname>*.template add
# <anyname>_TEMPLATE = <templatename>

# Install Archive files
ARCHIVE += EK9000.tpl-arch
ARCHIVE += EL5042.tpl-arch

# Install Autosave templates
REQ += EK9000.tpl-req
REQ += EL5042.tpl-req


include $(TOP)/configure/RULES
#----------------------------------------
#  ADD RULES AFTER THIS LINE

