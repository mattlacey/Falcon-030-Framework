#ifndef _TRI_H_
#define _TRI_H_

#include "Vector.h"

typedef struct Tri
{
	V3 verts[3];
	unsigned col;
} Tri;

Tri makeTri(V3 v1, V3 v2, V3 v3, unsigned col);
void triToScreen(Tri *pTri);
void renderTri(Tri *pTri, void *pBuffer);
void renderSpan(long x1, long x2, long y, unsigned col, void *pBuffer);
void renderLine(long x1, long y1, long x2, long y2, unsigned col, void *pBuffer);
void calcSpanBounds(long *boundBuffer, long x1, long y1, long x2, long y2);

#endif
