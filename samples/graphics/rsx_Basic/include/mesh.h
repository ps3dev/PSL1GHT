#ifndef __MESH_H__
#define __MESH_H__

#include <vectormath/cpp/vectormath_aos.h>

using namespace Vectormath::Aos;

template< class T >
inline const T& min_(const T& a,const T& b)
{
	return a<b?a:b;
}

template< class T >
inline const T& max_(const T& a,const T& b)
{
	return a<b?b:a;
}

template< class T >
inline const T clamp(const T& val,const T& low,const T& high)
{
	return min_(max_(val,low),high);
}

struct S3DVertex
{
	S3DVertex() {};
	S3DVertex(f32 x,f32 y,f32 z,f32 nx,f32 ny,f32 nz,f32 tu,f32 tv)
		: pos(x,y,z),nrm(nx,ny,nz),u(tu),v(tv) {};

	inline S3DVertex& operator=(const S3DVertex& other)
	{
		pos = other.pos;
		nrm = other.nrm;
		u = other.u;
		v = other.v;
		return *this;
	}

	Vector3 pos;
	Vector3 nrm;

	f32 u,v;
};

template< class T >
class CMeshBuffer
{
public:
	CMeshBuffer() : indices(NULL),cnt_indices(0),vertices(NULL),cnt_vertices(0) {};

	u16 *indices;
	u32 cnt_indices;

	S3DVertex *vertices;
	u32 cnt_vertices;
};

typedef CMeshBuffer<S3DVertex> SMeshBuffer;

#endif
