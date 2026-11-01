#include <stdio.h>
#include <stdlib.h>
#include <ppu-asm.h>

#include <sysutil/msg.h>
#include <sysutil/sysutil.h>
#include <sysutil/save.h>
#include <sysutil/game.h>
#include <sysutil/disc.h>
#include <sysutil/video.h>

/* sysUtil functions */
extern s32 cellSysutilRegisterCallbackEx(s32 slot,opd32 *opd,void *usrdata);

/* cellMsgDialog functions */
extern s32 cellMsgDialogOpenEx(CellMsgDialogType type,const char *msg,opd32 *func,void *usrData,void *unused);
extern s32 cellMsgDialogOpen2Ex(CellMsgDialogType type,const char *msg,opd32 *func,void *usrData,void *unused);
extern s32 cellMsgDialogOpenErrorCodeEx(u32 errorCode,opd32 *func,void *usrData,void *unused);

/* CellSaveData functions */
extern s32 cellSaveDataListLoad2Ex (u32 version, CellSaveDataListSettings *listSettings,
    CellSaveDataBufferSettings *bufferSettings, opd32 *listCb, opd32 *statusCb,
    opd32 *fileCb, sys_mem_container_t container, void *user_data);
extern s32 cellSaveDataListSave2Ex (u32 version, CellSaveDataListSettings *listSettings,
    CellSaveDataBufferSettings *bufferSettings, opd32 *listCb, opd32 *statusCb,
    opd32 *fileCb, sys_mem_container_t container, void *user_data);
extern s32 cellSaveDataListAutoLoadEx (u32 version, CellSaveDataErrorDialogMode errorDialogMode,
    CellSaveDataListSettings *listSettings, CellSaveDataBufferSettings *bufferSettings,
    opd32 *fixedCb, opd32 *statusCb, opd32 *fileCb,
    sys_mem_container_t container,  void *user_data);
extern s32 cellSaveDataListAutoSaveEx (u32 version, CellSaveDataErrorDialogMode errorDialogMode,
    CellSaveDataListSettings *listSettings, CellSaveDataBufferSettings *bufferSettings,
    opd32 *fixedCb, opd32 *statusCb, opd32 *fileCb,
    sys_mem_container_t container, void *user_data);
extern s32 cellSaveDataFixedLoad2Ex (u32 version, CellSaveDataListSettings *listSettings,
    CellSaveDataBufferSettings *bufferSettings, opd32 *fixedCb, opd32 *statusCb,
    opd32 *fileCb, sys_mem_container_t container, void *user_data);
extern s32 cellSaveDataFixedSave2Ex (u32 version, CellSaveDataListSettings *listSettings,
    CellSaveDataBufferSettings *bufferSettings, opd32 *fixedCb, opd32 *statusCb,
    opd32 *fileCb, sys_mem_container_t container, void *user_data);
extern s32 cellSaveDataAutoLoad2Ex (s32 version, const char *directoryName,
    CellSaveDataErrorDialogMode errorDialogMode,
    CellSaveDataBufferSettings *bufferSettings, opd32 *statusCb, opd32 *fileCb,
    sys_mem_container_t container, void *user_data);
extern s32 cellSaveDataAutoSave2Ex (s32 version, const char *directoryName,
    CellSaveDataErrorDialogMode errorDialogMode,
    CellSaveDataBufferSettings *bufferSettings,
    opd32 *statusCb, opd32 *fileCb, sys_mem_container_t container, void *user_data);

/* sysGame functions */
extern s32 cellGameThemeInstallFromBufferEx(u32 fileSize, u32 bufSize, void *buf, opd32 *themeCb, u32 option);
extern s32 cellGameRegisterDiscChangeCallbackEx(CellGameDiscEjectCallback cbEject,CellGameDiscInsertCallback cbInsert);

/* Disc utility support */
extern s32 cellDiscGameRegisterDiscChangeCallbackEx(opd32 *cbEject,opd32 *cbInsert);

