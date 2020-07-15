#include "tri.h"
#include "framewrk.h"

#include <stdio.h>

void printTri(Tri *pTri)
{
	printf("(%ld, %ld, %ld)  (%ld, %ld, %ld)  (%ld, %ld, %ld)\n",
		pTri->verts[0].x, pTri->verts[0].y, pTri->verts[0].z,
		pTri->verts[1].x, pTri->verts[1].y, pTri->verts[1].z,
		pTri->verts[2].x, pTri->verts[2].y, pTri->verts[2].z);
}

Tri makeTri(V3 vec1, V3 vec2, V3 vec3, unsigned col)
{
	Tri t;
	t.verts[0] = vec1;
	t.verts[1] = vec2;
	t.verts[2] = vec3;
	t.col = col;
	return t;
}

void triToScreen(Tri *pTri)
{
	pTri->verts[0].x = (pTri->verts[0].x >> FX_HSHIFT) + 160;
	pTri->verts[1].x = (pTri->verts[1].x >> FX_HSHIFT) + 160;
	pTri->verts[2].x = (pTri->verts[2].x >> FX_HSHIFT) + 160;

	pTri->verts[0].y = (pTri->verts[0].y >> FX_HSHIFT) + 120;
	pTri->verts[1].y = (pTri->verts[1].y >> FX_HSHIFT) + 120;
	pTri->verts[2].y = (pTri->verts[2].y >> FX_HSHIFT) + 120;
}

void renderTri(Tri t, void *pBuffer)
{
	V3 *verts = t.verts;
	V3 *top = &verts[0];
	V3 *mid = &verts[1];
	V3 *bot = &verts[2];
	V3 *temp;

	long bounds[2][240];
	long *left, *right;
	int i, iMin, iMax;

	if(mid->y < top->y)
	{
		temp = top;
		top = mid;
		mid = temp;
	}

	if(bot->y < mid->y)
	{
		temp = bot;
		bot = mid;
		mid = temp;
	}

	if(mid->y < top->y)
	{
		temp = top;
		top = mid;
		mid = temp;
	}

	if(top->y != mid ->y)
	{
		calcSpanBounds(bounds[0], top->x, top->y, mid->x, mid->y);
	}

	if(mid->y != bot->y)
	{
		calcSpanBounds(bounds[0], mid->x, mid->y, bot->x, bot->y);
	}

	calcSpanBounds(bounds[1], top->x, top->y, bot->x, bot->y);

#ifdef FILL
	iMin = top->y < 0 ? 0 : top->y;
	iMax = bot->y >= 240 ? 239 : bot->y;

	if(bounds[0][mid->y] < bounds[1][mid->y])
	{
		left = bounds[0];
		right = bounds[1];
	}
	else
	{
		left = bounds[1];
		right = bounds[0];
	}

	for(i = iMin; i < iMax; i++)
	{
		renderSpan(left[i], right[i], i, t.col, pBuffer);
	}
#endif

#ifdef WIREFRAME
	renderLine(top->x, top->y, bot->x, bot->y, GRN, pBuffer);
	renderLine(top->x, top->y, mid->x, mid->y, GRN, pBuffer);
	renderLine(mid->x, mid->y, bot->x, bot->y, GRN, pBuffer);
#endif

#ifdef POINTS
	renderPoint(top->x, top->y, YEL, pBuffer);
	renderPoint(mid->x, mid->y, MAG, pBuffer);
	renderPoint(bot->x, bot->y, CYN, pBuffer);
#endif
}

void calcSpanBounds(long *boundBuffer, long x1, long y1, long x2, long y2)
{
	long x = x1;
	long y = y1;
	long dx = (x1 < x2 ? x2 - x1 : x1 - x2);
	long sx = (x1 < x2 ? 1 : -1);

	long dy = - (y1 < y2 ? y2 - y1 : y1 - y2);
	long sy = (y1 < y2 ? 1 : -1);
	long err = dx + dy;
	long err2;

	while(1)
	{
		if(x == x2 && y == y2)
		{
			if(y >= 0 && y < 239)
			{
				boundBuffer[y] = x;
			}
			break;
		}

		err2 = 2 * err;

		if(err2 >= dy)
		{
			err += dy;
			x += sx;
		}

		if(err2 <= dx)
		{
			if(y >= 0 && y < 239)
			{
				boundBuffer[y] = x;
			}
			err += dx;
			y += sy;
		}
	}
}

void renderSpan(long x1, long x2, long y, unsigned col, void *pBuffer)
{
	long i;

	for(i = x1; i < x2; i++)
	{
		*((unsigned int*)pBuffer + i + (y * (long)320)) = col;
	}
}

void renderLine(long x1, long y1, long x2, long y2, unsigned col, void *pBuffer)
{
	long x = x1;
	long y = y1;
	long dx = (x1 < x2 ? x2 - x1 : x1 - x2);
	long sx = (x1 < x2 ? 1 : -1);

	long dy = - (y1 < y2 ? y2 - y1 : y1 - y2);
	long sy = (y1 < y2 ? 1 : -1);
	long err = dx + dy;
	long err2;

	long yoffset = (y * (long)320);

	while(1)
	{
		*((unsigned int*)pBuffer + x + yoffset) = col;

		if(x == x2 && y == y2) break;

		err2 = 2 * err;

		if(err2 >= dy)
		{
			err += dy;
			x += sx;
		}

		if(err2 <= dx)
		{
			err += dx;
			y += sy;
			yoffset += 320;
		}
	}
}

void renderPoint(long x, long y, unsigned col, void *pBuffer)
{
	*((unsigned int*)pBuffer + x + (y * (long)320)) = col;
}
