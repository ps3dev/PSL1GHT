#ifndef __LV2_SYSUTIL_SAVE_H__
#define __LV2_SYSUTIL_SAVE_H__

#include <ppu-types.h>
#include <time.h>

/* Maximum string/array sizes */
#define CELL_SAVE_DATA_MAX_DIRECTORY_NAME		32
#define CELL_SAVE_DATA_MAX_FILE_NAME			13
#define CELL_SAVE_DATA_DIRECTORY_LIST_MAX		2048
#define CELL_SAVE_DATA_CUSTOM_ERROR_MESSAGE_MAX	256
#define CELL_SAVE_DATA_MAX_LIST_PARAMETER		8
#define CELL_SAVE_DATA_MAX_TITLE			128
#define CELL_SAVE_DATA_MAX_SUBTITLE			128
#define CELL_SAVE_DATA_MAX_DETAIL			1024
#define CELL_SAVE_DATA_MAX_PROTECTED_FILE_ID		16

/* Current version value for the save data */
#define CELL_SAVE_DATA_CURRENT_VERSION 0

/* Result values for the callback */
#define CELL_SAVE_DATA_CALLBACK_RESULT_DONE		1
#define CELL_SAVE_DATA_CALLBACK_RESULT_CONTINUE 	0
#define CELL_SAVE_DATA_CALLBACK_RESULT_NO_SPACE_LEFT	-1
#define CELL_SAVE_DATA_CALLBACK_RESULT_ERROR		-2
#define CELL_SAVE_DATA_CALLBACK_RESULT_CORRUPTED	-3
#define CELL_SAVE_DATA_CALLBACK_RESULT_NOT_FOUND	-4
#define CELL_SAVE_DATA_CALLBACK_RESULT_ERROR_CUSTOM	-5

/* Flags for the binding information */
#define CELL_SAVE_DATA_BIND_NO_ERROR		0x00
#define CELL_SAVE_DATA_BIND_ANOTHER_CONSOLE	0x01
#define CELL_SAVE_DATA_BIND_ANOTHER_DISC	0x02
#define CELL_SAVE_DATA_BIND_ANOTHER_APP	0x04
#define CELL_SAVE_DATA_BIND_NO_USER_INFO	0x08
#define CELL_SAVE_DATA_BIND_OTHER_USER	0x10

/* Return values from the API calls */
#define CELL_SAVE_DATA_RETURN_DONE			0
#define CELL_SAVE_DATA_RETURN_CANCELED		1
#define CELL_SAVE_DATA_RETURN_ERROR			0x8002b400

#define CELL_SAVE_DATA_RETURN_ERROR_CALLBACK		(CELL_SAVE_DATA_RETURN_ERROR | 1)
#define CELL_SAVE_DATA_RETURN_ERROR_HDD_ERROR		(CELL_SAVE_DATA_RETURN_ERROR | 2)
#define CELL_SAVE_DATA_RETURN_ERROR_INTERNAL		(CELL_SAVE_DATA_RETURN_ERROR | 3)
#define CELL_SAVE_DATA_RETURN_ERROR_INVALID_ARG	(CELL_SAVE_DATA_RETURN_ERROR | 4)
#define CELL_SAVE_DATA_RETURN_ERROR_NO_SPACE_LEFT	(CELL_SAVE_DATA_RETURN_ERROR | 5)
#define CELL_SAVE_DATA_RETURN_ERROR_CORRUPTED		(CELL_SAVE_DATA_RETURN_ERROR | 6)
#define CELL_SAVE_DATA_RETURN_ERROR_FAILED		(CELL_SAVE_DATA_RETURN_ERROR | 7)
#define CELL_SAVE_DATA_RETURN_ERROR_ALREADY_IN_USE	(CELL_SAVE_DATA_RETURN_ERROR | 8)

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
  CELL_SAVE_DATA_SORT_TYPE_TIMESTAMP = 0,
  CELL_SAVE_DATA_SORT_TYPE_TITLE,
} CellSaveDataSortType;

