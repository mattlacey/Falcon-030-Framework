#ifndef _MATRIX_H_
#define _MATRIX_H_

#include "FX.H"
#include "VECTOR.H"

typedef fx32 Mat3d[4][4];

void setIdentity(Mat3d m);
void setProjection(Mat3d m);

void setRotX(Mat3d out, int angle);
void setRotY(Mat3d out, int angle);
void setRotZ(Mat3d out, int angle);


void multiplyMat3d(Mat3d out, Mat3d m1, Mat3d m2);
V3 V3xMat3d(V3 v, Mat3d m);
V3 V3xMat3dHom(V3 v, Mat3d m);

#endif