/* video system function */
extern s32 cellVideoOutRegisterCallbackEx(u32 slot, opd32 *cbVideo, void *userData);

/* cellSysutil wrapper functions */
s32 cellSysutilRegisterCallback(s32 slot,CellSysutilCallback cb,void *usrdata)
{
	return cellSysutilRegisterCallbackEx(slot, (opd32*)__get_opd32(cb),usrdata);
}

/* cellMsgDialog wraper functions */
s32 cellMsgDialogOpen(CellMsgDialogType type,const char *msg,CellMsgDialogCallback cb,void *usrData,void *unused)
{
	return cellMsgDialogOpenEx(type,msg,(opd32*)__get_opd32(cb),usrData,unused);
}

s32 cellMsgDialogOpen2(CellMsgDialogType type,const char *msg,CellMsgDialogCallback cb,void *usrData,void *unused)
{
	return cellMsgDialogOpen2Ex(type,msg,(opd32*)__get_opd32(cb),usrData,unused);
}

s32 cellMsgDialogOpenErrorCode(u32 errorCode,CellMsgDialogCallback cb,void *usrData,void *unused)
{
	return cellMsgDialogOpenErrorCodeEx(errorCode,(opd32*)__get_opd32(cb),usrData,unused);
}

/* CellSaveData wrapper functions */
s32 cellSaveDataListLoad2 (u32 version,
    CellSaveDataListSettings *listSettings,
    CellSaveDataBufferSettings *bufferSettings,
    CellSaveDataListCallback listCb,
    CellSaveDataStatusCallback statusCb,
    CellSaveDataFileCallback fileCb,
    sys_mem_container_t container,
    void *user_data)
{
  return cellSaveDataListLoad2Ex(version,
      listSettings, bufferSettings,
      (opd32*) __get_opd32(listCb),
      (opd32*) __get_opd32(statusCb),
      (opd32*) __get_opd32(fileCb),
      container, user_data);
}

s32 cellSaveDataListSave2 (u32 version,
    CellSaveDataListSettings *listSettings,
    CellSaveDataBufferSettings *bufferSettings,
    CellSaveDataListCallback listCb,
    CellSaveDataStatusCallback statusCb,
    CellSaveDataFileCallback fileCb,
    sys_mem_container_t container,
    void *user_data)
{
  return cellSaveDataListSave2Ex (version,
      listSettings, bufferSettings,
      (opd32*) __get_opd32(listCb),
      (opd32*) __get_opd32(statusCb),
      (opd32*) __get_opd32(fileCb),
      container, user_data);
}

s32 cellSaveDataListAutoLoad (u32 version,
    CellSaveDataErrorDialogMode errorDialogMode,
    CellSaveDataListSettings *listSettings,
    CellSaveDataBufferSettings *bufferSettings,
    CellSaveDataFixedCallback fixedCb,
    CellSaveDataStatusCallback statusCb,
    CellSaveDataFileCallback fileCb,
    sys_mem_container_t container,
    void *user_data)
{
  return cellSaveDataListAutoLoadEx (version,
      errorDialogMode,listSettings, bufferSettings,
      (opd32*) __get_opd32(fixedCb),
      (opd32*) __get_opd32(statusCb),
      (opd32*) __get_opd32(fileCb),
      container, user_data);
}

s32 cellSaveDataListAutoSave (u32 version,
    CellSaveDataErrorDialogMode errorDialogMode,
    CellSaveDataListSettings *listSettings,
    CellSaveDataBufferSettings *bufferSettings,
    CellSaveDataFixedCallback fixedCb,
    CellSaveDataStatusCallback statusCb,
    CellSaveDataFileCallback fileCb,
    sys_mem_container_t container,
    void *user_data)
{
  return cellSaveDataListAutoSaveEx (version,
      errorDialogMode, listSettings, bufferSettings,
      (opd32*) __get_opd32(fixedCb),
      (opd32*) __get_opd32(statusCb),
      (opd32*) __get_opd32(fileCb),
      container, user_data);
}

