#ifndef __SYS_USBD_H__
#define __SYS_USBD_H__

#include <ppu-lv2.h>

#ifdef __cplusplus
	extern "C" {
#endif

LV2_SYSCALL sysUsbdInitialize(u32 *initVal)
{
	lv2syscall1(530, initVal);
	return_to_user_prog(s32);
}
LV2_SYSCALL sysUsbdGetDeviceList(u32 handle, void *device_list, char unknown)
{
	lv2syscall3(532, handle, device_list, unknown);
	return_to_user_prog(s32);
}
LV2_SYSCALL sysUsbdGetDescriptorSize(u32 handle, u8 deviceNumber)
{
	lv2syscall2(533, handle, unk1);
	return_to_user_prog(s32);
}

LV2_SYSCALL sysUsbdGetDescriptor(u32 handle, u8 deviceNumber, void *descriptor, s64 descSize)
{
	lv2syscall4(534, handle, unk1, descriptor, descSize);
	return_to_user_prog(s32);
}

LV2_SYSCALL sysUsbdReceiveEvent(u32 handle, u64 *unk1, u64 *unk2, u64 *unk3)
{
	lv2syscall4(540, handle, unk1, unk2, unk3);
	return_to_user_prog(s32);
}

LV2_SYSCALL sysUsbdRegisterExtraLdd(u32 handle, void *lddOps, u16 strLen, u16 vendorID, u16 productID, u16 unk1)
{
	lv2syscall6(559, handle, lddOps, strLen, vendorID, productID, unk1);
	return_to_user_prog(s32);
}

#ifdef __cplusplus
	}
#endif

#endif
