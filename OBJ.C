#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

#include "OBJ.H"
#include "FRAMEWRK.H"
#include "FX.H"

Obj makeCube(void)
{
    Obj o;
    fx32 size = FX_ONE * 30;

    /* ok, it's a couple of squares for now */

    o.vertCount = 8;
    o.verts = malloc(sizeof(V3) * o.vertCount);

    o.verts[0] = Vec3(-size, size, -size);
    o.verts[1] = Vec3(size, size, -size);
    o.verts[2] = Vec3(-size, -size, -size);
    o.verts[3] = Vec3(size, -size, -size);

    o.verts[4] = Vec3(-size, size, size);
    o.verts[5] = Vec3(size, size, size);
    o.verts[6] = Vec3(-size, -size, size);
    o.verts[7] = Vec3(size, -size, size);

    o.indexCount = 12;
    o.indices = malloc(sizeof(long) * o.indexCount);

    /* front */
    o.indices[0] = 0; o.indices[1] = 1; o.indices[2] = 2;
    o.indices[3] = 2; o.indices[4] = 1; o.indices[5] = 3;

    /* back */
    o.indices[6] = 5; o.indices[7] = 4; o.indices[8] = 7;
    o.indices[9] = 7; o.indices[10] = 4; o.indices[11] = 6;

    o.col = RED;
    o.pos = Vec3(0, 0, - FX_ONE * 150);
    setIdentity(o.mat);

    return o;
}

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
    int scanCount = 0;
    Obj o;
    char input[128];
    char line[128];
    char floats[4][32];
    long indices[3];

    setIdentity(o.mat);
    o.pos = Vec3(0, 0, 0);
    o.vertCount = 0;
    o.indexCount = 0;

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
        }
    }

    fclose(f);

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
    int i = 0;
    Tri tx;
    V3 v1, v2, v3;

    /* Should transform all verts first, but for now we're going dumb */

    for(i = 0; i < o.indexCount; i+= 3)
    {
        v1 = V3xMat3d(o.verts[o.indices[i + 0]], o.mat);
        v2 = V3xMat3d(o.verts[o.indices[i + 1]], o.mat);
        v3 = V3xMat3d(o.verts[o.indices[i + 2]], o.mat);

        v1 = AddVec3(v1, o.pos);
        v2 = AddVec3(v2, o.pos);
        v3 = AddVec3(v3, o.pos);

        v1 = V3xMat3dHom(v1, cam);
        v2 = V3xMat3dHom(v2, cam);
        v3 = V3xMat3dHom(v3, cam);

        tx = makeTri(v1, v2, v3, o.col);
        triToScreen(&tx);
        renderTri(tx, pBuffer);
    }
}
