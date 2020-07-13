#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

#include "OBJ.H"
#include "FRAMEWRK.H"
#include "FX.H"
#include "VECTOR.H"

#define INPUT_BUFFER_SIZE (128)

void loadTest()
{
	FILE* f;
	char buffer[INPUT_BUFFER_SIZE];
	char floats[4][255];

	f = fopen("DATA/TEST.TXT", "r");

	if(f)
	{
		printf("Opened file\n");
		while(fgets(buffer, INPUT_BUFFER_SIZE, f))
		{
			sscanf(buffer, "%s", floats[0]);
			printf("Found: %ld in %s\n", (fx32)(FX_ONE * atof(floats[0])), buffer);
		}

		fclose(f);
	}
}

Obj loadObj(char * filename)
{
	FILE* f;
	int currentVert = 0;
	int currentIndex = 0;
	int currentNormal = 0;
	int scanCount = 0;
	Obj o;
	char input[INPUT_BUFFER_SIZE];
	char line[INPUT_BUFFER_SIZE];
	char floats[4][32];
	long indices[3];

	V3 v1, v2, v3;

	setIdentity(o.mat);
	o.pos = Vec3(0, 0, 0);
	o.vertCount = 0;
	o.indexCount = 0;
	o.faceCount = 0;

	f = fopen(filename, "r");

	if(f == NULL)
	{
		printf("Failed to open file");
		return o;
	}

	/* grab the vertex and face counts */
	while(fgets(input, INPUT_BUFFER_SIZE, f))
	{
		sscanf(input, "%s", input);

		if(strcmp(input, "v") == 0)
		{
			o.vertCount++;
		}
		else if(strcmp(input, "f") == 0)
		{
			/* each face uses 3 indices... need to switch to storing triangles */
			o.indexCount += 3;
		}
	}

	fseek(f, 0, SEEK_SET);

	o.verts = malloc(sizeof(V3) * o.vertCount);
	o.vertsX = malloc(sizeof(V3) * o.vertCount);
	o.indices = malloc(sizeof(long) * o.indexCount);

	while(fgets(line, INPUT_BUFFER_SIZE, f))
	{
		if (sscanf(line, "%s", input) == EOF)
		{
			continue;
		}

		if(strcmp(input, "v") == 0)
		{
			sscanf(line, "%*s %s %s %s", floats[0], floats[1], floats[2]);
			o.verts[currentVert].x = (fx32)(atof(floats[0]) * FX_ONE);
			o.verts[currentVert].y = (fx32)(atof(floats[1]) * FX_ONE);
			o.verts[currentVert].z = (fx32)(atof(floats[2]) * FX_ONE);
			currentVert++;
		}
		else if(strcmp(input, "f") == 0)
		{
			/* try vert/tex/normal format first */
			scanCount = sscanf(line, "%*s %ld/%*s %ld/%*s %ld/%*s", &indices[0], &indices[1], &indices[2]);

			if(scanCount == 1)
			{
				scanCount = sscanf(line, "%*s %ld %ld %ld", &indices[0], &indices[1], &indices[2]);
			}

			/* for some reason indices are not zero based */
			o.indices[currentIndex] = indices[0] - 1;
			o.indices[currentIndex + 1] = indices[1] - 1;
			o.indices[currentIndex + 2] = indices[2] - 1;
			currentIndex += 3;
			o.faceCount++;
		}
	}

	fclose(f);

#ifdef FACE_NORMALS
	o.faceNormals = malloc(sizeof(V3) * o.faceCount);
	o.faceNormalsX = malloc(sizeof(V3) * o.faceCount);

	for(currentNormal = 0, currentIndex = 0; currentNormal < o.faceCount; currentNormal++, currentIndex += 3)
	{
		v1 = o.verts[o.indices[currentIndex]];
		v2 = o.verts[o.indices[currentIndex + 1]];
		v3 = o.verts[o.indices[currentIndex + 2]];

		v1 = subVec3(v1, v2);
		normalize(&v1);

		v3 = subVec3(v3, v2);
		normalize(&v3);

		v2 = cross(v3, v1);
		normalize(&v2);
		o.faceNormals[currentNormal] = v2;

#ifdef DEBUG_LOADER
		printf("Face sides: ");
		printV3(v1);
		printV3(v3);

		printf("\nCP: ");
		printV3(o.faceNormals[currentNormal]);
		printf("\n");
#endif
	}
#endif

#ifdef DEBUG_LOADER
	printf("Verts:\n\n");

	for(currentVert = 0; currentVert < o.vertCount; currentVert++)
	{
		printf("%ld,%ld,%ld\n", o.verts[currentVert].x, o.verts[currentVert].y, o.verts[currentVert].z);
	}

	printf("\nFaces:\n\n");

	for(currentVert = 0; currentVert < o.indexCount; currentVert += 3)
	{
		printf("%ld - %ld - %ld\n", o.indices[currentVert], o.indices[currentVert + 1], o.indices[currentVert + 2]);
	}

	printf("Verts: %ld, Indices: %ld\n", o.vertCount, o.indexCount);

#endif

	return o;
}

