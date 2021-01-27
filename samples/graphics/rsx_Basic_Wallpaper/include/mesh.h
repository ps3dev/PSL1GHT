#ifndef __MESH_H__
#define __MESH_H__

#include <ppu-types.h>
#include <vectormath/cpp/vectormath_aos.h>

#include <rsx/rsx.h>

#include "irrarray.h"

using namespace irr;
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

class SColor
{
public:
	SColor() : color(0) {};
	SColor(u32 c) : color(c) {};
	SColor(u8 a,u8 r,u8 g,u8 b) : R(r),G(g),B(b),A(a) {};
	SColor(const SColor& other) : color(other.color) {};

	u8 getRed() const { return R; }
	u8 getGreen() const { return G; }
	u8 getBlue() const { return B; }
	u8 getAlpha() const { return A; }

	union {
		u32 color;
		struct {
			u8 R,G,B,A;
		};
	};
};

struct S3DVertex
{
	S3DVertex() {};
	S3DVertex(f32 x,f32 y,f32 z,f32 nx,f32 ny,f32 nz,SColor c,f32 tu,f32 tv)
		: pos(x,y,z),nrm(nx,ny,nz),col(c),u(tu),v(tv) {};
	S3DVertex(const Vector3& _pos,const Vector3& _nrm,const SColor& c,f32 tu,f32 tv)
		: pos(_pos),nrm(_nrm),col(c),u(tu),v(tv) {};

	inline S3DVertex& operator=(const S3DVertex& other)
	{
		pos = other.pos;
		nrm = other.nrm;
		col = other.col;
		u = other.u;
		v = other.v;
		return *this;
	}

	Vector3 pos;
	Vector3 nrm;
	SColor col;

	f32 u,v;
};


class SMeshBuffer
{
public:
	SMeshBuffer() : pos_off(0),nrm_off(0),col_off(0),uv_off(0),ind_off(0) {};
	virtual ~SMeshBuffer()
	{
	}

	virtual const void* getVertices() const
	{
		return vertices.const_pointer();
	}

	virtual void* getVertices()
	{
		return vertices.pointer();
	}

	virtual u32 getVertexCount() const
	{
		return vertices.size();
	}

	virtual const u32* getIndices() const
	{
		return indices.const_pointer();
	}

	virtual u32* getIndices()
	{
		return indices.pointer();
	}

	virtual u32 getIndexCount() const
	{
		return indices.size();
	}

	core::array< u32, core::allocatorRSX< u32 > > indices;
	core::array< S3DVertex, core::allocatorRSX< S3DVertex > > vertices;

	u32 pos_off;
	u32 nrm_off;
	u32 col_off;
	u32 uv_off;

	u32 ind_off;
};

#endif
