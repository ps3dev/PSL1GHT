#ifndef __LV2_MOUSE_H__
#define __LV2_MOUSE_H__

#include <ppu-types.h>

#define CELL_MAX_MICE				127
#define CELL_MOUSE_MAX_CODES			64

#define CELL_MOUSE_MAX_DATA_LIST		(8)

#ifdef __cplusplus
extern "C" {
#endif


typedef struct _mouse_info
{
	u32 max;					/* max mice allowed to connect */
	u32 connected;				/* how many mice connected */
	u32 info;					/* bit 0 lets the system intercept pad? other bits are reserved */
	u16 vendor_id[CELL_MAX_MICE];	/* vendor id */
	u16 product_id[CELL_MAX_MICE];	/* product id */
	u8  status[CELL_MAX_MICE];		/* 0: Not connected, 1: Connected, 5: Connected to custom controller.*/
} CellMouseInfo;

typedef struct _mouse_info_tablet
{
	u32 supported;
	u32 mode;
} CellMouseInfoTablet;

typedef struct _mouse_raw_data
{
	s32 len;
	u8 data[CELL_MOUSE_MAX_CODES];
} CellMouseRawData;

typedef struct _mouse_data
{
	u8 update;
	u8 buttons;
	s8 x_axis;
	s8 y_axis;
	s8 wheel;
	s8 tilt;	
} CellMouseData;

typedef struct _mouse_data_list
{
	u32 count;
	CellMouseData list[CELL_MOUSE_MAX_DATA_LIST];
} CellMouseDataList;

typedef struct _mouse_tablet_data
{
	s32 len;
	u8 data[CELL_MOUSE_MAX_CODES];
} CellMouseTabletData;

typedef struct _mouse_tablet_data_list
{
	u32 count;
	CellMouseTabletData list[CELL_MOUSE_MAX_DATA_LIST];
} CellMouseTabletDataList;

s32 cellMouseInit(u32 max);
s32 cellMouseEnd();
s32 cellMouseClearBuf(u32 port);
s32 cellMouseGetInfo(CellMouseInfo* info);
s32 cellMouseGetRawData(u32 port, CellMouseRawData* rawData);
s32 cellMouseGetData(u32 port, CellMouseData* data);
s32 cellMouseGetDataList(u32 port, CellMouseDataList* dataList);
s32 cellMouseGetTabletDataList(u32 port, CellMouseTabletDataList* tabletDataList);
s32 cellMouseInfoTabletMode(u32 port, CellMouseInfoTablet* infoTablet);
s32 cellMouseSetTabletMode(u32 port, u32 mode);


#ifdef __cplusplus
	}
#endif

#endif
