#include "MATRIX.H"

void setIdentity(Mat3d m)
{
    m[0][0] = FX_ONE; m[1][0] = 0;      m[2][0] = 0;      m[3][0] = 0;
    m[0][1] = 0;      m[1][1] = FX_ONE; m[2][1] = 0;      m[3][1] = 0;
    m[0][2] = 0;      m[1][2] = 0;      m[2][2] = FX_ONE; m[3][2] = 0;
    m[0][3] = 0;      m[1][3] = 0;      m[2][3] = 0;      m[3][3] = FX_ONE;
}

/* https://jsantell.com/3d-projection/ */
void setProjection(Mat3d m)
{
    #define NEAR FX_ONE
    #define FAR (FX_ONE * 1024)
    #define ASPECT_RATIO = 1.3333333;

    /* tan(45 / 2) << 16 */
    /*#define E (27145)*/
    #define E FX_ONE
    /* top = near * tan(fov / 2) : tan(45) = 1  */
    #define TOP FX_ONE
    #define BOTTOM (-TOP)
    #define RIGHT (FX_ONE * 1.3333333)
    #define LEFT (-RIGHT)
    

    /* (2 * FAR) should be 2 * NEAR * FAR but since NEAR is 1 for now, it's easier to skip it */
    m[0][0] = (E / 1.3333333); m[1][0] = 0; m[2][0] = 0; m[3][0] = 0;
    m[0][1] = 0; m[1][1] = FX_ONE; m[2][1] = 0; m[3][1] = 0;
    m[0][2] = 0; m[1][2] = 0; m[2][2] = ((NEAR + FAR) / (NEAR - FAR)); m[3][2] = ((2 * FAR) / (NEAR - FAR));
    m[0][3] = 0; m[1][3] = 0; m[2][3] = - FX_ONE; m[3][3] = 0;

}

void multiplyMat3d(Mat3d out, Mat3d m1, Mat3d m2)
{
    out[0][0] = FX_MUL(m1[0][0], m2[0][0]) + FX_MUL(m1[1][0], m2[0][1]) + FX_MUL(m1[2][0], m2[0][2]) + FX_MUL(m1[3][0], m2[0][3]);
    out[1][0] = FX_MUL(m1[0][0], m2[1][0]) + FX_MUL(m1[1][0], m2[1][1]) + FX_MUL(m1[2][0], m2[1][2]) + FX_MUL(m1[3][0], m2[1][3]);
    out[2][0] = FX_MUL(m1[0][0], m2[2][0]) + FX_MUL(m1[1][0], m2[2][1]) + FX_MUL(m1[2][0], m2[2][2]) + FX_MUL(m1[3][0], m2[2][3]);
    out[3][0] = FX_MUL(m1[0][0], m2[3][0]) + FX_MUL(m1[1][0], m2[3][1]) + FX_MUL(m1[2][0], m2[3][2]) + FX_MUL(m1[3][0], m2[3][3]);

    out[0][0] = FX_MUL(m1[0][1], m2[0][0]) + FX_MUL(m1[1][1], m2[0][1]) + FX_MUL(m1[2][1], m2[0][2]) + FX_MUL(m1[3][1], m2[0][3]);
    out[1][0] = FX_MUL(m1[0][1], m2[1][0]) + FX_MUL(m1[1][1], m2[1][1]) + FX_MUL(m1[2][1], m2[1][2]) + FX_MUL(m1[3][1], m2[1][3]);
    out[2][0] = FX_MUL(m1[0][1], m2[2][0]) + FX_MUL(m1[1][1], m2[2][1]) + FX_MUL(m1[2][1], m2[2][2]) + FX_MUL(m1[3][1], m2[2][3]);
    out[3][0] = FX_MUL(m1[0][1], m2[3][0]) + FX_MUL(m1[1][1], m2[3][1]) + FX_MUL(m1[2][1], m2[3][2]) + FX_MUL(m1[3][1], m2[3][3]);

    out[0][0] = FX_MUL(m1[0][2], m2[0][0]) + FX_MUL(m1[1][2], m2[0][1]) + FX_MUL(m1[2][2], m2[0][2]) + FX_MUL(m1[3][2], m2[0][3]);
    out[1][0] = FX_MUL(m1[0][2], m2[1][0]) + FX_MUL(m1[1][2], m2[1][1]) + FX_MUL(m1[2][2], m2[1][2]) + FX_MUL(m1[3][2], m2[1][3]);
    out[2][0] = FX_MUL(m1[0][2], m2[2][0]) + FX_MUL(m1[1][2], m2[2][1]) + FX_MUL(m1[2][2], m2[2][2]) + FX_MUL(m1[3][2], m2[2][3]);
    out[3][0] = FX_MUL(m1[0][2], m2[3][0]) + FX_MUL(m1[1][2], m2[3][1]) + FX_MUL(m1[2][2], m2[3][2]) + FX_MUL(m1[3][2], m2[3][3]);

    out[0][0] = FX_MUL(m1[0][3], m2[0][0]) + FX_MUL(m1[1][3], m2[0][1]) + FX_MUL(m1[2][3], m2[0][2]) + FX_MUL(m1[3][3], m2[0][3]);
    out[1][0] = FX_MUL(m1[0][3], m2[1][0]) + FX_MUL(m1[1][3], m2[1][1]) + FX_MUL(m1[2][3], m2[1][2]) + FX_MUL(m1[3][3], m2[1][3]);
    out[2][0] = FX_MUL(m1[0][3], m2[2][0]) + FX_MUL(m1[1][3], m2[2][1]) + FX_MUL(m1[2][3], m2[2][2]) + FX_MUL(m1[3][3], m2[2][3]);
    out[3][0] = FX_MUL(m1[0][3], m2[3][0]) + FX_MUL(m1[1][3], m2[3][1]) + FX_MUL(m1[2][3], m2[3][2]) + FX_MUL(m1[3][3], m2[3][3]);
}

/* Not compatible with translation or projection */
V3 V3xMat3d(V3 v, Mat3d m)
{
    V3 out;
    out.x = FX_MUL(v.x, m[0][0]) + FX_MUL(v.y, m[1][0]) + FX_MUL(v.z, m[2][0]) + m[3][0];
    out.y = FX_MUL(v.x, m[0][1]) + FX_MUL(v.y, m[1][1]) + FX_MUL(v.z, m[2][1]) + m[3][1];
    out.z = FX_MUL(v.x, m[0][2]) + FX_MUL(v.y, m[1][2]) + FX_MUL(v.z, m[2][2]) + m[3][2];
    return out;
}

V3 V3xMat3dHom(V3 v, Mat3d m)
{
    fx32 w;
    V3 out;

    out.x = FX_MUL(v.x, m[0][0]) + FX_MUL(v.y, m[1][0]) + FX_MUL(v.z, m[2][0]) + m[3][0];
    out.y = FX_MUL(v.x, m[0][1]) + FX_MUL(v.y, m[1][1]) + FX_MUL(v.z, m[2][1]) + m[3][1];
    out.z = FX_MUL(v.x, m[0][2]) + FX_MUL(v.y, m[1][2]) + FX_MUL(v.z, m[2][2]) + m[3][2];

    w = FX_MUL(v.x, m[0][3]) + FX_MUL(v.y, m[1][3]) + FX_MUL(v.z, m[2][3]) + m[3][3];

    if(w == 0)
    {
        /* FX_DIV shifts the second parameter right by 6 */
        w = ((fx32)1 << 7);
    }

    out.x = FX_DIV(out.x, w);
    out.y = FX_DIV(out.y, w);
    out.z = FX_DIV(out.z, w);

    return out;
}
