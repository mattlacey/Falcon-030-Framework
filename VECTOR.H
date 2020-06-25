#ifndef _VECTOR_H_
#define _VECTOR_H_

#include "fx.h"

typedef struct Vector3
{
	fx32 x;
	fx32 y;
	fx32 z;
} V3;

V3 Vec3(long x, long y, long z);

void printV3(V3 v);
V3 addVec3(V3 a, V3 b);
V3 subVec3(V3 a, V3 b);
V3 cross(V3 a, V3 b);
fx32 dot(V3 a, V3 b);
void normalize(V3 *a);

#endif
