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

#define TEXT_BUFFER_LENGTH 256

static vs32 dialog_action = 0;

uint8_t isRunningOskDialog = 0;
CellOskDialogInputFieldInfo inputFieldInfo;
CellOskDialogParam parameters;
CellOskDialogCallbackReturnParam outputParam;

extern "C" {
static void program_exit_callback()
{
	cellGcmSetWaitFlip(context);
	rsxFinish(context, 1);
}

static void sysutil_exit_callback(u64 status, u64 param, void *usrdata)
{
	switch(status) {
		case CELL_SYSUTIL_EXIT_GAME:
			break;
		case CELL_SYSUTIL_DRAW_BEGIN:
		case CELL_SYSUTIL_DRAW_END:
			break;
		case CELL_SYSUTIL_OSK_LOADED:
			printf("oskdialog loaded\n");
			break;
		case CELL_SYSUTIL_OSK_INPUT_CANCELED:
			printf("oskdialog input canceled\n");
			cellOskDialogAbort();
			// fall-through
		case CELL_SYSUTIL_OSK_DONE:
			if (status == CELL_SYSUTIL_OSK_DONE)
			{
				printf("oskdialog done\n");
			}
			cellOskDialogUnloadAsync(&outputParam);

			if (outputParam.res == CELL_OSKDIALOG_OK)
			{
				printf("oskdialog result OK\n");
			}
			else
			{
				printf("OSK result: %d\n", outputParam.res);
			}

			break;
		case CELL_SYSUTIL_OSK_UNLOADED:
			printf("oskdialog unloaded\n");
			isRunningOskDialog = 0;
			break;
		default:
			break;
	}
}
}

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
	cellSysutilCheckCallback();
	flip();
}

int main(int argc,char *argv[])
{
    static uint16_t title_utf16[TEXT_BUFFER_LENGTH];
    static uint16_t input_text_utf16[TEXT_BUFFER_LENGTH];
    static uint16_t initial_text_utf16[TEXT_BUFFER_LENGTH];
    static uint8_t input_text_utf8[TEXT_BUFFER_LENGTH];

	void *host_addr = memalign(1024*1024, HOST_SIZE);

    // Convert UTF8 to UTF16
    memset(title_utf16, 0, sizeof(title_utf16));
    memset(initial_text_utf16, 0, sizeof(initial_text_utf16));
    utf8_to_utf16((uint8_t *)"Enter your name:", title_utf16);
    utf8_to_utf16((uint8_t *)"Sergio", initial_text_utf16);

	printf("osk test...\n");

	init_screen(host_addr, HOST_SIZE);
	cellPadInit(7);

	// Configure the title and initial text of the keyboard, and a maximum length
	inputFieldInfo.message = title_utf16;
	inputFieldInfo.startText = initial_text_utf16;
	inputFieldInfo.maxLength = TEXT_BUFFER_LENGTH - 1;

	// Configure the type of panel
	parameters.allowedPanels = CELL_OSKDIALOG_PANEL_TYPE_DEFAULT;
	parameters.firstViewPanel = CELL_OSKDIALOG_PANEL_TYPE_DEFAULT;
	parameters.controlPoint = (CellOskDialogPoint) { 0, 0 };
	parameters.prohibitFlags = CELL_OSKDIALOG_PROHIBIT_RETURN; // This will disable entering a new line
	
	// Configure where the osk will write its result
	outputParam.res = CELL_OSKDIALOG_OK;
	outputParam.len = TEXT_BUFFER_LENGTH - 1;
	outputParam.str = input_text_utf16;

	atexit(program_exit_callback);
    cellSysutilRegisterCallback(CELL_SYSUTIL_EVENT_SLOT0, sysutil_exit_callback, NULL);

	s32 res = 0;
	sys_mem_container_t containerid;
	res = sysMemContainerCreate(&containerid, 4 * 1024 * 1024);
	if (res != 0)
	{
        printf("Error sysMemContainerCreate: %08x\n", res);
		return 0;
	}

	cellOskDialogSetInitialInputDevice(CELL_OSKDIALOG_DEVICE_PAD);
	cellOskDialogSetKeyLayoutOption(CELL_OSKDIALOG_FULLKEY_PANEL);
	cellOskDialogSetLayoutMode(CELL_OSKDIALOG_LAYOUTMODE_HORIZONTAL_ALIGN_CENTER | CELL_OSKDIALOG_LAYOUTMODE_VERTICAL_ALIGN_CENTER);

	res = cellOskDialogLoadAsync(containerid, &parameters, &inputFieldInfo);
	if (res != 0)
	{
        printf("Error cellOskDialogLoadAsync: %08x\n", res);
		cellSysutilUnregisterCallback(CELL_SYSUTIL_EVENT_SLOT0);
		sysMemContainerDestroy(containerid);
		return 0;
	}

	printf("Running CELL_OSKDIALOG\n");

	isRunningOskDialog = 1;

	while (isRunningOskDialog)
	{
		do_flip();
	}

	cellSysutilUnregisterCallback(CELL_SYSUTIL_EVENT_SLOT0);
	sysMemContainerDestroy(containerid);

	if (outputParam.res != CELL_OSKDIALOG_OK)
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
