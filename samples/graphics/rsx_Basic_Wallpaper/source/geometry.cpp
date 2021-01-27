#include <rsx/rsx.h>

#include "geometry.h"


SMeshBuffer* createQuad(Point3 P1, Point3 P2, Point3 P3, Point3 P4)
{
	u32 i;
	SColor col(255, 255, 255, 255);
	SMeshBuffer* buffer = new SMeshBuffer();
	const u16 u[6] = { 0,1,2,   0,3,1 };

	buffer->indices.set_used(6);

	for (i = 0; i < 6; i++) buffer->indices[i] = u[i];

	buffer->vertices.set_used(4);

	//                              position, normal,    texture
	buffer->vertices[0] = S3DVertex(P1.getX(), P1.getY(), P1.getZ(), -1, -1, -1, col, 0, 1);
	buffer->vertices[1] = S3DVertex(P2.getX(), P2.getY(), P2.getZ(),  1, -1, -1, col, 1, 0);
	buffer->vertices[2] = S3DVertex(P3.getX(), P3.getY(), P3.getZ(),  1,  1, -1, col, 0, 0);
	buffer->vertices[3] = S3DVertex(P4.getX(), P4.getY(), P4.getZ(), -1,  1, -1, col, 1, 1);

	
	rsxAddressToOffset(&buffer->vertices[0].pos, &buffer->pos_off);
	rsxAddressToOffset(&buffer->vertices[0].nrm, &buffer->nrm_off);
	rsxAddressToOffset(&buffer->vertices[0].col, &buffer->col_off);
	rsxAddressToOffset(&buffer->vertices[0].u, &buffer->uv_off);
	rsxAddressToOffset(&buffer->indices[0], &buffer->ind_off);

	return buffer;
}

SMeshBuffer* createQuad(f32 size, float z)
{
	u32 i;
	SColor col(255, 255, 255, 255);
	SMeshBuffer* buffer = new SMeshBuffer();
	const u16 u[6] = { 0,1,2,   0,3,1 };

	buffer->indices.set_used(6);

	for (i = 0; i < 6; i++) buffer->indices[i] = u[i];

	buffer->vertices.set_used(4);

	//                              position, normal,    texture
	buffer->vertices[0] = S3DVertex(-1.0, -1.0, z, -1, -1, -1, col, 0, 1);
	buffer->vertices[1] = S3DVertex( 1.0,  1.0, z,  1, -1, -1, col, 1, 0);
	buffer->vertices[2] = S3DVertex(-1.0,  1.0, z,  1,  1, -1, col, 0, 0);
	buffer->vertices[3] = S3DVertex( 1.0, -1.0, z, -1,  1, -1, col, 1, 1);

	//centre and resize
	//for (i = 0; i < 4; i++) {
	//	//center
	//	buffer->vertices[i].pos += Vector3(-0.5f, 0.5f, 0.0f);
	//	//resize
	//	buffer->vertices[i].pos *= size;
	//}

	rsxAddressToOffset(&buffer->vertices[0].pos, &buffer->pos_off);
	rsxAddressToOffset(&buffer->vertices[0].nrm, &buffer->nrm_off);
	rsxAddressToOffset(&buffer->vertices[0].col, &buffer->col_off);
	rsxAddressToOffset(&buffer->vertices[0].u, &buffer->uv_off);
	rsxAddressToOffset(&buffer->indices[0], &buffer->ind_off);

	return buffer;
}

SMeshBuffer* createCube(f32 size)
{
	u32 i;
	SColor col(255,255,255,255);
	SMeshBuffer *buffer = new SMeshBuffer();
	const u16 u[36] = {   0,1,2,   0,2,3,   1,4,5,   1,5,2,   4,7,6,	 4,6,5, 
						  7,0,3,   7,3,6,   9,2,5,   9,5,8,   0,10,11,   0,7,10};

	buffer->indices.set_used(36);
	for(i=0;i<36;i++) buffer->indices[i] = u[i];

	buffer->vertices.set_used(12);

	buffer->vertices[0] = S3DVertex(0,0,0, -1,-1,-1, col, 1, 0);
	buffer->vertices[1] = S3DVertex(1,0,0,  1,-1,-1, col, 1, 1);
	buffer->vertices[2] = S3DVertex(1,1,0,  1, 1,-1, col, 0, 1);
	buffer->vertices[3] = S3DVertex(0,1,0, -1, 1,-1, col, 0, 0);
	buffer->vertices[4] = S3DVertex(1,0,1,  1,-1, 1, col, 1, 0);
	buffer->vertices[5] = S3DVertex(1,1,1,  1, 1, 1, col, 0, 0);
	buffer->vertices[6] = S3DVertex(0,1,1, -1, 1, 1, col, 0, 1);
	buffer->vertices[7] = S3DVertex(0,0,1, -1,-1, 1, col, 1, 1);
	buffer->vertices[8] = S3DVertex(0,1,1, -1, 1, 1, col, 1, 0);
	buffer->vertices[9] = S3DVertex(0,1,0, -1, 1,-1, col, 1, 1);
	buffer->vertices[10] = S3DVertex(1,0,1,  1,-1, 1, col, 0, 1);
	buffer->vertices[11] = S3DVertex(1,0,0,  1,-1,-1, col, 0, 0);

	for(i=0;i<12;i++) {
		buffer->vertices[i].pos -= Vector3(0.5f,0.5f,0.5f);
		buffer->vertices[i].pos *= size;
	}

	rsxAddressToOffset(&buffer->vertices[0].pos,&buffer->pos_off);
	rsxAddressToOffset(&buffer->vertices[0].nrm,&buffer->nrm_off);
	rsxAddressToOffset(&buffer->vertices[0].col,&buffer->col_off);
	rsxAddressToOffset(&buffer->vertices[0].u,&buffer->uv_off);
	rsxAddressToOffset(&buffer->indices[0],&buffer->ind_off);

	return buffer;
}

