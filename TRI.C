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
	long tdy = dy << 1;
	long tdymtdx = tdy - (dx << 1);
	
	long x = x1;
	long y = y1;
	long tx = x2;
	long p;
	
	p = tdy - dx;
	
	while(x < tx)
	{
		*((unsigned *)pBuffer + x + (y * (long)320)) = col;
		if(p < 0)
		{
			p = p + tdy;
		}
		else
		{
			p = p + tdymtdx;
			y++;
		}
		
		x++;
	}
	
	return;
}