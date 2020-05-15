#include  "tri.h"

void renderTri(Tri *pTri, void *pBuffer)
{
	/* draw p1 -> p2 */
	long y;
	V3 *verts = pTri->verts;
	long x = verts[0].x;
	long xInc = (verts[1].x - verts[0].x) / (verts[1].y - verts[0].y);
	
	for(y = verts[0].y; y < verts[1].y; y++)
	{
		*((unsigned *)pBuffer + (y * (long)320) + x) = 0xffff;
		x += xInc;
	}
	
	return;
}