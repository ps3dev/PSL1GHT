#include <stdio.h>
#include <io/move.h>
#include <sys/memory.h>
#include <ppu-types.h>
#include <io/camera.h>
#include <ppu-types.h>
#include <spurs/spurs.h>
#include <sys/spu.h>
#include <sys/thread.h>
#include <sys/systime.h>

#include "../include/rsxutil.h"

#define SPURS_PREFIX_NAME "gemsample"

gemAttribute gem_attr;
gemInfo gem_info;
gemVideoConvertAttribute gem_video_convert;
gemState gem_state;
gemInertialState gem_inertial_state;
u16 oldGemPad = 0;
u16 newGemPad = 0;
u16 newGemAnalogT = 0;
extern int tracking;
extern cameraReadInfo camread;
extern cameraInfoEx camInf;
Spurs *spurs ATTRIBUTE_PRXPTR;
void *gem_memory ATTRIBUTE_PRXPTR;
extern rsxBuffer buffers[MAX_BUFFERS];
extern int currentBuffer;
float rx, ry;

int dx = 200;			// default step x
int dy = 100;			// default step y
gemImageState image_state;
void *buffer_mem ATTRIBUTE_PRXPTR;
void *video_out ATTRIBUTE_PRXPTR;
u8 video_frame[640*480*4];
extern int tracking;
extern int pos_x;
extern int pos_y;

static inline float
vec_array (vec_float4 vec, unsigned int idx)
{
  union {
    vec_float4 vec;
    float array[4];
  } v;

  v.vec = vec;

  if (idx > 3)
    return -1;
  return v.array[idx];
}

int
proccessGem (int t)
{

  int ret;

  switch (t) {

    case 0:

      ret = gemUpdateStart (camread.buffer, camread.timestamp);

      if (ret != 0) {
	printf ("Return from gemUpdateStart %X\n", ret);
      }
      break;
    case 1:

      ret = gemConvertVideoStart (camread.buffer);

      if (ret != 0) {
	printf ("Return from gemConvertVideoStart %X\n", ret);
      }
      break;
    case 2:

      ret = gemUpdateFinish ();
      if (ret != 0) {
	printf ("Return from gemUpdateFinish %X\n", ret);
      }
      break;
    case 3:
      ret = gemConvertVideoFinish ();
      if (ret != 0) {
	printf ("Return from gemConvertVideoFinish %X\n", ret);
      }
      break;
    default:
      ret = -1;
      break;

  }
  return ret;

}

int
initSpurs ()
{
  int ret;
  int i;
  sys_ppu_thread_t ppu_thread_id;
  int ppu_prio;
  unsigned int nthread;

  ret = sysSpuInitialize (6, 0);
  printf ("sysSpuInitialize return %d\n", ret);

  ret = sysThreadGetId (&ppu_thread_id);
  printf ("sysThreadGetId return %d ppu_thread_id %x\n", ret, ppu_thread_id);

  ret = sysThreadGetPriority (ppu_thread_id, &ppu_prio);
  printf ("sysThreadGetPriority return %d ppu_prio %d\n", ret, ppu_prio);

  /* initialize spurs */
  printf ("Initializing spurs\n");
  spurs = (void *) memalign (SPURS_ALIGN, sizeof (Spurs));
  printf ("Initializing spurs attribute\n");
  SpursAttribute attributeSpurs;

  ret = spursAttributeInitialize (&attributeSpurs, 5, 250, ppu_prio - 1, true);
  if (ret) {
    printf ("error : spursAttributeInitialize failed  %x\n", ret);
    return (ret);
  }

  printf ("Setting name prefix\n");
  ret =
      spursAttributeSetNamePrefix (&attributeSpurs, SPURS_PREFIX_NAME,
      strlen (SPURS_PREFIX_NAME));
  if (ret) {
    printf ("error : spursAttributeInitialize failed %x\n", ret);
    return (ret);
  }

  printf ("Initializing with attribute\n");
  ret = spursInitializeWithAttribute (spurs, &attributeSpurs);
  if (ret) {
    printf ("error: spursInitializeWithAttribute failed  %x\n", ret);
    return (ret);
  }

  ret = spursGetNumSpuThread (spurs, &nthread);
  if (ret) {
    printf ("error: spursGetNumSpuThread failed %x\n", ret);
  }

  sys_spu_thread_t *threads =
      (sys_spu_thread_t *) malloc (sizeof (sys_spu_thread_t) * nthread);

  ret = spursGetSpuThreadId (spurs, threads, &nthread);
  if (ret) {
    printf ("error: spursGetSpuThreadId failed %x\n", ret);
  }

  printf ("SPURS %d spu threads availables\n", nthread);
  for (i = 0; i < nthread; i++) {
    printf ("SPU Number:%d\tSPU Thread ID:%x\n", i, threads[i]);
  }
  printf ("\n");

  printf ("checking SpursInfo\n");
  SpursInfo info;

  ret = spursGetInfo (spurs, &info);
  if (ret) {
    printf ("error: spursGetInfo failed %x\n", ret);
  }
  printf ("SpursInfo: \n");
  printf ("nSpus=%d \n", info.nSpus);
  printf ("spuGroupPriority=%d \n", info.spuGroupPriority);
  printf ("ppuThreadPriority=%d \n", info.ppuThreadPriority);
  printf ("exitIfNoWork=%d \n", info.exitIfNoWork);
  printf ("namePrefix=%s \n", info.namePrefix);
  for (i = 0; i < info.nSpus; i++) {
    printf ("SPU Number:%d\tSPU Thread ID:%x\n", i, info.spuThreads[i]);
  }
  printf ("SPURS initialized correctly!!!\n");

}