typedef enum {
  CELL_SAVE_DATA_SORT_ORDER_DESCENDING = 0,
  CELL_SAVE_DATA_SORT_ORDER_ASCENDING,
} CellSaveDataSortOrder;

typedef enum {
  CELL_SAVE_DATA_FOCUS_POSITION_DIRECTORY_NAME = 0,
  CELL_SAVE_DATA_FOCUS_POSITION_LIST_HEAD,
  CELL_SAVE_DATA_FOCUS_POSITION_LIST_TAIL,
  CELL_SAVE_DATA_FOCUS_POSITION_LATEST,
  CELL_SAVE_DATA_FOCUS_POSITION_OLDEST,
  CELL_SAVE_DATA_FOCUS_POSITION_NEW_SAVE,
} CellSaveDataFocusPosition;

typedef enum {
  CELL_SAVE_DATA_NEW_SAVE_POSITION_TOP = 0,
  CELL_SAVE_DATA_NEW_SAVE_POSITION_BOTTOM,
} CellSaveDataNewSavePosition;


typedef enum {
  CELL_SAVE_DATA_FILETYPE_PROTECTED_FILE = 0,
  CELL_SAVE_DATA_FILETYPE_STANDARD_FILE,
  CELL_SAVE_DATA_FILETYPE_CONTENT_ICON0,
  CELL_SAVE_DATA_FILETYPE_CONTENT_ICON1,
  CELL_SAVE_DATA_FILETYPE_CONTENT_PIC1,
  CELL_SAVE_DATA_FILETYPE_CONTENT_SND0,
} CellSaveDataFileType;

typedef enum {
  CELL_SAVE_DATA_RECREATE_MODE_OVERWRITE_CORRUPTED = 0,
  CELL_SAVE_DATA_RECREATE_MODE_OVERWRITE_NOT_CORRUPTED,
  CELL_SAVE_DATA_RECREATE_MODE_DELETE
} CellSaveDataRecreateMode;

typedef enum {
  CELL_SAVE_DATA_FILE_OPERATION_READ = 0,
  CELL_SAVE_DATA_FILE_OPERATION_WRITE,
  CELL_SAVE_DATA_FILE_OPERATION_DELETE,
} CellSaveDataFileOperation;

typedef enum {
  CELL_SAVE_DATA_ERROR_DIALOG_NONE = 0,
  CELL_SAVE_DATA_ERROR_DIALOG_SHOW,
  CELL_SAVE_DATA_ERROR_DIALOG_SHOW_ONCE,
} CellSaveDataErrorDialogMode;

typedef struct {
  CellSaveDataSortType sortType;
  CellSaveDataSortOrder sortOrder;
  char *pathPrefix ATTRIBUTE_PRXPTR;
  void *reserved ATTRIBUTE_PRXPTR;
} CellSaveDataListSettings;

typedef struct {
  u32 maxDirectories;
  u32 maxFiles;
  u32 reserved[6];
  u32 bufferSize;
  void *buffer ATTRIBUTE_PRXPTR;
} CellSaveDataBufferSettings;


typedef struct {
  s32 result;
  u32 incrementProgress;
  s32 missingSpaceKB;
  char *customErrorMessage ATTRIBUTE_PRXPTR;
  void *user_data ATTRIBUTE_PRXPTR;
} CellSaveDataCallbackResult;

typedef struct {
  char directoryName[CELL_SAVE_DATA_MAX_DIRECTORY_NAME];
  char listParameter[CELL_SAVE_DATA_MAX_LIST_PARAMETER];
  u8 reserved[8];
} CellSaveDataDirectoryList;

typedef struct {
  char *title ATTRIBUTE_PRXPTR;
  u32 iconBufferSize;
  void *iconBuffer ATTRIBUTE_PRXPTR;
  void *reserved ATTRIBUTE_PRXPTR;
} CellSaveDataNewSaveGameIcon;

