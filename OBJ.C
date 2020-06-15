#include <stdlib.h>

#include "OBJ.H"
#include "FRAMEWRK.H"

Obj makeCube(void)
{
    Obj o;

    o.vertCount = 6;
    o.verts = malloc(sizeof(V3) * o.vertCount);

    o.indexCount = 12 * 3;
    o.indices = malloc(sizeof(long) * o.indexCount);

    o.col = RED;
    o.pos = Vec3(0, 0, 0);
    setIdentity(o.mat);

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
        v1 = o.verts[o.indices[i]];
        v2 = o.verts[o.indices[i + 1]];
        v3 = o.verts[o.indices[i + 2]];

        tx = makeTri(v1, v2, v3, o.col);
        triToScreen(tx);
        renderTri(tx, pBuffer);
    }
}
