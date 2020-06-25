#ifndef _OBJ_H
#define _OBJ_H

#include "TRI.H"
#include "VECTOR.H"
#include "MATRIX.h"

typedef struct
{
	V3 * verts;
	V3 * vertsX;
	V3 * faceNormals;
	V3 * faceNormalsX;
	long vertCount;
	long * indices;
	long indexCount;
	long faceCount;

	V3 pos;
	Mat3d mat;
	unsigned col;
} Obj;

void loadTest(void);
Obj loadObj(char * filename);
void renderObjectDebug(Obj o, Mat3d cam);
void renderObject(Obj o, Mat3d cam, void* pBuffer);

#endif