typedef struct {
  CellSaveDataNewSavePosition position;
  char *directoryName ATTRIBUTE_PRXPTR;
  CellSaveDataNewSaveGameIcon *icon ATTRIBUTE_PRXPTR;
  void *reserved ATTRIBUTE_PRXPTR;
} CellSaveDataNewSaveGame;

typedef struct {
  /* total directories found */
  u32 maxDirectories;
  /* number of directories in directoryList */
  u32 numDirectories;
  /* list of directories found */
  CellSaveDataDirectoryList *directoryList ATTRIBUTE_PRXPTR;
  u8 reserved[64];
} CellSaveDataListIn;

typedef struct {
  /* Where to put the focus of the cursor */
  CellSaveDataFocusPosition focus;
  /* The name of the directory to focus to */
  char *focusDirectoryName ATTRIBUTE_PRXPTR;
  u32 numDirectories;
  CellSaveDataDirectoryList *directoryList ATTRIBUTE_PRXPTR;
  CellSaveDataNewSaveGame *newSaveGame ATTRIBUTE_PRXPTR;
  void *reserved ATTRIBUTE_PRXPTR;
} CellSaveDataListOut;

typedef struct {
  time_t atime;
  time_t mtime;
  time_t ctime;
  char directoryName[CELL_SAVE_DATA_MAX_DIRECTORY_NAME];
} CellSaveDataDirectoryStatus;

typedef struct {
  char title[CELL_SAVE_DATA_MAX_TITLE];
  char subtitle[CELL_SAVE_DATA_MAX_SUBTITLE];
  char detail[CELL_SAVE_DATA_MAX_DETAIL];
  u32 copyProtected;
  u32 parentalLevel;
  char listParameter[CELL_SAVE_DATA_MAX_LIST_PARAMETER];
  u8 reserved[256];
} CellSaveDataSystemFileParam;

typedef struct {
  CellSaveDataFileType fileType;
  u8 reserved1[4];
  u64 fileSize;
  time_t atime;
  time_t mtime;
  time_t ctime;
  char filename[CELL_SAVE_DATA_MAX_FILE_NAME];
  u8 reserved2[3];
} CellSaveDataFileStatus;

typedef struct {
  s32 freeSpaceKB;
  u32 isNew;
  CellSaveDataDirectoryStatus directoryStatus;
  CellSaveDataSystemFileParam getParam;
  u32 bindingInformation;
  /* Size of the save data */
  s32 sizeKB;
  /* Size of system file */
  s32 systemSizeKB;
  /* total files in the save data */
  u32 totalFiles;
  /* Number of files in the the file list */
  u32 numFiles;
  CellSaveDataFileStatus *fileList ATTRIBUTE_PRXPTR;
  u8 reserved[64];
} CellSaveDataStatusIn;

typedef struct {
  CellSaveDataSystemFileParam *setParam ATTRIBUTE_PRXPTR;
  CellSaveDataRecreateMode recreateMode;
  void *reserved ATTRIBUTE_PRXPTR;
} CellSaveDataStatusOut;

typedef struct {
  u32 previousOperationResultSize;
  u8 reserved[64];
} CellSaveDataFileIn;

typedef struct {
  CellSaveDataFileOperation fileOperation;
  void *reserved ATTRIBUTE_PRXPTR;
  CellSaveDataFileType fileType;
  u8 protectedFileID[CELL_SAVE_DATA_MAX_PROTECTED_FILE_ID];
  char *filename ATTRIBUTE_PRXPTR;
  u32 offset;
  u32 size;
  u32 bufferSize;
  void *buffer ATTRIBUTE_PRXPTR;
} CellSaveDataFileOut;

typedef struct {
  char *directoryName ATTRIBUTE_PRXPTR;
  CellSaveDataNewSaveGameIcon *icon ATTRIBUTE_PRXPTR;
  void *reserved ATTRIBUTE_PRXPTR;
} CellSaveDataFixedOut;

