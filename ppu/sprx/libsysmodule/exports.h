#ifndef __EXPORTS_H__
#define __EXPORTS_H__


EXPORT(cellSysmoduleInitialize, 0x63ff6ff9);
EXPORT(cellSysmoduleFinalize, 0x96c07adf);

EXPORT(cellSysmoduleLoadModule, 0x32267A31);
EXPORT(cellSysmoduleUnloadModule, 0x112A5EE9);
EXPORT(cellSysmoduleIsLoaded, 0x5a59e258);

EXPORT(cellSysmoduleGetImagesize, 0x1ef115ef);
EXPORT(cellSysmoduleFetchImage, 0x3c92be09);
EXPORT(cellSysmoduleSetMemcontainer, 0xa193143c);


#endif
