#include <spu_intrinsics.h>

int main()
{
	spu_writech(SPU_WrOutMbox,0x1337BAAD);
	return 0;
}