int
endSpurs ()
{
  spursFinalize (spurs);

  return 0;
}

int
endGem ()
{
  endSpurs ();
  gemEnd ();
  return 0;
}

static inline void
initAttributeGem (gemAttribute * attribute, u32 max_connect, void *memory_ptr,
    Spurs * spurs, const u8 spu_priorities[8])
{
  int i;

  attribute->version = 2;
  attribute->max = max_connect;
  attribute->spurs = spurs;
  attribute->memory = memory_ptr;
  for (i = 0; i < 8; ++i)
    attribute->spu_priorities[i] = spu_priorities[i];
}
int
initGemVideoConvert()
{
  int ret;
  printf("Preparing GemVideoConvert structure \n");	
  gem_video_convert.version=2;
  gem_video_convert.format=2; //GEM_RGBA_640x480; 
  gem_video_convert.conversion= GEM_AUTO_WHITE_BALANCE|GEM_COMBINE_PREVIOUS_INPUT_FRAME|GEM_FILTER_OUTLIER_PIXELS|GEM_GAMMA_BOOST; 
  gem_video_convert.gain=1.0f;
  gem_video_convert.red_gain=1.0f;
  gem_video_convert.green_gain=1.0f;
  gem_video_convert.blue_gain=1.0f;	
  buffer_mem=memalign(128,640*480);
  video_out=(void *)video_frame;
  gem_video_convert.buffer_memory=buffer_mem;
  gem_video_convert.video_data_out=video_out;
  gem_video_convert.alpha=255;		
  ret=gemPrepareVideoConvert(&gem_video_convert); 
  return ret;
}
int
initGem ()
{
  int ret;
  int i;

  initSpurs ();

  ret = gemGetMemorySize (1);
  printf
      ("return from GemGetMemorySize %X size in bytes needed for move device to init libgem\n",
      ret);
  gem_memory = malloc (ret);

  printf
      ("preparing GemAttribute structure with sprus and memory stuff is very important align correctly spurs structure \n");
  u8 gem_spu_priorities[8] = { 1, 1, 1, 1, 1, 0, 0, 0 };	// execute
								// libgem jobs
								// on 5 spu
  gemAttribute gem_attr;

  initAttributeGem (&gem_attr, 1, gem_memory, spurs, gem_spu_priorities);

  printf
      ("calling GemInit with GemAttribute structure version=%d max_connect=%d spurs=%X memory_ptr=%X  \n",
      gem_attr.version, gem_attr.max, gem_attr.spurs, gem_attr.memory);
  ret = gemInit (&gem_attr);
  printf ("return from GemInit %X \n", ret);
  ret= initGemVideoConvert();
  printf ("return from initGemVideoConvert %X \n", ret);
  ret = gemPrepareCamera (128, 0.5);
  printf ("GemPrepareCamera return %d exposure set to 128 and quality to 0.5\n",
      ret);
  ret = gemReset (0);
  printf ("GemReset return %X \n", ret);
  return ret;

}

