#ifndef _framework_h_
#define _framework_h_

#define RED ((unsigned)0xf800)
#define GRN	((unsigned)0xf7f0)
#define BLU	((unsigned)0x001f)

#define YEL (RED | GRN)
#define MAG (RED | BLU)
#define CYN	(GRN | BLU)

/* #define POINTS (1) */
/* #define WIREFRAME (1) */
#define FILL (1)

/* #define RUN_ENGINE (1) */

#endif
