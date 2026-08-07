#ifndef __LV2_SYSUTIL_GAME_H__
#define __LV2_SYSUTIL_GAME_H__

#include <ppu-types.h>
#include <ppu-asm.h>


/*
 * constants
 */

/* Attribute / boot */
#define CELL_GAME_ATTRIBUTE_PATCH                 (1 <<0) /* boot Patch */
#define CELL_GAME_ATTRIBUTE_APP_HOME              (1 <<1) /* boot from / app_home/PS3_GAME */
#define CELL_GAME_ATTRIBUTE_DEBUG                 (1 <<2) /* start with Debug Mode */
#define CELL_GAME_ATTRIBUTE_XMBBUY                (1 <<3) /* restart of the utility purchases Game */
#define CELL_GAME_ATTRIBUTE_COMMERCE2_BROWSER     (1 <<4) /* restart from Commerce2Browser */
#define CELL_GAME_ATTRIBUTE_INVITE_MESSAGE        (1 <<5) /* boot from the invitation Title */
#define CELL_GAME_ATTRIBUTE_CUSTOM_DATA_MESSAGE   (1 <<6) /* start with the attached conditions data */
#define CELL_GAME_ATTRIBUTE_WEB_BROWSER           (1 <<8) /* restart from WebBrowser */

/* Options operation theme file installation */
#define CELL_GAME_THEME_OPTION_NONE               (0)     /* No option */
#define CELL_GAME_THEME_OPTION_APPLY              (1 <<0) /* to be applied after the Installation */

/* Code function exit callback */
#define CELL_GAME_CBRESULT_OK                     (0)     /* exit store data */
#define CELL_GAME_CBRESULT_OK_CANCEL              (1)     /* exit to cancel Theme Installation */

#define CELL_GAME_SIZEKB_NOTCALC                  (-1)    /* Directory Size is not calculated */
#define CELL_GAME_THEMEINSTALL_BUFSIZE_MIN        (4096)  /* Size of the minimum buffer Theme Installation */


