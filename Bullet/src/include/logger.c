#include <pspdebug.h>
#include <pspkernel.h>
#include <pspdisplay.h>
#include <stdio.h>
#include <callbacks.h>
#include <input.h>

#include "logger.h"

//
void print(const char *string, ...) 
{
	printf(string);
	printf("\n");
}

void printValue(float value)
{
	char string[15];

	sprintf(string, "%f", value);
  	print(string);
}

void throwError(const char *string, ...) 
{
    pspDebugScreenInit();
    pspDebugScreenClear();
	pspDebugScreenSetXY(0, 0);
	pspDebugScreenPrintf("%s\n",string);
	pspDebugScreenPrintf("Press X to quit.\n");
	while (running())
	{
		ctrlUpdate();
		if (crossPressed)
			break;
		sceDisplayWaitVblankStart();
	}
	sceKernelExitGame();
}