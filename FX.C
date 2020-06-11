#include "FX.H"
#include <math.h>

fx32 sinTable[360];
fx32 cosTable[360];


void initTables()
{
    int i = 0;

    for(i = 0; i < 360; i++)
    {
        double angle = ((double)i / 180.0) * M_PI;
        sinTable[i] = FX_ONE * sin(angle);
        cosTable[i] = FX_ONE * cos(angle);
    }
}
