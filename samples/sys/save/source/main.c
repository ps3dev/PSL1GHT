/*
 * main.c : Load/Save game sample
 *
 * Copyright (C) Youness Alaoui (KaKaRoTo)
 *
 * This software is distributed under the terms of the MIT License
 *
 */


#include "rsxutil.h"
#include <io/pad.h>
#include <sys/thread.h>
#include <sys/memory.h>
#include <sysutil/video.h>
#include <sysutil/sysutil.h>
#include <sysutil/save.h>
#include <string.h>
#include <stdio.h>
#include <malloc.h>

#define THREAD_STACK_SIZE 16*1024
#define THREAD_PRIORITY 1000

/* Allow 100 save games */
#define SAVE_LIST_MAX_DIRECTORIES 100
/* Max 3 files : icon, screenshot, save data */
#define SAVE_LIST_MAX_FILES 3

/* This should actually be the maximum between
 * (MAX_FILES * sizeof(sysSaveFileStatus)) and
 * (MAX_DIRECTORIES * sizeof(sysSaveDirectoryList))
 */
#define BUFFER_SETTINGS_BUFSIZE (SAVE_LIST_MAX_DIRECTORIES * \
      sizeof(sysSaveDirectoryList))
#define MEMORY_CONTAINER_SIZE (5*1024*1024)

#define SAVE_DATA_FILENAME "GAME"

enum SaveDataMode {
  PS3_SAVE_MODE_ICON,
  PS3_SAVE_MODE_SCREENSHOT,
  PS3_SAVE_MODE_DATA,
  PS3_SAVE_MODE_DONE,
};

static struct SaveData {
  sys_ppu_thread_t save_tid;
  int saving;
  int loading;
  char prefix[SYS_SAVE_MAX_DIRECTORY_NAME];
  sysSaveNewSaveGame new_save;
  sysSaveNewSaveGameIcon new_save_icon;
  enum SaveDataMode mode;
  u8 *icon_data;
  u64 icon_size;
  u8 *screenshot_data;
  u64 screenshot_size;
  u8 *save_data;
  u64 save_size;
  s32 result;
} *save_data;

static gcmContextData *context;
static void *host_addr;

#define MAX_BUFFERS 2

/* The buffers we will be drawing into. */
static rsxBuffer buffers[MAX_BUFFERS];
static int currentBuffer;
static int counter = 0;


void
saveload_game_list_cb (sysSaveCallbackResult *result,
    sysSaveListIn *in, sysSaveListOut *out)
{
  int i;

  printf ("saveload_game_list_cb called\n");

  printf ("Found %d directories. Listing %d\n", in->maxDirectories,
      in->numDirectories);
  for (i = 0; i < in->numDirectories; i++)
    printf ("   Directory : %s [%s]\n",
        in->directoryList[i].directoryName,
        in->directoryList[i].listParameter);

  memset (out, 0, sizeof(sysSaveListOut));
  out->focus = SYS_SAVE_FOCUS_POSITION_LIST_HEAD;
  out->numDirectories = in->numDirectories;
  out->directoryList = in->directoryList;

  /* If we're saving and we didn't reach max saves, add the new save button */
  if (save_data->saving && out->numDirectories < SAVE_LIST_MAX_DIRECTORIES) {
    sysSaveNewSaveGame *new_save = &save_data->new_save;
    sysSaveNewSaveGameIcon *new_save_icon = &save_data->new_save_icon;
    char *dir = malloc (SYS_SAVE_MAX_DIRECTORY_NAME + 1);
    int idx = -1;
    int j;

    /* We must choose a property directory name for the save data */
    for (i = 0; idx == -1 && i <= 99; i++) {
      snprintf (dir, SYS_SAVE_MAX_DIRECTORY_NAME, "%s%d",
          save_data->prefix, i);
      idx = i;
      for (j = 0; j < in->numDirectories; j++) {
        if (strcmp (dir, in->directoryList[j]. \
                directoryName) == 0) {
          idx = -1;
          break;
        }
      }
    }

    if (idx != -1) {
      printf ("New directory : %s\n", dir);
      memset (new_save, 0, sizeof(sysSaveNewSaveGame));
      memset (new_save_icon, 0, sizeof(sysSaveNewSaveGameIcon));
      new_save->position = SYS_SAVE_NEW_SAVE_POSITION_TOP;
      new_save->directoryName = dir;
      if (save_data->icon_size > 0) {
        new_save->icon = new_save_icon;
        new_save_icon->iconBufferSize = save_data->icon_size;
        new_save_icon->iconBuffer = save_data->icon_data;
      }
      out->newSaveGame = new_save;
    } else {
      free (dir);
    }
  }

  result->result = SYS_SAVE_CALLBACK_RESULT_CONTINUE;
  return;
}