SMeshBuffer* createDonut(f32 outerRadius,f32 innerRadius,u32 polyCntX,u32 polyCntY)
{
	u32 i,x,y,level;
	SColor col(100,255,255,255);
	SMeshBuffer *buffer = new SMeshBuffer();

	if(polyCntX<2) polyCntX = 2;
	if(polyCntY<2) polyCntY = 2;
	while(polyCntX*polyCntY>32767) {
		polyCntX /= 2;
		polyCntY /= 2;
	}

	f32 ay = 0;
	const f32 angleX = 2*M_PI/polyCntX;
	const f32 angleY = 2*M_PI/polyCntY;
	const u32 polyCntXpitch = polyCntX +1;
	const u32 polyCntYpitch = polyCntY + 1;

	buffer->vertices.set_used(polyCntYpitch*polyCntXpitch);
	buffer->indices.set_used(polyCntY*polyCntX*6);

	rsxAddressToOffset(&buffer->vertices[0].pos,&buffer->pos_off);
	rsxAddressToOffset(&buffer->vertices[0].nrm,&buffer->nrm_off);
	rsxAddressToOffset(&buffer->vertices[0].col,&buffer->col_off);
	rsxAddressToOffset(&buffer->vertices[0].u,&buffer->uv_off);
	rsxAddressToOffset(&buffer->indices[0],&buffer->ind_off);

	i = 0;
	for(y=0;y<=polyCntY;y++) {
		f32 axz = 0;

		const f32 sinay = sinf(ay);
		const f32 cosay = cosf(ay);
		const f32 tu = (f32)y/(f32)polyCntY;
		for(x=0;x<=polyCntX;x++) {
			const Vector3 pos(static_cast<f32>((outerRadius - (innerRadius*cosf(axz)))*cosay),
									  static_cast<f32>((outerRadius - (innerRadius*cosf(axz)))*sinay),
									  static_cast<f32>(innerRadius*sinf(axz)));
			
			const Vector3 nrm(static_cast<f32>(-cosf(axz)*cosay),
									  static_cast<f32>(-cosf(axz)*sinay),
									  static_cast<f32>(sinf(axz)));

			buffer->vertices[i] = S3DVertex(pos,nrm,col,tu,(f32)x/(f32)polyCntX);

			axz += angleX;
			i++;
		}
		ay += angleY;
	}

	i = 0;
	level = 0;
	for(y=0;y<polyCntY;y++) {
		for(x=0;x<polyCntX - 1;x++) {
			const u32 curr = level + x;
			buffer->indices[i++] = curr;
			buffer->indices[i++] = curr + polyCntXpitch;
			buffer->indices[i++] = curr + 1 + polyCntXpitch;
			
			buffer->indices[i++] = curr;
			buffer->indices[i++] = curr + 1 + polyCntXpitch;
			buffer->indices[i++] = curr + 1;
		}

		buffer->indices[i++] = level + polyCntX;
		buffer->indices[i++] = level + polyCntX - 1;
		buffer->indices[i++] = level + polyCntX - 1 + polyCntXpitch;
		
		buffer->indices[i++] = level + polyCntX;
		buffer->indices[i++] = level + polyCntX - 1 + polyCntXpitch;
		buffer->indices[i++] = level + polyCntX + polyCntXpitch;

		level += polyCntXpitch;
	}

	return buffer;
}

