#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <tos.h>
#include <ext.h>

#include "fx.h"
#include "framewrk.h"
#include "tri.h"
#include "matrix.h"
#include "obj.h"

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

void renderBG(void *pBuffer)
{
	unsigned int col = 0;
	long i;
	long res = 320 * 240;

	for(i = 0; i < res; i++)
	{
		*((unsigned int *)pBuffer + i) = col;
	}
}

int main()
{
	int prevMode;
	long maxIndices;
	unsigned long tick = 0;
	long i = 0;
	long screenSize;

	fx32 fxtemp;

	V3 v, v1;
	Mat3d cam;
	Mat3d rotZ;
	Mat3d rotY;

	Obj o;

	void * prevLogBase;
	void * prevPhyBase;
	void * buffers[2];
	void * current;

	initTables();

	prevLogBase = Logbase();
	prevPhyBase = Physbase();

	setProjection(cam);

#ifdef CUBE
	o = loadObj("DATA/SQUARE.OBJ");
	o.pos = Vec3(0, 0, FX_ONE * 5);
#else
	o = loadObj("DATA/TEAPOT.OBJ");
	o.pos = Vec3(0, 0, FX_ONE * 100);
#endif

	maxIndices = o.indexCount;
	/*o.indexCount = 0;*/

#ifdef RUN_ENGINE

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

	memset(buffers[1], 0x00, screenSize);
	current = buffers[1];

	xbios(5, buffers[1], buffers[1], -1);

	while(1)
	{
		current = buffers[1];
		buffers[1] = buffers[0];
		buffers[0] = current;

		xbios(5, buffers[0], buffers[1], -1);
		Vsync();

	/*	renderBG(current); */
		memset(current, 0x00, screenSize);
		renderObject(o, cam, current);

		i += 2;

		if(i == 360)
		{
			i = 0;
		}

		setRotZ(rotZ, i);
		setRotY(rotY, i);
		multiplyMat3d(o.mat, rotZ, rotY);

		if(kbhit())
		{
			char c = getch();

			if(c == 'q')
				break;

			if(c == 'p' && o.indexCount >= 3)
				o.indexCount -= 3;
			else if(c == 'o' && o.indexCount < maxIndices - 3)
				o.indexCount += 3;

			if(c == 'w')
				o.pos.z += FX_ONE;
			else if(c == 's')
				o.pos.z -= FX_ONE;
		}

		tick++;
	}

	xbios(5, prevLogBase, prevPhyBase, 3, prevMode);

#endif

	renderObjectDebug(o, cam);

	printf("\n\nPress a key to continue...\n");
	while(!kbhit());

	free(buffers[0]);
	free(buffers[1]);

	return 0;
}