void
saveload_game_status_cb (sysSaveCallbackResult *result,
    sysSaveStatusIn *in, sysSaveStatusOut *out)
{
  int i;

  printf ("saveload_game_status_cb called\n");

  printf ("Free space : %d\n"
      "New save game? : %d\n"
      " -dirname : %s\n"
      " -title : %s\n"
      "  subtitle : %s\n"
      "  detail : %s\n"
      "  copy protected? : %d\n"
      "  parental level : %d\n"
      "  listParameter : %s\n"
      "binding information : %d\n"
      "size of save data : %d\n"
      "size of system file : %d\n"
      "total files : %d\n"
      "number of files : %d\n",
      in->freeSpaceKB,
      in->isNew,
      in->directoryStatus.directoryName,
      in->getParam.title,
      in->getParam.subtitle,
      in->getParam.detail,
      in->getParam.copyProtected,
      in->getParam.parentalLevel,
      in->getParam.listParameter,
      in->bindingInformation,
      in->sizeKB,
      in->systemSizeKB,
      in->totalFiles,
      in->numFiles);

  for (i = 0; i < in->numFiles; i++)
    printf ("  -File type : %d\n"
        "   File size : %lu\n"
        "   filename : %s\n",
        in->fileList[i].fileType,
        in->fileList[i].fileSize,
        in->fileList[i].filename);

  result->result = SYS_SAVE_CALLBACK_RESULT_CONTINUE;
  out->setParam = &in->getParam;

  if (save_data->loading) {
    /* Do not tell it to delete the files if we're loading!!! */
    out->recreateMode = SYS_SAVE_RECREATE_MODE_OVERWRITE_NOT_CORRUPTED;
    /* We'll only load the data */
    save_data->mode = PS3_SAVE_MODE_DATA;
    save_data->save_size = 0;
    for (i = 0; i < in->numFiles; i++) {
      switch (in->fileList[i].fileType) {
        case SYS_SAVE_FILETYPE_STANDARD_FILE:
          save_data->save_size = in->fileList[i].fileSize;
          break;
        case SYS_SAVE_FILETYPE_CONTENT_ICON0:
          save_data->icon_size = in->fileList[i].fileSize;
          break;
        case SYS_SAVE_FILETYPE_CONTENT_PIC1:
          save_data->screenshot_size = in->fileList[i].fileSize;
          break;
        default:
          break;
      }
    }
    if (save_data->save_size == 0) {
      printf ("Couldn't find the save data.. !\n");
      result->result = SYS_SAVE_CALLBACK_RESULT_CORRUPTED;
      return;
    } else {
      printf ("Found save game data of size : %lu\n", save_data->save_size);
      save_data->save_data = malloc (save_data->save_size);
    }
  } else {
    char subtitle[SYS_SAVE_MAX_SUBTITLE];

    /* Delete */
    out->recreateMode = SYS_SAVE_RECREATE_MODE_DELETE;
    save_data->mode = PS3_SAVE_MODE_ICON;

    /* Check for free space... don't forget the system file's size, and to check
     * for existing files that would get deleted if you overwrite a save game.
     * Let's assume we need 1MB of data...
     */
    if ((in->freeSpaceKB + in->sizeKB) < (1024 + in->systemSizeKB)) {
      result->result = SYS_SAVE_CALLBACK_RESULT_NO_SPACE_LEFT;
      result->missingSpaceKB  = (1024 + in->systemSizeKB) -
          (in->freeSpaceKB + in->sizeKB);
    }

    snprintf (subtitle, SYS_SAVE_MAX_SUBTITLE, "Level %d", counter);
    strncpy (in->getParam.title, "My Game!", SYS_SAVE_MAX_TITLE);
    strncpy (in->getParam.subtitle, subtitle, SYS_SAVE_MAX_SUBTITLE);
    strncpy (in->getParam.detail,
        "Character: KaKaRoTo\n"
        "Class: Awesomeness\n"
        "HP: 100\n"
        "MP: 50",
        SYS_SAVE_MAX_DETAIL);
  }

}

