#ifndef _OBJ_H
#define _OBJ_H

#include "TRI.H"
#include "VECTOR.H"
#include "MATRIX.h"

typedef struct
{
	V3 * verts;
	long vertCount;
	long * indices;
	long indexCount;

	V3 pos;
	Mat3d mat;
	unsigned col;
} Obj;

Obj makeCube(void);
void loadTest(void);
Obj loadObj(char * filename);
void renderObject(Obj o, Mat3d cam, void* pBuffer);

#endif
