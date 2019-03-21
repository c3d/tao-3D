# Define useful variables for modules

isEmpty(TAO_SDK) {
  TAOTOPSRC = $$PWD/..
} else {
  TAOTOPSRC = $${TAO_SDK}
}

include(../main_defs.pri)

!exists(../tao.pro):IS_SDK=true

# Try to install into (in this order, depending on what variable is defined):
# 1. $$MODINSTPATH
# 2. $$MODINSTROOT/$$MODINSTDIR
# 3. <Tao application install dir, or user's dir>/$$MODINSTDIR
isEmpty(MODINSTPATH) {
  isEmpty(MODINSTROOT) {
    equals(IS_SDK, true) {
      # We don't have the full source, only the module SDK
      # In this case we want "make install" to install modules
      # directly into the user's module directory
      win32 {
        LOCALAPPDATA=$$system(bash -c \"cd $LOCALAPPDATA ; pwd\") # convert Win path to Mingw path
        MODINSTROOT = "$${LOCALAPPDATA}/Taodyne/Tao3D/modules"
      }
      macx:MODINSTROOT = $$(HOME)"/Library/Application Support/Taodyne/Tao3D/modules"
      linux*:MODINSTROOT = $$(HOME)"/.local/share/data/Taodyne/Tao3D/modules"
      isEmpty(MODINSTROOT):error(MODINSTROOT not defined)
    } else {
      # Building full Tao including its bundled modules
      isEmpty(APPINST):error(APPINST not defined)
      MODINSTROOT = $${APPINST}/modules
    }
  }
  isEmpty(MODINSTDIR):error(MODINSTDIR not defined)
  MODINSTPATH      = $${MODINSTROOT}/$$MODINSTDIR
}

isEmpty(FIX_QT_REFS):FIX_QT_REFS = $$PWD/fix_qt_refs