s32 cellSaveDataFixedLoad2 (u32 version,
    CellSaveDataListSettings *listSettings,
    CellSaveDataBufferSettings *bufferSettings,
    CellSaveDataFixedCallback fixedCb,
    CellSaveDataStatusCallback statusCb,
    CellSaveDataFileCallback fileCb,
    sys_mem_container_t container,
    void *user_data)
{
  return cellSaveDataFixedLoad2Ex (version,
      listSettings, bufferSettings,
      (opd32*) __get_opd32(fixedCb),
      (opd32*) __get_opd32(statusCb),
      (opd32*) __get_opd32(fileCb),
      container, user_data);
}

s32 cellSaveDataFixedSave2 (u32 version,
    CellSaveDataListSettings *listSettings,
    CellSaveDataBufferSettings *bufferSettings,
    CellSaveDataFixedCallback fixedCb,
    CellSaveDataStatusCallback statusCb,
    CellSaveDataFileCallback fileCb,
    sys_mem_container_t container,
    void *user_data)
{
  return cellSaveDataFixedSave2Ex (version,
      listSettings, bufferSettings,
      (opd32*) __get_opd32(fixedCb),
      (opd32*) __get_opd32(statusCb),
      (opd32*) __get_opd32(fileCb),
      container, user_data);
}

s32 cellSaveDataAutoLoad2 (s32 version,
    const char *directoryName,
    CellSaveDataErrorDialogMode errorDialogMode,
    CellSaveDataBufferSettings *bufferSettings,
    CellSaveDataStatusCallback statusCb,
    CellSaveDataFileCallback fileCb,
    sys_mem_container_t container,
    void *user_data)
{
  return cellSaveDataAutoLoad2Ex (version,
      directoryName, errorDialogMode, bufferSettings,
      (opd32*) __get_opd32(statusCb),
      (opd32*) __get_opd32(fileCb),
      container, user_data);
}

s32 cellSaveDataAutoSave2 (s32 version,
    const char *directoryName,
    CellSaveDataErrorDialogMode errorDialogMode,
    CellSaveDataBufferSettings *bufferSettings,
    CellSaveDataStatusCallback statusCb,
    CellSaveDataFileCallback fileCb,
    sys_mem_container_t container,
    void *user_data)
{
  return cellSaveDataAutoSave2Ex (version,
      directoryName, errorDialogMode, bufferSettings,
      (opd32*) __get_opd32(statusCb),
      (opd32*) __get_opd32(fileCb),
      container, user_data);
}

/* game utility support */
s32 cellGameThemeInstallFromBuffer(u32 fileSize, u32 bufSize, void *buf, CellGameThemeInstallCallback themeCb, u32 option)
{
	return cellGameThemeInstallFromBufferEx(fileSize,bufSize,buf,(opd32*)__get_opd32(themeCb),option);
}

s32 cellGameRegisterDiscChangeCallback(CellDiscGameEjectCallback cbEject,CellDiscGameInsertCallback cbInsert)
{
        return cellGameRegisterDiscChangeCallbackEx((opd32*)__get_opd32(cbEject),(opd32*)__get_opd32(cbInsert));
}

/* Disc utility support */
s32 cellDiscGameRegisterDiscChangeCallback(CellDiscGameEjectCallback cbEject,CellDiscGameInsertCallback cbInsert)
{
	return cellDiscGameRegisterDiscChangeCallbackEx((opd32*)__get_opd32(cbEject),(opd32*)__get_opd32(cbInsert));
}

/* video system functions */
s32 cellVideoOutRegisterCallback(u32 slot, CellVideoOutCallback cbVideo, void *userData)
{
    return cellVideoOutRegisterCallbackEx(slot, (opd32*)__get_opd32(cbVideo), userData);
}
