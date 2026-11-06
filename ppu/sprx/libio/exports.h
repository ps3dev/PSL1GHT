#ifndef __EXPORTS_H__
#define __EXPORTS_H__

EXPORT(sys_config_start, 0x068fcbc6);
EXPORT(sys_config_stop, 0x6d367953);
EXPORT(sys_config_add_service_listener, 0x6ae10596);
EXPORT(sys_config_remove_service_listener, 0xf5d9d571);
EXPORT(sys_config_register_service, 0x78f058a2);
EXPORT(sys_config_unregister_service, 0x5f81900c);

/* Pad */
EXPORT(cellPadInit, 0x1cf98800);
EXPORT(cellPadEnd, 0x4d9b75d5);
EXPORT(cellPadClearBuf, 0x0d5f2c14);
EXPORT(cellPadInfoPressMode, 0x0e2dfaad);
EXPORT(cellPadLddRegisterController, 0x20a97ba2);
EXPORT(cellPadGetInfo, 0x3aaad464);
EXPORT(cellPadGetRawData, 0x3f797dff);
EXPORT(cellPadGetDataExtra, 0x6bc09c61);
EXPORT(cellPadInfoSensorMode, 0x78200559);
EXPORT(cellPadGetData, 0x8b72cda1);
EXPORT(cellPadLddGetPortNo, 0x8b8231e5);
EXPORT(cellPadLddDataInsert, 0xbafd6409);
EXPORT(cellPadSetSensorMode, 0xbe5be3ba);
EXPORT(cellPadGetCapabilityInfo, 0xdbf4c59c);
EXPORT(cellPadLddUnregisterController, 0xe442faa8);
EXPORT(cellPadSetActDirect, 0xf65544ee);
EXPORT(cellPadSetPressMode, 0xf83f8182);

EXPORT(cellPadPeriphGetInfo, 0x4cc9b68d);
EXPORT(cellPadSetPortSetting, 0x578e3c98);
EXPORT(cellPadPeriphGetData, 0x8a00f264);
EXPORT(cellPadGetInfo2, 0xa703a51d);

/* Mouse */
EXPORT(cellMouseInit, 0xc9030138);
EXPORT(cellMouseEnd, 0xe10183ce);
EXPORT(cellMouseClearBuf, 0x3ef66b95);
EXPORT(cellMouseGetInfo, 0x5baf30fb);
EXPORT(cellMouseGetTabletDataList, 0x21a62e9b);
EXPORT(cellMouseSetTabletMode, 0x2d16da4f);
EXPORT(cellMouseGetData, 0x3138e632);
EXPORT(cellMouseInfoTabletMode, 0x4d0b3b1f);
EXPORT(cellMouseGetRawData, 0xa328cc35);
EXPORT(cellMouseGetDataList, 0x6bd131f0);

/* Keyboard */
EXPORT(cellKbInit, 0x433f6ec0);
EXPORT(cellKbEnd, 0xbfce3285);
EXPORT(cellKbRead, 0xff0a21b7);
EXPORT(cellKbSetReadMode, 0xdeefdfa7);
EXPORT(cellKbSetCodeType, 0xa5f85e4d);
EXPORT(cellKbCnvRawCode, 0x4ab1fa77);
EXPORT(cellKbSetLEDStatus, 0x3f72c56e);
EXPORT(cellKbGetInfo, 0x2f1774d5);
EXPORT(cellKbGetConfiguration, 0x1f71ecbe);
EXPORT(cellKbClearBuf, 0x2073b7f6);

#endif
