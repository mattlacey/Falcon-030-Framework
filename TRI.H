#ifndef _TRI_H_
#define _TRI_H_

#include "Vector.h"

typedef struct Tri
{
	V3 verts[3];
} Tri;

void renderTri(Tri *pTri, void *pBuffer, unsigned col);

#endif
