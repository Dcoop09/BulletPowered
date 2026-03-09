#include <pspkernel.h>
#include <psputility.h>
#include <pspgu.h>
#include <pspdisplay.h>
#include <string.h>
#include <stdlib.h>
#include "savefile.h"
#include "graphics.h"

Savefile gameSave;

char nameMultiple[][20] =
{
 "0000",
 "0001",
 "0002",
 "0003",
 "0004",
 ""
};

static Vertex __attribute__((aligned(16))) plane[2*3] =
{
	{1, 0, 0xB566,-10,-10, 0}, // 0
	{0, 0, 0xB566,-10, 10, 0}, // 1
	{0, 1, 0xA906, 20, 10, 0}, // 2

	{1, 0, 0xB566,-10,-10, 0}, // 0
	{0, 1, 0xA906, 20, 10, 0}, // 2
	{1, 1, 0xA906, 20,-10, 0}, // 3
};

PspUtilitySavedataListSaveNewData newData;

char* titleShow = "New Save";

void initSavedata(SceUtilitySavedataParam * savedata, int mode)
{
	memset(savedata, 0, sizeof(SceUtilitySavedataParam));
	savedata->base.size = sizeof(SceUtilitySavedataParam);

	savedata->base.language = PSP_SYSTEMPARAM_LANGUAGE_ENGLISH;
	savedata->base.buttonSwap = PSP_UTILITY_ACCEPT_CROSS;
	savedata->base.graphicsThread = 0x11;
	savedata->base.accessThread = 0x13;
	savedata->base.fontThread = 0x12;
	savedata->base.soundThread = 0x10;

	savedata->mode = mode;
	savedata->overwrite = 1;
	savedata->focus = PSP_UTILITY_SAVEDATA_FOCUS_LATEST; // Set initial focus to the newest file (for loading)

#if defined(_PSP_FW_VERSION) && _PSP_FW_VERSION >= 200
    char key[] = "QTAK319JQKJ952HA";
	strncpy(savedata->key, key, 16);
#endif

	strcpy(savedata->gameName, gameSave.gameSerial);	// First part of the save name, game identifier name
	strcpy(savedata->saveName, "0000");	// Second part of the save name, save identifier name

	// List of multiple names
	savedata->saveNameList = nameMultiple;

	strcpy(savedata->fileName, "DATA.BIN");	// name of the data file

	// Allocate buffers used to store various parts of the save data
	savedata->dataBuf = malloc(gameSave.dataSize);
	savedata->dataBufSize = gameSave.dataSize;
	savedata->dataSize = gameSave.dataSize;

	// Set some data
	if (mode == PSP_UTILITY_SAVEDATA_LISTSAVE)
	{
	 memset(savedata->dataBuf, 0, gameSave.dataSize);
	 strcpy(savedata->dataBuf, gameSave.data);

	strcpy(savedata->sfoParam.title, gameSave.title);
	strcpy(savedata->sfoParam.savedataTitle, gameSave.title);
	strcpy(savedata->sfoParam.detail, gameSave.info);
	savedata->sfoParam.parentalLevel = 1;
	
	// No icon1
	savedata->icon1FileData.buf = NULL;
	savedata->icon1FileData.bufSize = 0;
	savedata->icon1FileData.size = 0;

	savedata->pic1FileData.buf = gameSave.backgroundPicture;
	savedata->pic1FileData.bufSize = gameSave.backgroundPictureSize;
	savedata->pic1FileData.size = gameSave.backgroundPictureSize;

	savedata->icon0FileData.buf = gameSave.iconPicture;
	savedata->icon0FileData.bufSize = gameSave.iconPictureSize;
	savedata->icon0FileData.size = gameSave.iconPictureSize;
	
	// No snd0
	savedata->snd0FileData.buf = NULL;
	savedata->snd0FileData.bufSize = 0;
	savedata->snd0FileData.size = 0;

	// Set title
	newData.title = titleShow;

	// Set new data
	savedata->newData = &newData;
	
	savedata->focus = PSP_UTILITY_SAVEDATA_FOCUS_FIRSTEMPTY; // If saving, set inital focus to the first empty slot
	}
}

static void showSaveDialog (int mode)
{
    Camera3D cam = 
    {
	    .x = 0.0f,
	    .y = 0.0f,
	    .z = 0.0f,
	    .yaw = 0.0f,
    	.pitch = 0.0f
    };

	SceUtilitySavedataParam dialog;

    initSavedata(&dialog, mode);

    sceUtilitySavedataInitStart(&dialog);

	while(1==1) {

	ScePspFVector3 gridpos = {-5.0f, 0.0f, -10.0f};
	ScePspFVector3 gridrot = {0.0f, 0.0f, -0.0f};
	loadTexture(NULL, 0, 0, VRAM_FALSE, SHARP_FALSE);
	renderMesh(gridpos, gridrot, (ScePspFVector3){1, 1, 1}, sizeof(plane)/sizeof(plane[0]), plane);

    sceGuFinish();
    sceGuSync(0,0);

	switch(sceUtilitySavedataGetStatus()) {
    case PSP_UTILITY_DIALOG_INIT:
        break;

	case PSP_UTILITY_DIALOG_VISIBLE :
	    sceUtilitySavedataUpdate(1);
	    break;

	case PSP_UTILITY_DIALOG_QUIT :

	    sceUtilitySavedataShutdownStart();
	    break;
	    
	case PSP_UTILITY_DIALOG_FINISHED :
        if(mode = PSP_UTILITY_SAVEDATA_LISTLOAD) 
		{
			gameSave.data = dialog.dataBuf;
		}
			
	case PSP_UTILITY_DIALOG_NONE :
	    return;
	}
	sceDisplayWaitVblankStart();
	sceGuSwapBuffers();

	//called last so order isn't messed up
	startframe(cam);
    }
}

void setSave(Savefile* save) 
{
    gameSave.title = save->title;
    gameSave.gameSerial = save->gameSerial;
    gameSave.info = save->info;
    gameSave.iconPicture = save->iconPicture;
    gameSave.iconPictureSize = save->iconPictureSize;
    gameSave.backgroundPicture = save->backgroundPicture;
    gameSave.backgroundPictureSize = save->backgroundPictureSize;
    gameSave.data = save->data;
    gameSave.dataSize = save->dataSize;
}

void saveSavefile(Savefile* save) 
{
    setSave(save);
    showSaveDialog(PSP_UTILITY_SAVEDATA_LISTSAVE);
}

void* openSavefile(Savefile* save) 
{
    setSave(save);
    showSaveDialog(PSP_UTILITY_SAVEDATA_LISTLOAD);
	return gameSave.data;
}