#ifdef __cplusplus
extern "C" {
#endif

/*
 * enumerations
 */

/* PARAM.SFO parameters */
typedef enum sys_game_system_param_size
{
  CELL_GAME_SYSP_LANGUAGE_NUM =                   20,     /* number of languages */
  CELL_GAME_SYSP_TITLE_SIZE =                     128,    /* TITLE size */
  CELL_GAME_SYSP_TITLEID_SIZE =                   10,     /* TITLE_ID size */
  CELL_GAME_SYSP_VERSION_SIZE =                   6,      /* Content Version Size */
  CELL_GAME_SYSP_PS3_SYSTEM_VER_SIZE =            8,      /* Runtime Library Release Version size */
  CELL_GAME_SYSP_APP_VER_SIZE =                   6       /* System Patches Version size */
} CellGameSystemParamSize;

/* PARAM.SFO, ID setting information acquisition */
typedef enum sys_game_params
{
  CELL_GAME_PARAMID_TITLE =                       0,
  CELL_GAME_PARAMID_TITLE_DEFAULT,
  CELL_GAME_PARAMID_TITLE_JAPANESE,
  CELL_GAME_PARAMID_TITLE_ENGLISH,
  CELL_GAME_PARAMID_TITLE_FRENCH,
  CELL_GAME_PARAMID_TITLE_SPANISH,
  CELL_GAME_PARAMID_TITLE_GERMAN,
  CELL_GAME_PARAMID_TITLE_ITALIAN,
  CELL_GAME_PARAMID_TITLE_DUTCH,
  CELL_GAME_PARAMID_TITLE_PORTUGUESE,
  CELL_GAME_PARAMID_TITLE_RUSSIAN,
  CELL_GAME_PARAMID_TITLE_KOREAN,
  CELL_GAME_PARAMID_TITLE_CHINESE_T,
  CELL_GAME_PARAMID_TITLE_CHINESE_S,
  CELL_GAME_PARAMID_TITLE_FINNISH,
  CELL_GAME_PARAMID_TITLE_SWEDISH,
  CELL_GAME_PARAMID_TITLE_DANISH,
  CELL_GAME_PARAMID_TITLE_NORWEGIAN,
  CELL_GAME_PARAMID_TITLE_POLISH,

  CELL_GAME_PARAMID_TITLE_ID =                    100,
  CELL_GAME_PARAMID_VERSION,
  CELL_GAME_PARAMID_PARENTAL_LEVEL,
  CELL_GAME_PARAMID_RESOLUTION,
  CELL_GAME_PARAMID_SOUND_FORMAT,
  CELL_GAME_PARAMID_PS3_SYSTEM_VER,
  CELL_GAME_PARAMID_APP_VER
} CellGameParams;

/* error dialog */
typedef enum sys_game_error
{
  CELL_GAME_ERRDIALOG_BROKEN_GAMEDATA =           0,      /* corruption error */
  CELL_GAME_ERRDIALOG_BROKEN_HDDGAME,                     /* corruption error */
  CELL_GAME_ERRDIALOG_NOSPACE,                            /* HDD full */

  CELL_GAME_ERRDIALOG_BROKEN_EXIT_GAMEDATA =      100,    /* corruption error */
  CELL_GAME_ERRDIALOG_BROKEN_EXIT_HDDGAME,                /* corruption error */
  CELL_GAME_ERRDIALOG_NOSPACE_EXIT                        /* HDD full */
} CellGameError;

/* E Set the supported rendering resolution resolution settings corresponding drawing  */
typedef enum sys_game_resolution
{
  CELL_GAME_RESOLUTION_1080 =                     0x08,   /* 1080 */
  CELL_GAME_RESOLUTION_720 =                      0x04,   /* 720 */
  CELL_GAME_RESOLUTION_576SQ =                    0x20,   /* Wide screen of 576 */
  CELL_GAME_RESOLUTION_576 =                      0x02,   /* 576 */
  CELL_GAME_RESOLUTION_480SQ =                    0x10,   /* Wide screen of 480 */
  CELL_GAME_RESOLUTION_480 =                      0x01    /* 480 */
} CellGameResolution;

/* Set the supported sound format audio formats support setting */
typedef enum sys_game_sound_format
{
  CELL_GAME_SOUNDFORMAT_71LPCM =                  0x10,   /* 7.1ch LPCM */
  CELL_GAME_SOUNDFORMAT_51LPCM =                  0x04,   /* 5.1ch LPCM */
  CELL_GAME_SOUNDFORMAT_51DDENC =                 0x102,  /* Dolby Digital */
  CELL_GAME_SOUNDFORMAT_51DTSENC =                0x202,  /* DTS Digital Surround */
  CELL_GAME_SOUNDFORMAT_2LPCM =                   0x01,   /* 2ch LPCM */
} CellGameSoundFormat;

/* Disc type */
typedef enum sys_game_disc_type
{
  CELL_GAME_DISCTYPE_OTHER =                      0,
  CELL_GAME_DISCTYPE_PS3,
  CELL_GAME_DISCTYPE_PS2
} CellGameDiscType;


/*
 * structures
 */

/* Directory Size *Information */
typedef struct sys_game_content_size
{
  s32 hddFreeSizeKB;
  s32 sizeKB;
  s32 sysSizeKB;
} CellGameContentSize;

/* for setting the initial value PARAM.SFO Parameters */
typedef struct sys_game_set_init_params {
  char title [CELL_GAME_SYSP_TITLE_SIZE]; 
  char titleId [CELL_GAME_SYSP_TITLEID_SIZE]; 
  char reserved0 [2];
  char version [CELL_GAME_SYSP_VERSION_SIZE]; 
  char reserved1 [66];
} CellGameSetInitParams;


/*
 * callbacks
 */

typedef s32 (*CellGameThemeInstallCallback)(u32 fileOffset, u32 readSize, void *buf);
typedef void (*CellGameDiscEjectCallback)(void);
typedef void (*CellGameDiscInsertCallback)(u32 discType, char *titleId);


/*
 * functions
 */

s32 cellGameDataCheck(u32 type, const char *dirName, CellGameContentSize *size);
s32 cellGameBootCheck(u32 *type, u32 *attributes, CellGameContentSize *size, char *dirName);
s32 cellGamePatchCheck(CellGameContentSize *size, void *reserved);
s32 cellGameCreateGameData(CellGameSetInitParams *init, char *tmp_contentInfoPath, char *tmp_usrdirPath);
s32 cellGameGetParamInt(s32 id, s32 *value);
s32 cellGameGetParamString(s32 id, char *buf, u32 bufsize);
s32 cellGameGetSizeKB(s32 *sizeKB);
s32 cellGameSetParamString(s32 id, const char *buf);
s32 cellGameGetDiscContentInfoUpdatePath(char *updatePath);
s32 cellGameContentPermit(char *contentInfoPath, char *usrdirPath);
s32 cellGameContentErrorDialog(s32 type, s32 errNeedSizeKB, const char *dirName);
s32 cellGameThemeInstall(const char *usrdirPath, const char *fileName, u32 option);
s32 cellGameThemeInstallFromBuffer(u32 fileSize, u32 bufSize, void *buf, CellGameThemeInstallCallback cb, u32 option);
s32 cellGameGetLocalWebContentPath(char *contentPath);
s32 cellGameDeleteGameData(const char *dirName);
s32 cellGameRegisterDiscChangeCallback(CellGameDiscEjectCallback cbEject, CellGameDiscInsertCallback cbInsert);
s32 cellGameUnregisterDiscChangeCallback(void);

#ifdef __cplusplus
    }
#endif

#endif /* __LV2_SYSUTIL_GAME_H__ */

