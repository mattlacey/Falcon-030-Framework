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
	V3 *left;
	V3 *right;
	V3 *temp;
	V3 *target;

	long x[2], y[2], dx[2], dy[2], sx[2], sy[2], err[2], err2[2];
	long safe = 1000;
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

/*	
	renderLine(top->x, top->y, left->x, left->y, 0x8f00, pBuffer);
	renderLine(top->x, top->y, right->x, right->y, 0x00ff, pBuffer);
*/

	dx[0] = (top->x < left->x ? left->x - top->x : top->x - left->x);
	dx[1] = (top->x < right->x ? right->x - top->x : top->x - right->x);

	sx[0] = (top->x < left->x ? 1 : -1);
	sx[1] = (top->x < right->x ? 1 : -1);

/*	long dy = - (y1 < y2 ? y2 - y1 : y1 - y2);
	long sy = (y1 < y2 ? 1 : -1);
*/	
	dy[0] = - (left->y - top->y);
	dy[1] = - (right->y - top->y);

	sy[0] = 1;
	sy[1] = 1;
	
	err[0] = dx[0] + dy[0];
	err[1] = dx[1] + dy[1];
	
	if(top->y == mid->y)
	{
		x[(left == mid ? 0 : 1)] = mid->x;
		y[(left == mid ? 0 : 1)] = mid->y;
	}
	
	target = mid;

	while(safe)
	{
		renderSpan(x[0], x[1], y[0], pTri->col, pBuffer);
		/*
		*((unsigned int*)pBuffer + x[0] + (y[0] * (long)320)) = col;
		*((unsigned int*)pBuffer + x[1] + (y[1] * (long)320)) = col;
		*/

		if((x[0] == target->x && y[0] == target->y)
			|| (x[1] == target->x && y[1] == target->y))
		{
			if(target == bot)
			{
				break;
			}
			else
			{
				/* First half done - recalculate left/right accordingly */
				target = bot;
				
				i = (x[0] == mid->x && y[0] == mid->y) ? 0 : 1;
				
				dx[i] = (mid->x < bot->x ? bot->x - mid->x : mid->x - bot->x);
				dy[i] = - (bot->y - mid->y);
				sx[i] = (mid->x < bot->x ? 1 : -1);
				dy[i] = - (bot->y - mid->y);
				err[i] = dx[i] + dy[i];
			}
		}

		err2[0] = 2 * err[0];
		err2[1] = 2 * err[1];
		
		if(err2[0] >= dy[0])
		{
			err[0] += dy[0];
			x[0] += sx[0];
		}
		
		if(err2[1] >= dy[1])
		{
			err[1] += dy[1];
			x[1] += sx[1];
		}
		
		if(err2[0] <= dx[0])
		{
			err[0] += dx[0];
			y[0] += sy[0];
		}
		
		if(err2[1] <= dx[1])
		{
			err[1] += dx[1];
			y[1] += sy[1];
		}
		
		safe--;
	}
		
	renderLine(top->x, top->y, bot->x, bot->y, 0xffe0, pBuffer);
	renderLine(top->x, top->y, mid->x, mid->y, 0x0cff, pBuffer);
	renderLine(mid->x, mid->y, bot->x, bot->y, 0xf81f, pBuffer); 
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