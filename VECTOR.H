#ifndef _VECTOR_H_
#define _VECTOR_H_

#include "fx.h"

typedef struct Vector3
{
	long x;
	long y;
	long z;
} V3;

V3 Vec3(long x, long y, long z);

V3 addVec3(V3 a, V3 b);
V3 subVec3(V3 a, V3 b);
V3 cross(V3 a, V3 b);
fx32 dot(V3 a, V3 b);
void normalize(V3 *a);

#endif
