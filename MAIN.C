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

void printV3(V3 v)
{
	printf("(%ld, %ld, %ld)\n", 160 + (v.x >> 4), 120 + (v.y >> 4), v.z);
}

int main()
{
	int prevMode;
	unsigned long tick = 0;
	long i;
	long screenSize;
	fx32 f1, f2;
	
	Tri t1, t2, t3, tx;
	V3 v1, v2, v3;
	Mat3d cam;

	void * prevLogBase;
	void * prevPhyBase;
	void * buffers[2];
	void * current;

	#define FX_X (FX_ONE * (fx32)100)
	#define FX_Z (FX_ONE * (fx32)100)

	t1 = makeTri(Vec3(-FX_X, 0, FX_Z), Vec3(0, FX_X, FX_Z),  Vec3(FX_X, 0, 2 * FX_Z), 0xf800);
	t2 = makeTri(Vec3(-FX_X, 0, FX_Z), Vec3(0, FX_X, FX_Z),  Vec3(FX_X, 0, 2 * FX_Z), 0x07e0);
	t3 = makeTri(Vec3(FX_X, 0, FX_Z),  Vec3(0, 0, -1), Vec3(0, 1, 0), 0x001f);
	
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
		
		v1 = V3xMat3dHom(t1.verts[0], cam);
		v2 = V3xMat3dHom(t1.verts[1], cam);
		v3 = V3xMat3dHom(t1.verts[2], cam);
		
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

	v1 = V3xMat3dHom(t1.verts[0], cam);
	v2 = V3xMat3dHom(t1.verts[1], cam);
	v3 = V3xMat3dHom(t1.verts[2], cam);
		
	printf("TRI 1:\n");
	printV3(v1);
	printV3(v2);
	printV3(v3);
	
	v1 = V3xMat3dHom(t2.verts[0], cam);
	v2 = V3xMat3dHom(t2.verts[1], cam);
	v3 = V3xMat3dHom(t2.verts[2], cam);
	
	printf("\n\nTRI 2:\n");
	printV3(v1);
	printV3(v2);
	printV3(v3);
	
		
	printf("\n\nPress a key to continue...\n");
	while(!kbhit());
	
	free(buffers[0]);
	free(buffers[1]);

	return 0;
}