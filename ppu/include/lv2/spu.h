/*! \file lv2/spu.h
  \brief SPU management library.

  These are the system library calls for SPU management.
  
  \see \ref spu_man
*/

#ifndef __LV2_SPU_H__
#define __LV2_SPU_H__

#include <ppu-types.h>

/*! \brief Copy segment type */
#define SPU_SEGMENT_TYPE_COPY				0x01
/*! \brief Fill segment type */
#define SPU_SEGMENT_TYPE_FILL				0x02
/*! \brief Info segment type */
#define SPU_SEGMENT_TYPE_INFO				0x04

#define SPU_IMAGE_TYPE_USER					0x00
#define SPU_IMAGE_TYPE_KERNEL				0x00

#define SPU_IMAGE_PROTECT					0x00
#define SPU_IMAGE_DIRECT					0x01

#ifdef __cplusplus
extern "C" {
#endif

//! SPU segment data structure.
typedef struct _sys_spu_segment
{
	/*! segment type.

	Possible values:
	- \ref SPU_SEGMENT_TYPE_COPY
	- \ref SPU_SEGMENT_TYPE_FILL
	- \ref SPU_SEGMENT_TYPE_INFO
	*/
	u32 type;
	u32 lsStart;		//!< start address in local store
	u32 size;			//!< segment size in bytes
	//! bla
	union {
		/*! \brief start address in main memory to be copied if \ref type
		is \ref SPU_SEGMENT_TYPE_COPY */
		u32 paStart;
		/*! \brief fill value if \ref type is \ref SPU_SEGMENT_TYPE_FILL */
		u32 value;
	};
} sysSpuSegment;

//! SPU image data structure.
typedef struct _sys_spu_image
{
	u32 type;			//!< type
	u32 entryPoint;		//!< entry point
	u32 segments;		//!< address of segments
	u32 segmentCount;	//!< number of segments
} sysSpuImage;


s32 sysSpuElfGetInformation(const void *elf,u32 *entryPoint,u32 *segmentCount);
s32 sysSpuElfGetSegments(const void *elf,sysSpuSegment *segments,u32 segmentCount);

/*! \brief Load a SPU program file into a raw SPU's local store.
 \param spu
 The raw SPU identifier.
 \param path
 The SPU program file name.
 \param entryPoint
 Pointer to the returned entry point address.
 \return
 zero if no error occured, nonzero otherwise.
*/
s32 sysSpuRawLoad(sys_raw_spu_t spu,const char *path,u32 *entryPoint);

/*! \brief Load a SPU image into a raw SPU's local store.
 \param spu
 The raw SPU identifier.
 \param image
 Pointer to the SPU image structure.
 \return
 zero if no error occured, nonzero otherwise.
*/
s32 sysSpuRawImageLoad(sys_raw_spu_t spu,sysSpuImage *image);

/*! \brief Close an opened SPU image file.
 \param image
 Pointer to the SPU image structure.
 \return
 zero if no error occured, nonzero otherwise.
*/
s32 sysSpuImageClose(sysSpuImage *image);

/*! \brief Import a SPU image from an ELF file in memory.
 \param image
 Pointer to the updated image structure.
 \param elf
 Pointer to the ELF data in memory.
 \param type
 Type of the image file. Generally set to 0.
 \return
 zero if no error occured, nonzero otherwise.
*/
s32 sysSpuImageImport(sysSpuImage *image,const void *elf,u32 type);
/*! \brief Initialize Spu printf service on ppu.
 \param prio
 Priority of server
 \param entry
 Pointer to funtion callback on ppu side
 \return
 zero if no error occured, nonzero otherwise.
*/
s32 sysSpuPrintfInitialize(int prio, void (*entry)(void*));

/*! \brief Enable Spu printf service from each of the SPU thread in the specified SPU thread group.
 \param group
 SPU thread group ID to be attached
 \return
 zero if no error occured, nonzero otherwise.
*/
s32 sysSpuPrintfAttachGroup(sys_spu_group_t group);

/*! \brief Disable Spu printf service from each of the SPU thread in the specified SPU thread group.
 \param group
 SPU thread group ID to be detached
 \return
 zero if no error occured, nonzero otherwise.
*/
s32 sysSpuPrintfDetachGroup(sys_spu_group_t group);

/*! \brief Enable Spu printf service in the specified SPU thread.
 \param group
 SPU thread  ID to be attached
 \return
 zero if no error occured, nonzero otherwise.
*/
s32 sysSpuPrintfAttachThread(sys_spu_thread_t thread);

/*! \brief Disable Spu printf service in the specified SPU thread.
 \param thread
 SPU thread  ID to be detatached
 \return
 zero if no error occured, nonzero otherwise.
*/
s32 sysSpuPrintfDetach_Tread(sys_spu_thread_t thread);

/*! \brief Terminate Spu printf service
 \param none
 \return
 zero if no error occured, nonzero otherwise.
*/
s32 sysSpuPrintfFinalize();

#ifdef __cplusplus
	}
#endif

#endif
