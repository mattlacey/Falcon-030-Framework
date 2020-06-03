#ifndef _FX_H_
#define _FX_H_

typedef long fx32;

#define FX_SHIFT (16)
#define FX_HSHIFT (8)
#define FX_ONE ((fx32)1 << FX_SHIFT)
/*#define FX_MUL(a, b) ((a >> 6) * (b >> FX_HSHIFT))
#define FX_DIV(a, b) (((a << FX_HSHIFT) / (b << FX_HSHIFT)))*/

#define FX_MUL(a, b)	((((a) >> 6) * ((b) >> 6)) >> 4)
#define FX_DIV(a, b)	((((a) << 6 ) / ((b) >> 6)) >> 4)

#define FX_RECIPROCAL(a) (((fx32)1 << 30) / (a << 2))

#endif

