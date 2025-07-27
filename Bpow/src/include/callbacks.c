#include <pspkernel.h>
#include <pspdisplay.h>
#include <pspdebug.h>

static int exitRequest = 0;

int running()
{
	return !exitRequest;
}

//Idk bro
int exitCallback(int arg1, int arg2, void *common)
{
	exitRequest = 1;
	return 0;
}

//Makes it so the psp can end the program or something
int callbackThread(SceSize args, void *argp)
{
	int cbid;

	cbid = sceKernelCreateCallback("Exit Callback", exitCallback, NULL);
	sceKernelRegisterExitCallback(cbid);

	sceKernelSleepThreadCB();

	return 0;
}

int setupCallbacks(void)
{
	int thid = 0;

	thid = sceKernelCreateThread("update_thread", callbackThread, 0x11, 0xFA0, 0, 0);
	if(thid >= 0)
	{
		sceKernelStartThread(thid, 0, 0);
	}

	return thid;
}
