#ifndef _OBJ_H
#define _OBJ_H

#define FACE_NORMALS 1
#define BRANCH_NODE 0xffffffff
/* #define DEBUG_LOADER 1 */

#include "TRI.H"
#include "VECTOR.H"
#include "MATRIX.h"

typedef struct BSPPlane
{
	long orientation; /* 0 = x, 1 = y, 2 = z */
	fx32 distance;
} BSPPlane;

typedef struct ObjFace
{
	long v1;
	long v2;
	long v3;
} ObjFace;

typedef struct ObjPart
{
	ObjFace* faces;
	long faceCount;
} ObjPart;

typedef struct ObjNode
{
	ObjPart* pPart;
	BSPPlane hyperplane;
	struct ObjNode* pLeft;
	struct ObjNode* pRight;
} ObjNode;

typedef struct Obj
{
	V3* verts;
	V3* vertsX;
	long vertCount;

	union
	{
		long* indices;
		ObjFace* faces;
	};

	long indexCount;
	long faceCount;

#ifdef FACE_NORMALS
	V3* faceNormals;
	V3* faceNormalsX;
#endif

	ObjNode* pRootNode;

	V3 pos;
	Mat3d mat;
	unsigned col;
} Obj;

void loadTest(void);
Obj loadObj(char * filename);
Obj loadTree(char* filename);
void freeObj(Obj* pObj);
void renderObjectDebug(Obj *pObj, Mat3d cam);
void renderObject(Obj *pObj, Mat3d projection, void* pBuffer);

#endif
