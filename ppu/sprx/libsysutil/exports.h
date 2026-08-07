#ifndef __EXPORTS_H__
#define __EXPORTS_H__

/* video support */
EXPORT(cellVideoOutGetResolution, 0xe558748d);
EXPORT(cellVideoOutConfigure, 0x0bae8772);
EXPORT(cellVideoOutGetState, 0x887572d5);
EXPORT(cellVideoOutGetDeviceInfo, 0x1e930eef);
EXPORT(cellVideoOutGetConfiguration, 0x15b0b0cd);
EXPORT(cellVideoOutGetResolutionAvailability, 0xa322db75);
EXPORT(cellVideoOutDebugSetMonitorType, 0xcfdf24bb);
EXPORT(cellVideoOutRegisterCallback, 0x8e8bc444);
EXPORT(cellVideoOutUnregisterCallback, 0x7871bed4);
EXPORT(cellVideoOutGetNumberOfDevice, 0x75bbb672);
EXPORT(cellVideoOutGetConvertCursorColorInfo, 0x55e425c3);

/* message dialog support */
EXPORT(cellMsgDialogClose, 0x20543730);
EXPORT(cellMsgDialogOpenErrorCodeEx, 0x3e22cb4b);
EXPORT(cellMsgDialogOpenEx, 0xf81eca25);
EXPORT(cellMsgDialogProgressBarInc, 0x94862702);
EXPORT(cellMsgDialogAbort, 0x62b0f803);
EXPORT(cellMsgDialogOpen2Ex, 0x7603d3db);
EXPORT(cellMsgDialogProgressBarReset, 0x7bc2c8a8);
EXPORT(cellMsgDialogProgressBarSetMsg, 0x9d6af72a);

/* on-screen-keyboard support */
EXPORT(cellOskDialogGetInputText, 0x1d99c3ee);
EXPORT(cellOskDialogSetInitialInputDevice, 0xf3b4b43e);
EXPORT(cellOskDialogGetSize, 0x35beade0);
EXPORT(cellOskDialogUnloadAsync, 0x3d1e1931);
EXPORT(cellOskDialogDisableDimmer, 0x41f20828);
EXPORT(cellOskDialogSetKeyLayoutOption, 0xb53c54fa);
EXPORT(cellOskDialogAbort, 0xb6d84526);
EXPORT(cellOskDialogSetDeviceMask, 0xd3790a86);
EXPORT(cellOskDialogSetSeparateWindowOption, 0x53e39df3);
EXPORT(cellOskDialogAddSupportLanguage, 0x7f21c918);
EXPORT(cellOskDialogLoadAsync, 0x7fcfc915);
EXPORT(cellOskDialogSetInitialKeyLayout, 0xc427890c);
EXPORT(cellOskDialogSetLayoutMode, 0xf0ec3ccc);

/* system utility support */
EXPORT(cellSysutilRegisterCallbackEx, 0x9d98afa0);
EXPORT(cellSysutilCheckCallback, 0x189a74da);
EXPORT(cellSysutilUnregisterCallback, 0x02ff3c1b);
EXPORT(cellSysutilGetSystemParamInt, 0x40e895d3);
EXPORT(cellSysutilGetSystemParamString, 0x938013a0);

/* Save utility support */
EXPORT(cellSaveDataListLoad2Ex, 0x1dfbfdd6);
EXPORT(cellSaveDataListSave2Ex, 0x2de0d663);
EXPORT(cellSaveDataListAutoLoadEx, 0x21425307);
EXPORT(cellSaveDataListAutoSaveEx, 0x4dd03a4e);
EXPORT(cellSaveDataFixedLoad2Ex, 0x2a8eada2);
EXPORT(cellSaveDataFixedSave2Ex, 0x2aae9ef5);
EXPORT(cellSaveDataAutoLoad2Ex, 0xfbd5c856);
EXPORT(cellSaveDataAutoSave2Ex, 0x8b7ed64b);
EXPORT(cellSaveDataDelete2, 0xedadd797);

/* Game Content utility support */
EXPORT(cellGameDataCheck, 0xdb9819f3);
EXPORT(cellGameBootCheck, 0xf52639ea);
EXPORT(cellGamePatchCheck, 0xce4374f6);
EXPORT(cellGameCreateGameData, 0x42a2e133);
EXPORT(cellGameGetParamInt, 0xb7a45caf);
EXPORT(cellGameGetParamString, 0x3a5d726a);
EXPORT(cellGameGetSizeKB, 0xef9d42d5);
EXPORT(cellGameSetParamString, 0xdaa5cd20);
EXPORT(cellGameGetDiscContentInfoUpdatePath, 0x2a8e6b92);
EXPORT(cellGameContentPermit, 0x70acec67);
EXPORT(cellGameContentErrorDialog, 0xb0a1f8c6);
EXPORT(cellGameThemeInstall, 0xd24e3928);
EXPORT(cellGameThemeInstallFromBufferEx, 0x87406734);
EXPORT(cellGameGetLocalWebContentPath, 0xa80bf223);
EXPORT(cellGameDeleteGameData, 0xb367c6e3);
EXPORT(cellGameRegisterDiscChangeCallbackEx, 0xf8115d69);
EXPORT(cellGameUnregisterDiscChangeCallback, 0x21fc0c71);

/* Disc utility support */
EXPORT(cellDiscGameRegisterDiscChangeCallbackEx, 0xdfdd302e);
EXPORT(cellDiscGameUnregisterDiscChangeCallback, 0xedc34e1a);

/* trophy utility support */
EXPORT(sceNpTrophyInit, 0x39567781);
EXPORT(sceNpTrophyAbortHandle, 0x48bd97c7);
EXPORT(sceNpTrophyCreateContext, 0xe3bf9a28);
EXPORT(sceNpTrophyCreateHandle, 0x1c25470d);
EXPORT(sceNpTrophyDestroyContext, 0x3741ecc7);
EXPORT(sceNpTrophyDestroyHandle, 0x623cd2dc);
EXPORT(sceNpTrophyGetGameIcon, 0xff299e03);
EXPORT(sceNpTrophyGetGameInfo, 0x49d18217);
EXPORT(sceNpTrophyGetGameProgress, 0x079f0e87);
EXPORT(sceNpTrophyGetRequiredDiskSpace, 0x370136fe);
EXPORT(sceNpTrophyGetTrophyIcon, 0xbaedf689);
EXPORT(sceNpTrophyGetTrophyInfo, 0xfce6d30a);
EXPORT(sceNpTrophyGetTrophyUnlockState, 0xb3ac3478);
EXPORT(sceNpTrophyRegisterContextEx, 0x1197b52c);
EXPORT(sceNpTrophySetSoundLevel, 0x27deda93);
EXPORT(sceNpTrophyTerm, 0xa7fabf4d);
EXPORT(sceNpTrophyUnlockTrophy, 0x8ceedd21);

#endif