SMeshBuffer* createSphere(f32 radius,u32 polyCntX,u32 polyCntY)
{
	u32 i,p1,p2,level;
	u32 x,y,polyCntXpitch;
	const f32 RECIPROCAL_PI = 1.0f/M_PI;
	SMeshBuffer *buffer = new SMeshBuffer();

	if(polyCntX<2) polyCntX = 2;
	if(polyCntY<2) polyCntY = 2;
	if(polyCntX*polyCntY>32767) {
		if(polyCntX>polyCntY) 
			polyCntX = 32767/polyCntY-1;
		else
			polyCntY = 32767/(polyCntX+1);
	}
	polyCntXpitch = polyCntX+1;

	buffer->vertices.set_used((polyCntXpitch*polyCntY) + 2);
	buffer->indices.set_used((polyCntX*polyCntY)*6);

	rsxAddressToOffset(&buffer->vertices[0].pos,&buffer->pos_off);
	rsxAddressToOffset(&buffer->vertices[0].nrm,&buffer->nrm_off);
	rsxAddressToOffset(&buffer->vertices[0].col,&buffer->col_off);
	rsxAddressToOffset(&buffer->vertices[0].u,&buffer->uv_off);
	rsxAddressToOffset(&buffer->indices[0],&buffer->ind_off);

	i = 0;
	level = 0;
	for(p1=0;p1<polyCntY-1;p1++) {
		for(p2=0;p2<polyCntX-1;p2++) {
			const u32 curr = level + p2;
			buffer->indices[i++] = curr;
			buffer->indices[i++] = curr + polyCntXpitch;
			buffer->indices[i++] = curr + 1 + polyCntXpitch;

			buffer->indices[i++] = curr;
			buffer->indices[i++] = curr + 1 + polyCntXpitch;
			buffer->indices[i++] = curr + 1;
		}

		buffer->indices[i++] = level + polyCntX;
		buffer->indices[i++] = level + polyCntX - 1;
		buffer->indices[i++] = level + polyCntX - 1 + polyCntXpitch;

		buffer->indices[i++] = level + polyCntX;
		buffer->indices[i++] = level + polyCntX - 1 + polyCntXpitch;
		buffer->indices[i++] = level + polyCntX + polyCntXpitch;

		level += polyCntXpitch;
	}

	const u32 polyCntSq = polyCntXpitch*polyCntY;
	const u32 polyCntSq1 = polyCntSq+1;
	const u32 polyCntSqM1 = (polyCntY-1)*polyCntXpitch;

	for(p2=0;p2<polyCntX-1;p2++) {
		buffer->indices[i++] = polyCntSq;
		buffer->indices[i++] = p2;
		buffer->indices[i++] = p2+1;

		buffer->indices[i++] = polyCntSq1;
		buffer->indices[i++] = polyCntSqM1+p2;
		buffer->indices[i++] = polyCntSqM1+p2+1;
	}

	buffer->indices[i++] = polyCntSq;
	buffer->indices[i++] = polyCntX-1;
	buffer->indices[i++] = polyCntX;

	buffer->indices[i++] = polyCntSq1;
	buffer->indices[i++] = polyCntSqM1;
	buffer->indices[i++] = polyCntSqM1+polyCntX-1;

	f32 axz;
	f32 ay = 0;
	SColor col(100,255,255,255);
	const f32 angelX = 2*M_PI/polyCntX;
	const f32 angelY = M_PI/polyCntY;

	i = 0;
	for(y=0;y<polyCntY;y++) {
		axz = 0;
		ay += angelY;
		const f32 sinay = sinf(ay);
		for(x=0;x<polyCntX;x++) {
			const Vector3 pos(static_cast<f32>(radius*cosf(axz)*sinay), static_cast<f32>(radius*cosf(ay)), static_cast<f32>(radius*sinf(axz)*sinay));
			
			Vector3 normal = normalize(pos);
			
			f32 tu = 0.5F;
			if(y==0) {
				if(normal.getY()!=-1.0F && normal.getY()!=1.0F)
					tu = static_cast<f32>(acosf(clamp(normal.getX()/sinay,-1.0f,1.0f))*0.5F*RECIPROCAL_PI);
				if(normal.getZ()<0.0F)
					tu = 1-tu;
			} else
				tu = buffer->vertices[i - polyCntXpitch].u;

			buffer->vertices[i] = S3DVertex(pos,normal,col,tu,static_cast<f32>(ay*RECIPROCAL_PI));
			axz += angelX;
			i++;
		}
		buffer->vertices[i] = S3DVertex(buffer->vertices[i-polyCntX]);
		buffer->vertices[i].u = 1.0F;
		i++;
	}

	buffer->vertices[i++] = S3DVertex(0.0F,radius,0.0F,0.0F,1.0F,0.0F,col,0.5F,0.0F);
	buffer->vertices[i] = S3DVertex(0.0F,-radius,0.0F,0.0F,-1.0F,0.0F,col,0.5F,1.0F);

	return buffer;
}
