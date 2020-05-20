#ifndef _TRI_H_
#define _TRI_H_

#include "Vector.h"

typedef struct Tri
{
	V3 verts[3];
} Tri;

void renderTri(Tri *pTri, void *pBuffer, unsigned int col);
void renderSpan(long x1, long x2, long y, unsigned col, void *pBuffer);
void renderLine(long x1, long y1, long x2, long y2, void *pBuffer, unsigned int col);

#endif
