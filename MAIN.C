#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <tos.h>
#include <ext.h>

#include "framewrk.h"

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
	long tick = 0;
	long i;
	long screenSize;

	void * prevLogBase;
	void * prevPhyBase;
	void * buffers[2];
	void * current;
	
	void * t1;
	void * t2;
	
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

	memset(buffers[0], 0xf0, screenSize);
	memset(buffers[1], 0x07, screenSize);
	
	prevMode = VgetMode();
	xbios(5, buffers[0], buffers[1], 3, (int)V_MODE);

	while(1)
	{
		if(tick & 1)
			xbios(5, buffers[0], buffers[1], -1, -1);
		else
			xbios(5, buffers[1], buffers[0], -1, -1);

		Vsync();

		if(kbhit())
		{
			char c = getch();
			
			if(c == 'q')
				break;
		}
		
		tick = tick + 1;
	}

	xbios(5, prevLogBase, prevPhyBase, 3, prevMode);
	
	printf("Press a key to continue...\n");

	while(!kbhit());

	free(buffers[0]);
	free(buffers[1]);

	return 0;
}