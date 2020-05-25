#include  "tri.h"

#define RED 0xf800
#define GRN	0xf7f0
#define BLU	0x001f

Tri makeTri(V3 vec1, V3 vec2, V3 vec3, unsigned col)
{
	Tri t;
	t.verts[0] = vec1;
	t.verts[1] = vec2;
	t.verts[2] = vec3;
	t.col = col;
	return t;
}

void renderTri(Tri *pTri, void *pBuffer)
{
	V3 *verts = pTri->verts;
	V3 *top = &verts[0];
	V3 *mid = &verts[1];
	V3 *bot = &verts[2];
	V3 *temp;

	long bounds[2][240];
	int i;

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
	
	if(mid->x <= bot->x)
	{
		calcSpanBounds(bounds[0], top->x, top->y, mid->x, mid->y); 
		calcSpanBounds(bounds[0], mid->x, mid->y, bot->x, bot->y); 
		calcSpanBounds(bounds[1], top->x, top->y, bot->x, bot->y); 
	}
	else
	{
		calcSpanBounds(bounds[1], top->x, top->y, mid->x, mid->y); 
		calcSpanBounds(bounds[1], mid->x, mid->y, bot->x, bot->y); 
		calcSpanBounds(bounds[0], top->x, top->y, bot->x, bot->y); 
	}

	renderLine(top->x, top->y, bot->x, bot->y, 0xffe0, pBuffer);
	renderLine(top->x, top->y, mid->x, mid->y, 0x0cff, pBuffer);
	renderLine(mid->x, mid->y, bot->x, bot->y, 0xf81f, pBuffer); 

	for(i = top->y; i <= bot->y; i++)
	{
		renderSpan(bounds[0][i], bounds[1][i], i, pTri->col, pBuffer);
	}
}

void calcSpanBounds(long *boundBuffer, long x1, long y1, long x2, long y2)
{
	long x = x1;
	long y = y1;
	long dx = (x1 < x2 ? x2 - x1 : x1 - x2);
	long sx = (x1 < x2 ? 1 : -1);

	long dy = - (y1 < y2 ? y2 - y1 : y1 - y2);
	long sy = (y1 < y2 ? 1 : -1);
	long ymod = 0;
	long err = dx + dy;
	long err2;

	while(1)
	{
		if(x == x2 && y == y2)
		{
			boundBuffer[y + ymod] = x;
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
			boundBuffer[y + ymod] = x;
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