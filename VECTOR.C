#include "vector.h"
#include "fx.h"

V3 Vec3(long x, long y, long z)
{
	V3 v;
	v.x = x;
	v.y = y;
	v.z = z;
	return v;
}

V3 addVec3(V3 a, V3 b)
{
	V3 out;

	out.x = a.x + b.x;
	out.y = a.y + b.y;
	out.z = a.z + b.z;

	return out;
}

V3 subVec3(V3 a, V3 b)
{
	V3 out;

	out.x = a.x - b.x;
	out.y = a.y - b.y;
	out.z = a.z - b.z;

	return out;
}

V3 cross(V3 a, V3 b)
{
	V3 out;

	out.x = FX_MUL(a.y, b.z) - FX_MUL(a.z, b.y);
	out.y = FX_MUL(a.x, b.z) - FX_MUL(a.z, b.x);
	out.z = FX_MUL(a.x, b.y) - FX_MUL(a.y, b.z);

	return out;
}

fx32 dot(V3 a, V3 b)
{
	return FX_MUL(a.x, b.x) + FX_MUL(a.y, b.y) + FX_MUL(a.z, b.z);
}
