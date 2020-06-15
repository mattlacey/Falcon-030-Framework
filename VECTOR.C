#include "vector.h"

V3 Vec3(long x, long y, long z)
{
	V3 v;
	v.x = x;
	v.y = y;
	v.z = z;
	return v;
}

V3 AddVec3(V3 a, V3 b)
{
	V3 out;

	out.x = a.x + b.x;
	out.y = a.y + b.y;
	out.z = a.z + b.z;

	return out;
}
