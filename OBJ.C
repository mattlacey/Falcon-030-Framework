#include <stdlib.h>

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

void renderObject(Obj o, Mat3d cam, void* pBuffer)
{
    int i = 0;
    Tri tx;
    V3 v1, v2, v3, sv1, sv2, sv3;

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