#include  "tri.h"

void renderTri(Tri *pTri, void *pBuffer, unsigned col)
{
	/* draw p1 -> p2 */
	V3 *verts = pTri->verts;

	long x1 = verts[0].x;
	long y1 = verts[0].y;
	long x2 = verts[1].x;
	long y2 = verts[1].y;
	
	long dx = x2 - x1;
	long dy = y2 - y1;
	long tdx = dx << 1;
	long tdy = dy << 1;
	long tdxmtdy = tdx - tdy;
	long tdymtdx = tdy - tdx;
	
	long x = x1;
	long y = y1;
	long p;
	
	long xinc = (x1 > x2 ? -1 : 1);
	long yinc = (y1 > y2 ? -1 : 1);

	p = tdy - dx;
	
	if(dx > dy)
	{
		while(x != x2)
		{
			*((unsigned *)pBuffer + x + (y * (long)320)) = col;
			if(p < 0)
			{
				p = p + tdy;
			}
			else
			{
				p = p + tdymtdx;
				y += yinc;
			}
	
			x += xinc;
		}
	}
	else
	{
		while(y != y2)
		{
			*((unsigned *)pBuffer + x + (y * (long)320)) = col;
			if(p < 0)
			{
				p = p + tdx;
			}
			else
			{
				p = p + tdxmtdy;
				x += xinc;
			}
	
			y += yinc;
		}
	}
	
	return;
}