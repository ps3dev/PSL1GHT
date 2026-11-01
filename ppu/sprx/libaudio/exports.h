#ifndef __EXPORTS_H__
#define __EXPORTS_H__

EXPORT(cellAudioInit, 0x0b168f92);
EXPORT(cellAudioPortClose, 0x4129fe2d);
EXPORT(cellAudioPortStop, 0x5b1e2c73);
EXPORT(cellAudioGetPortConfig, 0x74a66af0);
EXPORT(cellAudioPortStart, 0x89be28f2);
EXPORT(cellAudioQuit, 0xca5ac370);
EXPORT(cellAudioPortOpen, 0xcd7bc431);
EXPORT(cellAudioSetPortLevel, 0x56dfe179);
EXPORT(cellAudioCreateNotifyEventQueue, 0x04af134e);
EXPORT(cellAudioMiscSetAccessoryVolume, 0x31211f6b);
EXPORT(cellAudioSetNotifyEventQueue, 0x377e0cd9);
EXPORT(cellAudioGetPortTimestamp, 0x4109d08c);
EXPORT(cellAudioAdd2chData, 0x9e4b1db8);
EXPORT(cellAudioAddData, 0xdab029aa);
EXPORT(cellAudioGetPortBlockTag, 0xe4046afe);
EXPORT(cellAudioRemoveNotifyEventQueue, 0xff3626fd);

#endif
