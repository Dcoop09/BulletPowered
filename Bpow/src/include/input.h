#pragma once

extern char selectDown;
extern char startDown;
extern char circleDown;
extern char triangleDown;
extern char squareDown;
extern char crossDown;
extern char rightDown;
extern char upDown;
extern char leftDown;
extern char downDown;
extern char ltriggerDown;
extern char rtriggerDown;
extern char holdDown;

extern char selectPressed;
extern char startPressed;
extern char circlePressed;
extern char trianglePressed;
extern char squarePressed;
extern char crossPressed;
extern char rightPressed;
extern char upPressed;
extern char leftPressed;
extern char downPressed;
extern char ltriggerPressed;
extern char rtriggerPressed;
extern char holdPressed;

//returns the analog stick's x axis
float getXAnalog();

//returns the analog stick's y axis
float getYAnalog();

//done in engine do not use
void ctrlInit();
//done in engine do not use
void ctrlUpdate();