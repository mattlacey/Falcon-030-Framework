#ifndef _OBJ_H
#define _OBJ_H

#define FACE_NORMALS 1
/* #define DEBUG_LOADER 1 */

#include "TRI.H"
#include "VECTOR.H"
#include "MATRIX.h"

typedef struct
{
	V3 * verts;
	V3 * vertsX;
#ifdef FACE_NORMALS
	V3 * faceNormals;
	V3 * faceNormalsX;
#endif
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
