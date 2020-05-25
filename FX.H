#ifndef _FX_H_
#define _FX_H_

typedef long fx32;

#define FX_SHIFT (16)
#define FX_ONE (fx32)(1 << FX_SHIFT)
#define FX_MUL(a, b) ((a * b) >> FX_SHIFT)
#define FX_DIV(a, b) ((a / b) << FX_SHIFT)

#endif