void freeObj(Obj* pObj)
{
	/* todo: check for leaks */
	free(pObj->verts);
	free(pObj->vertsX);
	free(pObj->indices);

#ifdef FACE_NORMALS
	free(pObj->faceNormals);
	free(pObj->faceNormalsX);
#endif
}


void renderObjectDebug(Obj *pObj, Mat3d cam)
{
	int i = 0, j = 0;
	unsigned int col = 0;
	Tri tx;
	V3 v1, v2, v3, vCam, ve1, ve2, vn;

	/* Extract this from the camera matrix z component */
	vCam = Vec3(0, 0, FX_ONE);

	printf("Translating verts...\n");

	for(i = 0; i < pObj->vertCount; i++)
	{
		v1 = V3xMat3d(pObj->verts[i], pObj->mat);
		v1 = addVec3(v1, pObj->pos);
		pObj->vertsX[i] = V3xMat3dHom(v1, cam);
	}

	printf("Drawing faces...\n");

	for(i = 0, j = 0; i < pObj->indexCount; i+= 3, j++)
	{
		col += 4096;

		v1 = pObj->vertsX[pObj->indices[i + 0]];
		v2 = pObj->vertsX[pObj->indices[i + 1]];
		v3 = pObj->vertsX[pObj->indices[i + 2]];

		printf("Face %ld\n", i);

		ve1 = subVec3(v1, v2);
		ve2 = subVec3(v3, v2);

		ve1.x <<= 8;
		ve1.y <<= 8;
		ve1.z <<= 8;

		ve2.x <<= 8;
		ve2.y <<= 8;
		ve2.z <<= 8;

		printV3(ve1);
		printV3(ve2);

		vn = cross(ve2, ve1);

		printf("CP: ");
		printV3(vn);

		/* Not needed yet, but will be needed for lighting etc */
		normalize(&vn);

		printf("Normalized: ");
		printV3(vn);
	
		if(dot(vn, vCam) <= 0)
		{
			tx = makeTri(v1, v2, v3, col);
			triToScreen(&tx);
			printTri(&tx);
		}
	}
}

void renderObject(Obj *pObj, Mat3d projection, void* pBuffer)
{
	int i = 0, j = 0;
	unsigned int col = 0;
	Tri tx;
	V3 v1, v2, v3, vCam, vLight, ve1, ve2, vn;

	/* Extract this from the camera matrix z component */
	vCam = Vec3(0, 0, FX_ONE);
	vLight = Vec3(FX_ONE, FX_ONE, - 2 * FX_ONE);
	normalize(&vLight);

	for(i = 0; i < pObj->vertCount; i++)
	{
		v1 = V3xMat3d(pObj->verts[i], pObj->mat);
		v1 = addVec3(v1, pObj->pos);
		pObj->vertsX[i] = V3xMat3dHom(v1, projection);
	}

#ifdef FACE_NORMALS
	for(i = 0; i < pObj->faceCount; i++)
	{
		pObj->faceNormalsX[i] = V3xMat3d(pObj->faceNormals[i], pObj->mat);
	}
#endif

	for(i = 0, j = 0; i < pObj->indexCount; i+= 3, j++)
	{
		v1 = pObj->vertsX[pObj->indices[i + 0]];
		v2 = pObj->vertsX[pObj->indices[i + 1]];
		v3 = pObj->vertsX[pObj->indices[i + 2]];

		ve1 = subVec3(v1, v2);
		ve2 = subVec3(v3, v2);

		vn = cross(ve2, ve1);

		normalize(&vn);
	
		if(dot(vn, vCam) <= 0)
		{
			/* light needs to be in camera space too */
#ifdef FACE_NORMALS
			fx32 light = dot(pObj->faceNormalsX[j], vLight);

			/* 5 because we have 5 bits per channel */

			if(light > 0)
			{
				col = (unsigned int)((light >> (FX_SHIFT - 5)) & BLU);
			}
			else
			{
				col = 0;
			}
#else
			col += 0xff;
#endif

			col |= 0x01;

			tx = makeTri(v1, v2, v3, col);
			triToScreen(&tx);
			renderTri(tx, pBuffer);
		}
	}
}
