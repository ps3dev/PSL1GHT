#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include <audio/audio.h>

#include "tada48_16_2_raw.h"

static u64 snd_key;
static sys_event_queue_t snd_queue;

void fillBuffer(f32 *buf)
{
	u32 i;
	static u32 pos = 0;

	for(i=0;i<CELL_AUDIO_BLOCK_SAMPLES;i++) {
		buf[i*2 + 0] = (f32)*((s16*)&tada48_16_2_raw[pos])/32768.0f;
		buf[i*2 + 1] = (f32)*((s16*)&tada48_16_2_raw[pos + 2])/32768.0f;

		pos += 4;
		if(pos>=tada48_16_2_raw_size) pos = 0;
	}
}

void playOneBlock(CellAudioPortConfig *config)
{
	f32 *buf;
	s32 ret = 0;
	sys_event_t event;
	u64 current_block = *(u64*)((u64)config->readIndex);
	f32 *dataStart = (f32*)((u64)config->audioDataStart);
	u32 audio_block_index = (current_block + 1)%config->numBlocks;

	ret = sysEventQueueReceive(snd_queue,&event,20*1000);

	buf = dataStart + config->channelCount*CELL_AUDIO_BLOCK_SAMPLES*audio_block_index;
	fillBuffer(buf);
}

int main(int argc,char *argv[])
{
	u32 portNum,i;
	CellAudioPortParam params;
	CellAudioPortConfig config;

	s32 ret = cellAudioInit();

	printf("cellAudioInit: %08x\n",ret);

	params.numChannels = CELL_AUDIO_PORT_2CH;
	params.numBlocks = CELL_AUDIO_BLOCK_8;
	params.attrib = CELL_AUDIO_PORT_INITLEVEL;
	params.level = 1.0f;
	ret = cellAudioPortOpen(&params,&portNum);
	printf("audioPortOpen: %08x\n",ret);
	printf("  portNum: %d\n",portNum);

	ret = cellAudioGetPortConfig(portNum,&config);
	printf("audioGetPortConfig: %08x\n",ret);
	printf("config.readIndex: %08x\n",config.readIndex);
	printf("config.status: %d\n",config.status);
	printf("config.channelCount: %ld\n",config.channelCount);
	printf("config.numBlocks: %ld\n",config.numBlocks);
	printf("config.portSize: %d\n",config.portSize);
	printf("config.audioDataStart: %08x\n",config.audioDataStart);

	ret = cellAudioCreateNotifyEventQueue(&snd_queue,&snd_key);
	printf("audioCreateNotifyEventQueue: %08x\n",ret);
	printf("snd_queue: %16lx\n",(long unsigned int)snd_queue);
	printf("snd_key: %16lx\n",snd_key);

	ret = cellAudioSetNotifyEventQueue(snd_key);
	printf("cellAudioSetNotifyEventQueue: %08x\n",ret);

	ret = sysEventQueueDrain(snd_queue);
	printf("sysEventQueueDrain: %08x\n",ret);

	ret = cellAudioPortStart(portNum);
	printf("cellAudioPortStart: %08x\n",ret);

	i = 0;
	while(i<1000) {
		playOneBlock(&config);
			i++;
	}

	ret = cellAudioPortStop(portNum);
	printf("cellAudioPortStop: %08x\n",ret);

	ret = cellAudioRemoveNotifyEventQueue(snd_key);
	printf("cellAudioRemoveNotifyEventQueue: %08x\n",ret);

	ret = cellAudioPortClose(portNum);
	printf("cellAudioPortClose: %08x\n",ret);

	ret = sysEventQueueDestroy(snd_queue,0);
	printf("sysEventQueueDestroy: %08x\n",ret);

	ret = cellAudioQuit();
	printf("cellAudioQuit: %08x\n",ret);

	return 0;
}
