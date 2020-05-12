#include <stdio.h>
#include <TOS.H>
#include <ext.h>

#include "framewrk.h"

#define DEBUG	0
#define V_MODE 276
#define u32 unsigned int

int VgetMode(void)
{
	return xbios(88, -1);
}

int main(int argc, char ** argv)
{
	int prevMode;
	long tick = 0;

	size_t screenSize;
	void * prevLogBase;
	void * prevPhyBase;
	void * buffers[2];
	void * current;
	
	prevLogBase = Logbase();
	prevPhyBase = Physbase();
	
	screenSize = VgetSize(V_MODE);
	buffers[0] = malloc(screenSize);
	buffers[1] = malloc(screenSize);
	current = buffers[0];
	
	prevMode = VgetMode();
	xbios(5, buffers[0], buffers[1], 3, (int)V_MODE);
	
	while(!kbhit())
	{
/*		xbios(5, buffers[0], buffers[1], 3, -1);*/
		Vsync();
		/*
		tick++;
		current = buffers[tick & 1];*/
	}
	
	xbios(5, prevLogBase, prevPhyBase, 3, prevMode);
	
	free(buffers[0]);
	free(buffers[1]);

	return 0;
}