void
readGemPad (int num_gem)
{
  int ret;
  unsigned int hues[] = { 4 << 24, 4 << 24, 4 << 24, 4 << 24 };
  ret = gemGetState (0, 0, -22000, &gem_state);

  newGemPad = gem_state.paddata.buttons & (~oldGemPad);
  newGemAnalogT = gem_state.paddata.ANA_T;
  oldGemPad = gem_state.paddata.buttons;

  switch (ret) {
    case 2:
      gemForceRGB (num_gem, 0.5, 0.5, 0.5);
      break;
    case 5:

      gemTrackHues (hues, NULL);
      break;
    default:
      break;
  }

}

void
readGemAccPosition (int num_gem)
{
  vec_float4 position;

  gemGetAccelerometerPositionInDevice (num_gem, &position);

  printf (" accelerometer device coordinates [%f,%f,%f,%f]\n",
      vec_array (position, 0), vec_array (position, 1), vec_array (position, 2),
      vec_array (position, 3));

}

void
readGemInertial (int num_gem)
{
  int ret;

  ret = gemGetInertialState (num_gem, 0, -22000, &gem_inertial_state);
  printf ("gemGetInertialState return %X\n", ret);
  printf ("counter %d temperature %f\n", gem_inertial_state.counter,
      gem_inertial_state.temperature);

  printf (" accelerometer sensor   [%f,%f,%f,%f]\n",
      vec_array (gem_inertial_state.accelerometer, 0),
      vec_array (gem_inertial_state.accelerometer, 1),
      vec_array (gem_inertial_state.accelerometer, 2),
      vec_array (gem_inertial_state.accelerometer, 3));

  printf (" accelerometer_bias sensor   [%f,%f,%f,%f]\n",
      vec_array (gem_inertial_state.accelerometer_bias, 0),
      vec_array (gem_inertial_state.accelerometer_bias, 1),
      vec_array (gem_inertial_state.accelerometer_bias, 2),
      vec_array (gem_inertial_state.accelerometer_bias, 3));

  printf (" gyro sensor  [%f,%f,%f,%f]\n", vec_array (gem_inertial_state.gyro,
	  0), vec_array (gem_inertial_state.gyro, 1),
      vec_array (gem_inertial_state.gyro, 2),
      vec_array (gem_inertial_state.gyro, 3));

  printf (" gyro_bias sensor  [%f,%f,%f,%f]\n",
      vec_array (gem_inertial_state.gyro_bias, 0),
      vec_array (gem_inertial_state.gyro_bias, 1),
      vec_array (gem_inertial_state.gyro_bias, 2),
      vec_array (gem_inertial_state.gyro_bias, 3));

}

void
readGem ()
{

  proccessGem (0);
  proccessGem (1);
  proccessGem (2);
  proccessGem (3);
  readGemPad (0);		// This will read buttons from Move
  switch (newGemPad) {
    case 1:
      printf ("Select pressed \n");
      break;
    case 2:
      printf ("T pressed value %d\n", newGemAnalogT);
      printf
	  ("Frame %d center of the sphere in world coordinates %f %f %f %f \n",
	  camread.frame, vec_array (gem_state.pos, 0), vec_array (gem_state.pos,
	      1), vec_array (gem_state.pos, 2), vec_array (gem_state.pos, 3));
      break;
    case 4:
      printf ("Move pressed \n");
      gemCalibrate (0);
      break;
    case 8:
      printf ("Start pressed \n");
      pos_x = 0;
      pos_y = 0;
      break;
    case 16:
      printf ("Triangle pressed \n");
      getImageState ();
      break;
    case 32:
      printf ("Circle pressed \n");
      break;
    case 64:
      printf ("Cross pressed \n");
      printf ("X,Y,Z position (mm) %f %f %f\n", vec_array (gem_state.pos, 0),
	  vec_array (gem_state.pos, 1), vec_array (gem_state.pos, 2));
      readGemAccPosition (0);
      break;
    case 128:
      printf ("Square pressed \n");
      readGemInertial (0);
      break;
    default:
      break;
  }

}

void
getImageState ()
{
  int ret;

  gemImageState imgState;

  gemGetImageState (0, &imgState);
  printf (" u  [%f]\n", imgState.u);
  printf (" v  [%f]\n", imgState.v);
  printf (" r  [%f]\n", imgState.r);
  printf (" projectionx  [%f]\n", imgState.projectionx);
  printf (" projectiony  [%f]\n", imgState.projectiony);
  printf (" distance  [%f]\n", imgState.distance);
  printf ("visible=%d r_valid=%d\n", imgState.visible, imgState.r_valid);
  printf ("tiemestamp=%Ld\n", imgState.frame_time);
}
