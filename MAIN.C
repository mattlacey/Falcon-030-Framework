#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <tos.h>
#include <ext.h>

#include "framewrk.h"
#include "tri.h"

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

int main(int argc, char ** argv)
{
	int prevMode;
	unsigned long tick = 0;
	long i;
	long screenSize;
	
	Tri t1, t2, t3;

	void * prevLogBase;
	void * prevPhyBase;
	void * buffers[2];
	void * current;

	t1 = makeTri(Vec3(20, 20, 0), Vec3(80, 20, 0), Vec3(30, 80, 0), 0xf800);
	t2 = makeTri(Vec3(120, 20, 0), Vec3(200, 100, 0), Vec3(140, 150, 0), 0x07e0);
	/* Change this second vector back to (200, 230, 0) */
	t3 = makeTri(Vec3(80, 160, 0), Vec3(40, 230, 0), Vec3(300, 230, 0), 0x001f);
	
	prevLogBase = Logbase();
	prevPhyBase = Physbase();
	
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

		/* clear old stuff from two frames ago 
		t.verts[0].x = 20 + ((tick - 2) & 0x00ff);
		t.verts[1].x = 320 - ((tick - 2) & 0x00ff);
		renderTri(&t, buffers[0], 0x0000);*/

		/* render new stuff 
		t.verts[0].x = 20 + (tick & 0x00ff);
		t.verts[1].x = 320 - (tick & 0x00ff);
		renderTri(&t, buffers[0], 0xf800);*/
		
		renderTri(&t1, buffers[0]);
		renderTri(&t2, buffers[0]);
		renderTri(&t3, buffers[0]);
		

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

	free(buffers[0]);
	free(buffers[1]);

	return 0;
}