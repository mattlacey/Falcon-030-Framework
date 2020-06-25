#ifndef _FX_H_
#define _FX_H_

typedef long fx32;

#define FX_SHIFT (16)
#define FX_SIZE (32)
#define FX_HSHIFT (8)
#define FX_ONE ((fx32)1 << FX_SHIFT)
#define FX_HALF ((fx32)1 << (FX_SHIFT - 1))
/*#define FX_MUL(a, b) ((a >> 6) * (b >> FX_HSHIFT))*/
/* #define FX_DIV(a, b) ((((a) << FX_HSHIFT) / ((b) << FX_HSHIFT)) << FX_SHIFT) */

#define FX_MUL(a, b)	((((a) >> 6) * ((b) >> 6)) >> 4)

#define FX_DIV(a, b)	((((a) << 6 ) / ((b) >> 6)) << 4)

#define FX_RECIPROCAL(a) (((fx32)1 << 30) / (a << 2))

extern fx32 sinTable[360];
extern fx32 cosTable[360];

void initTables(void);
fx32 fx32abs(fx32 value);
fx32 safeDiv(fx32 a, fx32 b);

#endif

