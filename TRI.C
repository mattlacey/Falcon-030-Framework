#include  "tri.h"

void renderTri(Tri *pTri, void *pBuffer, unsigned col)
{
	/* draw p1 -> p2 */
	V3 *verts = pTri->verts;

	renderLine(verts[0].x, verts[0].y, verts[1].x, verts[1].y, pBuffer, col);
	renderLine(verts[0].x, verts[0].y, verts[2].x, verts[2].y, pBuffer, col);
	renderLine(verts[1].x, verts[1].y, verts[2].x, verts[2].y, pBuffer, col);
}

void renderLine(long x1, long y1, long x2, long y2, void *pBuffer, unsigned col)	
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
		*((unsigned *)pBuffer + x + (y * (long)320)) = col;
		
		if(x == x2 || y == y2) break;
		
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