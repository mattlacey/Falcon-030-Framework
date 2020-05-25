#include "MATRIX.H"

void setIdentity(Mat3d m)
{
    m[0][0] = FX_ONE; m[1][0] = 0;      m[2][0] = 0;      m[3][0] = 0;
    m[0][1] = 0;      m[1][1] = FX_ONE; m[2][1] = 0;      m[3][1] = 0;
    m[0][2] = 0;      m[1][2] = 0;      m[2][2] = FX_ONE; m[3][2] = 0;
    m[0][3] = 0;      m[1][3] = 0;      m[2][3] = 0;      m[3][3] = FX_ONE;
}
