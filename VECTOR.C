#include <stdio.h>

#include "vector.h"
#include "fx.h"

/* Probably wrong types */
#define ALPHA_MAX (fx32)(0.93980863517232 * FX_ONE)
#define BETA_MED (fx32)(0.389281482723724 * FX_ONE)
#define GAMMA_MIN (fx32)(0.29870618761437 * FX_ONE)

V3 Vec3(long x, long y, long z)
{
	V3 v;
	v.x = x;
	v.y = y;
	v.z = z;
	return v;
}

void printV3(V3 v)
{
	printf("(%ld, %ld, %ld)\n", v.x, v.y, v.z);
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
	out.z = FX_MUL(a.x, b.y) - FX_MUL(a.y, b.x);

	return out;
}

fx32 dot(V3 a, V3 b)
{
	return FX_MUL(a.x, b.x) + FX_MUL(a.y, b.y) + FX_MUL(a.z, b.z);
}

void normalize(V3 *v)
{
	/* https://math.stackexchange.com/a/3182291/65473 */
	fx32 a, b, c, temp, len;
	V3 tv;

	a = fx32abs(v->x);
	b = fx32abs(v->y);
	c = fx32abs(v->z);

	if(b < a)
	{
		temp = a;
		a = b;
		b = temp;
	}

	if(c < b)
	{
		temp = b;
		b = c;
		c = temp;
	}

	if(b < a)
	{
		temp = a;
		a = b;
		b = temp;
	}

	len = FX_MUL(ALPHA_MAX, c) + FX_MUL(BETA_MED, b) + FX_MUL(GAMMA_MIN, a);
	len = (c > len ? c : len);

	if(len)
	{
		v->x = FX_DIV(v->x, len);
		v->y = FX_DIV(v->y, len);
		v->z = FX_DIV(v->z, len);
	}
}
