#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include <malloc.h>
#include <unistd.h>

#include <io/pad.h>

#include <sys/memory.h>
#include <sysutil/osk.h>
#include <sysutil/sysutil.h>

#include "rsxutil.h"

static vs32 dialog_action = 0;

uint8_t isRunningOSK = 0;
oskInputFieldInfo inputFieldInfo;
oskParam parameters;
oskCallbackReturnParam outputParam;

static void utf16_to_utf8(const uint16_t *src, uint8_t *dst)
{
    int i;
    for (i = 0; src[i]; i++)
    {
        if ((src[i] & 0xFF80) == 0)
        {
            *(dst++) = src[i] & 0xFF;
        }
        else if((src[i] & 0xF800) == 0)
        {
            *(dst++) = ((src[i] >> 6) & 0xFF) | 0xC0;
            *(dst++) = (src[i] & 0x3F) | 0x80;
        }
        else if((src[i] & 0xFC00) == 0xD800 && (src[i + 1] & 0xFC00) == 0xDC00)
        {
            *(dst++) = (((src[i] + 64) >> 8) & 0x3) | 0xF0;
            *(dst++) = (((src[i] >> 2) + 16) & 0x3F) | 0x80;
            *(dst++) = ((src[i] >> 4) & 0x30) | 0x80 | ((src[i + 1] << 2) & 0xF);
            *(dst++) = (src[i + 1] & 0x3F) | 0x80;
            i += 1;
        }
        else
        {
            *(dst++) = ((src[i] >> 12) & 0xF) | 0xE0;
            *(dst++) = ((src[i] >> 6) & 0x3F) | 0x80;
            *(dst++) = (src[i] & 0x3F) | 0x80;
        }
    }

    *dst = '\0';
}

static void utf8_to_utf16(const uint8_t *src, uint16_t *dst)
{
    int i;
    for (i = 0; src[i];)
    {
        if ((src[i] & 0xE0) == 0xE0)
        {
            *(dst++) = ((src[i] & 0x0F) << 12) | ((src[i + 1] & 0x3F) << 6) | (src[i + 2] & 0x3F);
            i += 3;
        }
        else if ((src[i] & 0xC0) == 0xC0)
        {
            *(dst++) = ((src[i] & 0x1F) << 6) | (src[i + 1] & 0x3F);
            i += 2;
        }
        else
        {
            *(dst++) = src[i];
            i += 1;
        }
    }

    *dst = '\0';
}

static void do_flip()
{
	sysUtilCheckCallback();
	flip();
}

void program_exit_callback()
{
	gcmSetWaitFlip(context);
	rsxFinish(context, 1);
}

void sysutil_exit_callback(u64 status, u64 param, void *usrdata)
{
	switch(status) {
		case SYSUTIL_EXIT_GAME:
			break;
		case SYSUTIL_DRAW_BEGIN:
		case SYSUTIL_DRAW_END:
			break;
		case SYSUTIL_OSK_LOADED:
			printf("OSK loaded\n");
			break;
		case SYSUTIL_OSK_INPUT_CANCELED:
			printf("OSK input canceled\n");
			oskAbort();
			// fall-through
		case SYSUTIL_OSK_DONE:
			if (status == SYSUTIL_OSK_DONE)
			{
				printf("OSK done\n");
			}
			oskUnloadAsync(&outputParam);

			if (outputParam.res == OSK_OK)
			{
				printf("OSK result OK\n");
			}
			else
			{
				printf("OKS result: %d\n", outputParam.res);
			}

			break;
		case SYSUTIL_OSK_UNLOADED:
			printf("OSK unloaded\n");
			isRunningOSK = 0;
			break;
		default:
			break;
	}
}

#define TEXT_BUFFER_LENGTH 256

int main(int argc,char *argv[])
{
	void *host_addr = memalign(1024*1024, HOST_SIZE);
    static uint16_t title_utf16[TEXT_BUFFER_LENGTH];
    static uint16_t input_text_utf16[TEXT_BUFFER_LENGTH];
    static uint16_t initial_text_utf16[TEXT_BUFFER_LENGTH];
    static uint8_t input_text_utf8[TEXT_BUFFER_LENGTH];

    // Convert UTF8 to UTF16
    memset(title_utf16, 0, sizeof(title_utf16));
    memset(initial_text_utf16, 0, sizeof(initial_text_utf16));
    utf8_to_utf16((uint8_t *)"Enter your name:", title_utf16);
    utf8_to_utf16((uint8_t *)"Sergio", initial_text_utf16);

	printf("osk test...\n");

	init_screen(host_addr, HOST_SIZE);
	ioPadInit(7);

	// Configure the title and initial text of the keyboard, and a maximum length
	inputFieldInfo.message = title_utf16;
	inputFieldInfo.startText = initial_text_utf16;
	inputFieldInfo.maxLength = TEXT_BUFFER_LENGTH - 1;

	// Configure the type of panel
	parameters.allowedPanels = OSK_PANEL_TYPE_DEFAULT;
	parameters.firstViewPanel = OSK_PANEL_TYPE_DEFAULT;
	parameters.controlPoint = (oskPoint) { 0, 0 };
	parameters.prohibitFlags = OSK_PROHIBIT_RETURN; // This will disable entering a new line
	
	// Configure where the osk will write its result
	outputParam.res = OSK_OK;
	outputParam.len = TEXT_BUFFER_LENGTH - 1;
	outputParam.str = input_text_utf16;

	atexit(program_exit_callback);

	s32 res = 0;
	
	sys_mem_container_t containerid;
	res = sysMemContainerCreate(&containerid, 4 * 1024 * 1024);
	if (res != 0)
	{
        printf("Error sysMemContainerCreate: %08x\n", res);
		return 0;
	}

	res = sysUtilRegisterCallback(SYSUTIL_EVENT_SLOT0, sysutil_exit_callback, NULL);
	if (res != 0)
	{
        printf("Error sysUtilRegisterCallback: %08x\n", res);
		sysMemContainerDestroy(containerid);
		return 0;
	}

	oskSetInitialInputDevice(OSK_DEVICE_PAD);
	oskSetKeyLayoutOption(OSK_FULLKEY_PANEL);
	oskSetLayoutMode(OSK_LAYOUTMODE_HORIZONTAL_ALIGN_CENTER | OSK_LAYOUTMODE_VERTICAL_ALIGN_CENTER);

	res = oskLoadAsync(containerid, &parameters, &inputFieldInfo);
	if (res != 0)
	{
        printf("Error oskLoadAsync: %08x\n", res);
		sysUtilUnregisterCallback(SYSUTIL_EVENT_SLOT0);
		sysMemContainerDestroy(containerid);
		return 0;
	}

	printf("Running OSK\n");

	isRunningOSK = 1;

	while (isRunningOSK)
	{
		do_flip();
	}

	sysUtilUnregisterCallback(SYSUTIL_EVENT_SLOT0);
	sysMemContainerDestroy(containerid);

	if (outputParam.res != OSK_OK)
	{
        printf("Keyboard cancelled\n");
		return 0;
	}

	// Convert UTF16 to UTF8
	utf16_to_utf8(outputParam.str, input_text_utf8);
	printf("Hello %s!\n", input_text_utf8);

	sleep(5);

	return 0;
}