void
saveload_game_file_cb (sysSaveCallbackResult *result,
    sysSaveFileIn *in, sysSaveFileOut *out)
{

  printf ("saveload_game_file_cb called\n");

  printf ("Last operation %s %d bytes\n", save_data->saving? "wrote" : "read",
      in->previousOperationResultSize);

  memset (out, 0, sizeof(sysSaveFileOut));
  switch (save_data->mode) {
    case PS3_SAVE_MODE_ICON:
      {
        printf ("Saving icon\n");

        out->fileOperation = SYS_SAVE_FILE_OPERATION_WRITE;
        out->fileType = SYS_SAVE_FILETYPE_CONTENT_ICON0;
        out->size = save_data->icon_size;
        out->bufferSize = save_data->icon_size;
        out->buffer = save_data->icon_data;

        result->result = SYS_SAVE_CALLBACK_RESULT_CONTINUE;
        result->incrementProgress = 30;
        save_data->mode = PS3_SAVE_MODE_SCREENSHOT;
        break;
      }
    case PS3_SAVE_MODE_SCREENSHOT:
      {
        printf ("Saving screenshot\n");

        out->fileOperation = SYS_SAVE_FILE_OPERATION_WRITE;
        out->fileType = SYS_SAVE_FILETYPE_CONTENT_PIC1;
        out->size = save_data->screenshot_size;
        out->bufferSize = save_data->screenshot_size;
        out->buffer = save_data->screenshot_data;

        result->result = SYS_SAVE_CALLBACK_RESULT_CONTINUE;
        result->incrementProgress = 30;
        save_data->mode = PS3_SAVE_MODE_DATA;
        break;
      }
    case PS3_SAVE_MODE_DATA:
      {
        if (save_data->saving) {
          printf ("Writing game data\n");
          out->fileOperation = SYS_SAVE_FILE_OPERATION_WRITE;
        } else {
          printf ("Reading game data\n");
          out->fileOperation = SYS_SAVE_FILE_OPERATION_READ;
        }

        out->filename = SAVE_DATA_FILENAME;
        out->fileType = SYS_SAVE_FILETYPE_STANDARD_FILE;
        out->size = save_data->save_size;
        out->bufferSize = save_data->save_size;
        out->buffer = save_data->save_data;

        result->result = SYS_SAVE_CALLBACK_RESULT_CONTINUE;
        result->incrementProgress = 100;
        save_data->mode = PS3_SAVE_MODE_DONE;
        break;
      }
    case PS3_SAVE_MODE_DONE:
    default:
      result->result = SYS_SAVE_CALLBACK_RESULT_DONE;
      if (save_data->loading) {
        if (in->previousOperationResultSize != save_data->save_size) {
          result->result = SYS_SAVE_CALLBACK_RESULT_CORRUPTED;
        } else {
          if ((counter % 2) == 0) {
            result->result = SYS_SAVE_CALLBACK_RESULT_ERROR_CUSTOM;
            result->customErrorMessage = (char *)save_data->save_data;
          }
        }
      }
      break;
  }

  printf ("saveload_game_file_cb exit\n");
}

