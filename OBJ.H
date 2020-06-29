#ifndef _OBJ_H
#define _OBJ_H

/* #define DEBUG_LOADER 1 */

#include "TRI.H"
#include "VECTOR.H"
#include "MATRIX.h"

typedef struct
{
	V3 * verts;
	V3 * vertsX;
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
void renderObjectDebug(Obj *pObj, Mat3d cam);
void renderObject(Obj *pObj, Mat3d cam, void* pBuffer);

#endif
