#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

#include "OBJ.H"
#include "FRAMEWRK.H"
#include "FX.H"
#include "VECTOR.H"

void loadTest()
{
	FILE* f;
	double v;
	char buffer[255];
	char floats[4][255];

	f = fopen("DATA/TEST.TXT", "r");

	if(f)
	{
		printf("Opened file\n");
		while(fgets(buffer, sizeof buffer, f))
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
	char input[128];
	char line[128];
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
		return o;
	}

	/* grab the vertex and face counts */
	while(fgets(input, sizeof input, f))
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

	while(fgets(line, sizeof line, f))
	{
		sscanf(line, "%s", input);

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

	o.faceNormals = malloc(sizeof(V3) * o.faceCount);
	o.faceNormalsX = malloc(sizeof(V3) * o.faceCount);

	for(currentNormal = 0, currentIndex = 0; currentNormal < o.faceCount; currentNormal++, currentIndex += 3)
	{
		v1 = o.verts[currentIndex];
		v2 = o.verts[currentIndex + 1];
		v3 = o.verts[currentIndex + 2];

		v2 = subVec3(v2, v1);
		normalize(&v2);

		v3 = subVec3(v3, v1);
		normalize(&v3);

		o.faceNormals[currentNormal] = cross(v2, v3);
	}

#ifndef RUN_ENGINE
	printf("Verts: %ld, Indices: %ld\n", o.vertCount, o.indexCount);

	for(currentVert = 0; currentVert < o.vertCount; currentVert++)
	{
		printf("%ld,%ld,%ld\n", o.verts[currentVert].x, o.verts[currentVert].y, o.verts[currentVert].z);
	}

	printf("\nFaces:\n\n");

	for(currentVert = 0; currentVert < o.indexCount; currentVert += 3)
	{
		printf("%ld - %ld - %ld\n", o.indices[currentVert], o.indices[currentVert + 1], o.indices[currentVert + 2]);
	}
#endif

	return o;
}

void renderObject(Obj o, Mat3d cam, void* pBuffer)
{
	int i = 0, j = 0;
	unsigned int col = 0;
	Tri tx;
	V3 v1, v2, v3, vCam;

	/* Extract this from the camera matrix z component */
	vCam = Vec3(0, 0, FX_ONE);

	for(i = 0; i < o.vertCount; i++)
	{
		v1 = V3xMat3d(o.verts[i], o.mat);
		v1 = addVec3(v1, o.pos);
		o.vertsX[i] = V3xMat3dHom(v1, cam);
	}

	for(i = 0; i < o.faceCount; i++)
	{
		o.faceNormalsX[i] = V3xMat3d(o.faceNormals[i], o.mat);
	}

	for(i = 0, j = 0; i < o.indexCount; i+= 3, j++)
	{
		col += 4096;

		v1 = o.vertsX[o.indices[i + 0]];
		v2 = o.vertsX[o.indices[i + 1]];
		v3 = o.vertsX[o.indices[i + 2]];

		/*if(dot(o.faceNormalsX[j], vCam) < 0)*/
		{
			tx = makeTri(v1, v2, v3, col);
			triToScreen(&tx);
			renderTri(tx, pBuffer);
		}
	}
}
