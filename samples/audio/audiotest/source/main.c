/* VideoInfo:
 * This example querys the PS3 for its current reslution and prints it to tty.
 * You will need something that redirects tty to see the output.
 */

#include <audio/audio.h>
#include <math.h>
#include <assert.h>
#include <stdio.h>


#define PI 3.14159265f

void fillBuffer(float *buf)
{
	static float pos=0;

	for (unsigned int i = 0; i < AUDIO_BLOCK_SAMPLES; ++i)
	{
		//just fill with a beautiful sine wave :P
		buf[i*2+0] = sin(pos);
		//in fact 2 different ones
		buf[i*2+1] = sin(pos*2);
		pos+=0.01f;
		if(pos>M_PI)
			pos-=2*M_PI;
	}
}

u32 playOneBlock(u64 *readIndex, float *audioDataStart)
{
	static u32 audio_block_index=1;

	//get position of the hardware
	u64 current_block = *readIndex;

	//try to stay ahead of it
	if(audio_block_index == current_block)
	{
		return 0;
	}
	printf("playOneBlock: %ld, %d\n",current_block,audio_block_index);

	//get position of the block to write
	float *buf = audioDataStart + 2 /*channelcount*/ * AUDIO_BLOCK_SAMPLES * audio_block_index;
	fillBuffer(buf);

	audio_block_index = (audio_block_index + 1) % AUDIO_BLOCK_8;

	return 1;
}

int main(int argc, const char* argv[])
{
	audioPortParam params;
	audioPortConfig config;
	u32 portNum;

	//initialize the audio system
	int ret=audioInit();

	printf("audioInit: %d\n",ret);

	//set some parameters we want
	//either 2 or 8 channel
	params.numChannels = AUDIO_PORT_2CH;
	//8 16 or 32 block buffer
	params.numBlocks = AUDIO_BLOCK_8;
	//extended attributes
	params.attrib = 0;
	//sound level (1 is default)
	params.level = 1;

	//open the port (still stopped)
	ret=audioPortOpen(&params, &portNum);
	printf("audioPortOpen: %d\n",ret);
	printf("  portNum: %d\n",portNum);

	//get the params for the buffers, etc
	ret=audioGetPortConfig(portNum, &config);
	printf("audioGetPortConfig: %d\n",ret);
	printf("  readIndex: 0x%8X\n",config.readIndex);
	printf("  status: %d\n",config.status);
	printf("  channelCount: %ld\n",config.channelCount);
	printf("  numBlocks: %ld\n",config.numBlocks);
	printf("  portSize: %d\n",config.portSize);
	printf("  audioDataStart: 0x%8X\n",config.audioDataStart);

	//start the loop
	ret=audioPortStart(portNum);
	printf("audioPortStart: %d\n",ret);

	int i=0;
	while(i<1000)
	{
		if(playOneBlock((u64*)(u64)config.readIndex,(float*)(u64)config.audioDataStart)!=0)
			i++;
	}

	//shutdown in reverse order
	ret=audioPortStop(portNum);
	printf("audioPortStop: %d\n",ret);
	ret=audioPortClose(portNum);
	printf("audioPortClose: %d\n",ret);
	ret=audioQuit();
	printf("audioQuit: %d\n",ret);

	return 0;
}