static int
load_file (const char *filename, u8 **data, u64 *size)
{
  FILE *fd = fopen (filename, "rb");

  if (fd == NULL)
    return FALSE;

  fseek (fd, 0, SEEK_END);
  *size = ftell (fd);
  fseek (fd, 0, SEEK_SET);

  *data = malloc (*size);
  fread (*data, *size, 1, fd);
  fclose (fd);

  return TRUE;
}


void
saveload_game_thread(void *user_data)
{
  sysSaveListSettings listSettings;
  sysSaveBufferSettings bufferSettings;
  mem_container_t container;
  char *prefix = save_data->prefix;
  char filename[256];
  s32 ret;

  printf ("saveload_thread started\n");

  /* Directory name must be all upper case */
  strncpy (prefix, "SAVELOAD-TEST", SYS_SAVE_MAX_DIRECTORY_NAME);


  memset (&listSettings, 0, sizeof (sysSaveListSettings));
  listSettings.sortType = SYS_SAVE_SORT_TYPE_TIMESTAMP;
  listSettings.sortOrder = SYS_SAVE_SORT_ORDER_DESCENDING;
  listSettings.pathPrefix = save_data->prefix;
  listSettings.reserved = NULL;

  memset (&bufferSettings, 0, sizeof (sysSaveBufferSettings));
  bufferSettings.maxDirectories = SAVE_LIST_MAX_DIRECTORIES;
  bufferSettings.maxFiles = SAVE_LIST_MAX_FILES;
  bufferSettings.bufferSize = BUFFER_SETTINGS_BUFSIZE;
  bufferSettings.buffer = malloc (bufferSettings.bufferSize);

  if (sysMemContainerCreate (&container, MEMORY_CONTAINER_SIZE) != 0 ) {
    printf ("Unable to create memory container\n");
    goto end;
  }

  if (save_data->saving) {
    printf ("Loading icon\n");

    snprintf (filename, 255, "data/icon%d.png", counter);
    if (load_file (filename, &save_data->icon_data,
            &save_data->icon_size) == FALSE) {
      snprintf (filename, 255, "data/icon.png");
      load_file (filename, &save_data->icon_data, &save_data->icon_size);
    }

    printf ("Loading screenshot\n");
    snprintf (filename, 255, "data/screenshot%d.png", counter);
    if (load_file (filename, &save_data->icon_data,
            &save_data->icon_size) == FALSE) {
      snprintf (filename, 255, "data/screenshot.png");
      load_file (filename, &save_data->icon_data, &save_data->icon_size);
    }

    save_data->save_data = malloc (1024);
    snprintf ((char *) save_data->save_data, 1024,
        "This is the content of save data : %d",  counter);

    save_data->save_size = strlen ((char *) save_data->save_data);
    printf ("Save data is : %s\n", save_data->save_data);

    ret = sysSaveListSave2 (SYS_SAVE_CURRENT_VERSION,
        &listSettings, &bufferSettings,
        saveload_game_list_cb, saveload_game_status_cb, saveload_game_file_cb,
        container, NULL);
  } else {
    ret = sysSaveListLoad2 (SYS_SAVE_CURRENT_VERSION,
        &listSettings, &bufferSettings,
        saveload_game_list_cb, saveload_game_status_cb, saveload_game_file_cb,
        container, NULL);
  }

  save_data->result = ret;

  printf ("sysSaveListLoad2/Save2 returned : %d\n", ret);
  sysMemContainerDestroy (container);

end:
  if (bufferSettings.buffer)
    free (bufferSettings.buffer);
  if (save_data->new_save.directoryName)
    free (save_data->new_save.directoryName);
  if (save_data->save_data)
    free (save_data->save_data);
  if (save_data->icon_data)
    free (save_data->icon_data);
  if (save_data->screenshot_data)
    free (save_data->screenshot_data);

  printf ("saveload_thread exiting\n");
  save_data->save_tid = 0;
  sysThreadExit (0);
}

