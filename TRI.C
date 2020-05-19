#include  "tri.h"

#define RED 0xf800
#define GRN	0xf7f0
#define BLU	0x001f

void renderTri(Tri *pTri, void *pBuffer, unsigned int col)
{
	V3 *verts = pTri->verts;
	V3 *top = &verts[0];
	V3 *mid = &verts[1];
	V3 *bot = &verts[2];
	V3 *left;
	V3 *right;
	V3 *temp;

	int left, right;
	long x[2], y[2], dx[2], dy[2], sx[2], sy[2], err[2], err2[2];

	if(mid->y < top->y)
	{
		temp = top;
		top = mid;
		mid = top;
	}
	
	if(bot->y < top->y)
	{
		temp = top;
		top = bot;
		bot = temp;
	}
	
	if(bot->y < mid->y)
	{
		temp = bot;
		bot = mid;
		mid = temp;
	}
	
	x[0] = x[1] = top->x;
	y[0] = y[1] = top->y;
	
	if(mid->x <= bot->x)
	{
		left = mid;
		right = bot;
	}
	else
	{
		left = bot;
		right = mid;
	}

	dx[0] = (top->x < left->x ? left->x - top->x : top->x - left->x);
	dx[1] = (top->x < right->x ? right->x - top->x : top->x - left->x);

	long sx = (x1 < x2 ? 1 : -1);

	long dy = - (y1 < y2 ? y2 - y1 : y1 - y2);
	long sy = (y1 < y2 ? 1 : -1);
	long err = dx + dy;
	long err2;

	while(1)
	{
		*((unsigned int*)pBuffer + x + (y * (long)320)) = col;
		
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
		}
	}
}

void renderLine(long x1, long y1, long x2, long y2, void *pBuffer, unsigned int col)
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
		*((unsigned int*)pBuffer + x + (y * (long)320)) = col;
		
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
		}
	}
}