#include <pspctrl.h>

#include "input.h"

static SceCtrlData pad;
static SceCtrlLatch latch;

char selectDown;
char startDown;
char circleDown;
char triangleDown;
char squareDown;
char crossDown;
char rightDown;
char upDown;
char leftDown;
char downDown;
char ltriggerDown;
char rtriggerDown;
char holdDown;

char selectPressed;
char startPressed;
char circlePressed;
char trianglePressed;
char squarePressed;
char crossPressed;
char rightPressed;
char upPressed;
char leftPressed;
char downPressed;
char ltriggerPressed;
char rtriggerPressed;
char holdPressed;

float getXAnalog() 
{
    float result = pad.Lx;
    result /= 127.5f;
    result -= 1.0f;

    if(result < 0.2f && result > -0.2f) return 0.0f;

    return result;
}

float getYAnalog() 
{
    float result = pad.Ly;
    result /= 127.0f;
    result -= 1.0f;

    if(result < 0.2f && result > -0.2f) return 0.0f;

    return result;
}

void ctrlInit() 
{
    sceCtrlSetSamplingCycle(0);
	sceCtrlSetSamplingMode(PSP_CTRL_MODE_ANALOG);
}

void ctrlUpdate() 
{
	sceCtrlPeekBufferPositive(&pad, 1);
    sceCtrlReadLatch(&latch);

    selectDown =      (latch.uiPress & PSP_CTRL_SELECT) ? 1 : 0;
    startDown =       (latch.uiPress & PSP_CTRL_START) ? 1 : 0;
    circleDown =      (latch.uiPress & PSP_CTRL_CIRCLE) ? 1 : 0;
    triangleDown =    (latch.uiPress & PSP_CTRL_TRIANGLE) ? 1 : 0;
    squareDown =      (latch.uiPress & PSP_CTRL_SQUARE) ? 1 : 0;
    crossDown =       (latch.uiPress & PSP_CTRL_CROSS) ? 1 : 0;
    rightDown =       (latch.uiPress & PSP_CTRL_RIGHT) ? 1 : 0;
    upDown =          (latch.uiPress & PSP_CTRL_UP) ? 1 : 0;
    leftDown =        (latch.uiPress & PSP_CTRL_LEFT) ? 1 : 0;
    downDown =        (latch.uiPress & PSP_CTRL_DOWN) ? 1 : 0;
    ltriggerDown =    (latch.uiPress & PSP_CTRL_LTRIGGER) ? 1 : 0;
    rtriggerDown =    (latch.uiPress & PSP_CTRL_RTRIGGER) ? 1 : 0;
    holdDown =        (latch.uiPress & PSP_CTRL_HOLD) ? 1 : 0;

    selectPressed =   (latch.uiMake & PSP_CTRL_SELECT) ? 1 : 0;
    startPressed =    (latch.uiMake & PSP_CTRL_START) ? 1 : 0;
    circlePressed =   (latch.uiMake & PSP_CTRL_CIRCLE) ? 1 : 0;
    trianglePressed = (latch.uiMake & PSP_CTRL_TRIANGLE) ? 1 : 0;
    squarePressed =   (latch.uiMake & PSP_CTRL_SQUARE) ? 1 : 0;
    crossPressed =    (latch.uiMake & PSP_CTRL_CROSS) ? 1 : 0;
    rightPressed =    (latch.uiMake & PSP_CTRL_RIGHT) ? 1 : 0;
    upPressed =       (latch.uiMake & PSP_CTRL_UP) ? 1 : 0;
    leftPressed =     (latch.uiMake & PSP_CTRL_LEFT) ? 1 : 0;
    downPressed =     (latch.uiMake & PSP_CTRL_DOWN) ? 1 : 0;
    ltriggerPressed = (latch.uiMake & PSP_CTRL_LTRIGGER) ? 1 : 0;
    rtriggerPressed = (latch.uiMake & PSP_CTRL_RTRIGGER) ? 1 : 0;
    holdPressed =     (latch.uiMake & PSP_CTRL_HOLD) ? 1 : 0;
}