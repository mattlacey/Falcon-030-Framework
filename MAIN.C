#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <tos.h>
#include <ext.h>

#include "framewrk.h"
#include "tri.h"
#include "matrix.h"

#define DEBUG	0

#define VM_BPS16	0x4
#define VM_COL_80	0x8
#define VM_VGA		0x10
#define VM_PAL		0x20
#define VM_OVRSCN	0x40
#define VM_STMODE	0x80
#define	VM_DBLLIN	0x100

#define V_MODE (VM_BPS16|VM_VGA|VM_PAL|VM_DBLLIN)

int VgetMode(void)
{
	return xbios(88, (int)-1);
}

int main()
{
	int prevMode;
	unsigned long tick = 0;
	long i;
	long screenSize;
	
	Tri t1, t2, t3, tx;
	Mat3d cam;

	void * prevLogBase;
	void * prevPhyBase;
	void * buffers[2];
	void * current;

	t1 = makeTri(Vec3(-FX_ONE, 0, FX_ONE), Vec3(FX_ONE, 0, FX_ONE),  Vec3(0, FX_ONE, 0), 0xf800);
	t2 = makeTri(Vec3(-FX_ONE, 0, FX_ONE), Vec3(0, 0, -1), Vec3(0, 1, 0), 0x07e0);
	t3 = makeTri(Vec3(FX_ONE, 0, FX_ONE),  Vec3(0, 0, -1), Vec3(0, 1, 0), 0x001f);
	
	prevLogBase = Logbase();
	prevPhyBase = Physbase();
	
	setProjection(cam);
	
	screenSize = VgetSize((int)V_MODE);
	buffers[0] = malloc(screenSize);
	buffers[1] = malloc(screenSize);
	current = buffers[0];

	if(!buffers[0] || !buffers[1])
	{
		return 1;
	}

	memset(buffers[0], 0x00, screenSize);
	
	prevMode = VgetMode();
	xbios(5, buffers[1], buffers[0], 3, (int)V_MODE);

	/* this buffer gets trashed by VSetscreen so clear here */
	memset(buffers[1], 0x00, screenSize);
	xbios(5, buffers[1], buffers[1], -1);

	while(1)
	{
		current = buffers[1];
		buffers[1] = buffers[0];
		buffers[0] = current;
	
		xbios(5, buffers[0], buffers[1], -1);
		Vsync();

		multiplyV3ByMat3d(tx.verts[0], t1.verts[0], cam);
		multiplyV3ByMat3d(tx.verts[1], t1.verts[1], cam);
		multiplyV3ByMat3d(tx.verts[2], t1.verts[2], cam);

	break;
		if(kbhit())
		{
			char c = getch();
			
			if(c == 'q')
				break;
		}
		
		tick++;
	}

	xbios(5, prevLogBase, prevPhyBase, 3, prevMode);
	
	printf("Press a key to continue...\n");

	while(!kbhit());

	tx.verts[0].x = FX_ONE;
	printf("%i, %il, %il\n", tx.verts[0].x, tx.verts[0].y, tx.verts[0].z);

	free(buffers[0]);
	free(buffers[1]);

	return 0;
}