int
_create_thread (int saving, char *thread_name)
{
  s32 ret;

  if (save_data->save_tid != 0) {
    printf ("Save/Load thread already running\n");
    return FALSE;
  }

  memset (save_data, 0, sizeof(struct SaveData));
  save_data->saving = saving;
  save_data->loading = !saving;
  ret = sysThreadCreate (&save_data->save_tid, saveload_game_thread, NULL,
      THREAD_PRIORITY, THREAD_STACK_SIZE, 0, thread_name);

  if (ret < 0) {
    printf ("Failed to create %s : %d\n", thread_name, ret);
    return FALSE;
  }

  return TRUE;
}

int
test_save_game ()
{
  return _create_thread (TRUE, "save_thread");
}

int
test_load_game ()
{
  return _create_thread (FALSE, "save_thread");
}

static int xmb_exit = 0;

static void
event_handler (u64 status, u64 param, void * user_data)
{

  static struct XMBEvent {
    int drawing;
    int opened;
    int closed;
    int exit;
  } xmb;

  printf ("Received event %lX\n", status);
  if (status == SYSUTIL_EXIT_GAME) {
    xmb_exit = xmb.exit = 1;
  } else if (status == SYSUTIL_MENU_OPEN) {
    xmb.opened = 1;
    xmb.closed = 0;
  } else if (status == SYSUTIL_MENU_CLOSE) {
    xmb.opened = 0;
    xmb.closed = 1;
  } else if (status == SYSUTIL_DRAW_BEGIN) {
    /* We must start drawing, to avoid the app freezing */
    xmb.drawing = 1;
  } else if (status == SYSUTIL_DRAW_END) {
    xmb.drawing = 0;
  }
}

int
main_loop_iterate ()
{
  padInfo padinfo;
  padData paddata;
  int i;
  rsxBuffer *buffer = &buffers[currentBuffer];

  if (xmb_exit != 0)
    goto done;

  ioPadGetInfo (&padinfo);
  for (i = 0; i < MAX_PADS; i++) {
    if (padinfo.status[i]) {
      ioPadGetData (i, &paddata);
      if(paddata.BTN_CROSS)
        goto done;
    }
  }

  setRenderTarget(context, buffer);
  /* Wait for the last flip to finish, so we can draw to the old buffer */
  waitFlip ();
  memset (buffer->ptr, 0, buffer->height * buffer->width * 4);
  /* Flip buffer onto screen */
  flipBuffer (context, currentBuffer);
  currentBuffer++;
  if (currentBuffer >= MAX_BUFFERS)
    currentBuffer = 0;

  /* If we just started and we're doing nothing, then save */
  if (save_data->saving == 0 && save_data->loading == 0)
    test_save_game ();

  /* Check if we finished loading, then start saving */
  if (save_data->loading && save_data->save_tid == 0)
    test_save_game ();

  /* Check if we finished saving, then start loading */
  if (save_data->saving && save_data->save_tid == 0) {
    test_load_game ();
    counter++;
  }

  /* We need to poll for events */
  sysUtilCheckCallback ();

  return TRUE;

 done:
  return FALSE;
}

int
main (int argc, char *argv[])
{
  int i;
  u16 width, height;

  /* Allocate a 1Mb buffer, alligned to a 1Mb boundary
   * to be our shared IO memory with the RSX. */
  host_addr = memalign (1024*1024, HOST_SIZE);
  context = initScreen (host_addr, HOST_SIZE);

  getResolution(&width, &height);
  for (i = 0; i < MAX_BUFFERS; i++)
    makeBuffer (&buffers[i], width, height, i);

  ioPadInit (7);
  sysUtilRegisterCallback (SYSUTIL_EVENT_SLOT0, event_handler, NULL);

  flipBuffer(context, MAX_BUFFERS - 1);

  save_data = malloc (sizeof(struct SaveData));
  memset (save_data, 0, sizeof(save_data));

  /* Main loop */
  while (main_loop_iterate ());

  ioPadEnd();
  sysUtilUnregisterCallback(SYSUTIL_EVENT_SLOT0);

  gcmSetWaitFlip(context);
  for (i = 0; i < MAX_BUFFERS; i++)
    rsxFree (buffers[i].ptr);

  rsxFinish (context, 1);
  free (host_addr);

  return 0;
}
