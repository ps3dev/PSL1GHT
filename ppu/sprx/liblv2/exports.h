#ifndef __EXPORTS_H__
#define __EXPORTS_H__

/* processes */
EXPORT(sysProcessExit, 0xe6f2c1e7);
EXPORT(sysProcessExitSpawn2, 0x67f9fedb);
EXPORT(sysProcessExitSpawnWithLevel, 0xa2c7ba64);

/* threading */
EXPORT(sysThreadCreateEx, 0x24a1ea07);
EXPORT(sysThreadExit, 0xaff080a4);
EXPORT(sysThreadGetId, 0x350d454e);
EXPORT(sysThreadOnce, 0xa3e3be68);
EXPORT(sysThreadInitialzeTLS, 0x744680a2);

/* spu handling */
EXPORT(sysSpuElfGetSegments, 0xdb6b3250);
EXPORT(sysSpuElfGetInformation, 0x1ed454ce);
EXPORT(sysSpuRawLoad, 0x893305fa);
EXPORT(sysSpuRawImageLoad, 0xb995662e);
EXPORT(sysSpuImageClose, 0xe0da8efd);
EXPORT(sysSpuImageImport, 0xebe5f72f);

/* mutex handling */
EXPORT(sysLwMutexCreate, 0x2f85c0ef);
EXPORT(sysLwMutexDestroy, 0xc3476d0c);
EXPORT(sysLwMutexLock, 0x1573dc3f);
EXPORT(sysLwMutexTryLock, 0xaeb78725);
EXPORT(sysLwMutexUnlock, 0x1bc200f4);

/* cond variable handling */
EXPORT(sysLwCondCreate, 0xda0eb71a);
EXPORT(sysLwCondWait, 0x2a6d9d51);
EXPORT(sysLwCondSignal, 0xef87a695);
EXPORT(sysLwCondSignalTo, 0x52aadadf);
EXPORT(sysLwCondSignalAll, 0xe9a1bd84);
EXPORT(sysLwCondDestroy, 0x1c9a942c);

/* time handling */
EXPORT(sysGetSystemTime, 0x8461e528);

/* memory management */
EXPORT(sysMMapperFreeMemory, 0x409ad939);
EXPORT(sysMMapperMapMemory, 0xdc578057);
EXPORT(sysMMapperUnmapMemory, 0x4643ba6e);
EXPORT(sysMMapperAllocateMemory, 0xb257540b);
EXPORT(sysMMapperAllocateMemoryFromContainer, 0x70258515);

/* spinlock handling */
EXPORT(sysSpinlockInitialize, 0x8c2bb498);
EXPORT(sysSpinlockLock, 0xa285139d);
EXPORT(sysSpinlockTryLock, 0x722a0254);
EXPORT(sysSpinlockUnlock, 0x5267cb35);

/* thread interupt handling */
EXPORT(sysInterruptThreadDisestablish, 0x4a071d98);

#endif