typedef void (* CellSaveDataListCallback) (CellSaveDataCallbackResult *result,
    CellSaveDataListIn *in, CellSaveDataListOut *out);

typedef void (* CellSaveDataStatusCallback) (CellSaveDataCallbackResult *result,
    CellSaveDataStatusIn *in, CellSaveDataStatusOut *out);

typedef void (* CellSaveDataFileCallback) (CellSaveDataCallbackResult *result,
    CellSaveDataFileIn *in, CellSaveDataFileOut *out);

typedef void (* CellSaveDataFixedCallback) (CellSaveDataCallbackResult *result,
    CellSaveDataListIn *in, CellSaveDataFixedOut *out);


s32 cellSaveDataListLoad2 (u32 version,
    CellSaveDataListSettings *listSettings,
    CellSaveDataBufferSettings *bufferSettings,
    CellSaveDataListCallback listCb,
    CellSaveDataStatusCallback statusCb,
    CellSaveDataFileCallback fileCb,
    sys_mem_container_t container,
    void *user_data);

s32 cellSaveDataListSave2 (u32 version,
    CellSaveDataListSettings *listSettings,
    CellSaveDataBufferSettings *bufferSettings,
    CellSaveDataListCallback listCb,
    CellSaveDataStatusCallback statusCb,
    CellSaveDataFileCallback fileCb,
    sys_mem_container_t container,
    void *user_data);

s32 cellSaveDataListAutoLoad (u32 version,
    CellSaveDataErrorDialogMode errorDialogMode,
    CellSaveDataListSettings *listSettings,
    CellSaveDataBufferSettings *bufferSettings,
    CellSaveDataFixedCallback fixedCb,
    CellSaveDataStatusCallback statusCb,
    CellSaveDataFileCallback fileCb,
    sys_mem_container_t container,
    void *user_data);

s32 cellSaveDataListAutoSave (u32 version,
    CellSaveDataErrorDialogMode errorDialogMode,
    CellSaveDataListSettings *listSettings,
    CellSaveDataBufferSettings *bufferSettings,
    CellSaveDataFixedCallback fixedCb,
    CellSaveDataStatusCallback statusCb,
    CellSaveDataFileCallback fileCb,
    sys_mem_container_t container,
    void *user_data);

s32 cellSaveDataFixedLoad2 (u32 version,
    CellSaveDataListSettings *listSettings,
    CellSaveDataBufferSettings *bufferSettings,
    CellSaveDataFixedCallback fixedCb,
    CellSaveDataStatusCallback statusCb,
    CellSaveDataFileCallback fileCb,
    sys_mem_container_t container,
    void *user_data);

s32 cellSaveDataFixedSave2 (u32 version,
    CellSaveDataListSettings *listSettings,
    CellSaveDataBufferSettings *bufferSettings,
    CellSaveDataFixedCallback fixedCb,
    CellSaveDataStatusCallback statusCb,
    CellSaveDataFileCallback fileCb,
    sys_mem_container_t container,
    void *user_data);

s32 cellSaveDataAutoLoad2 (s32 version,
    const char *directoryName,
    CellSaveDataErrorDialogMode errorDialogMode,
    CellSaveDataBufferSettings *bufferSettings,
    CellSaveDataStatusCallback statusCb,
    CellSaveDataFileCallback fileCb,
    sys_mem_container_t container,
    void *user_data);

s32 cellSaveDataAutoSave2 (s32 version,
    const char *directoryName,
    CellSaveDataErrorDialogMode errorDialogMode,
    CellSaveDataBufferSettings *bufferSettings,
    CellSaveDataStatusCallback statusCb,
    CellSaveDataFileCallback fileCb,
    sys_mem_container_t container,
    void *user_data);

s32 cellSaveDataDelete2(sys_mem_container_t container);

#ifdef __cplusplus
	}
#endif

#endif
