#ifndef _VECTOR_H_
#define _VECTOR_H_

typedef struct Vector3
{
	long x;
	long y;
	long z;
} V3;

V3 Vec3(long x, long y, long z);
V3 AddVec3(V3 a, V3 b);

#endif
