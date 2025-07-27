#include <graphics.h>

typedef void (*Process)(float);

//Object that the engine uses
typedef struct
{
    void(*processMaster)(float delta);
    Camera3D* mainCamera;
}Application;

//Runs all programs
void runProcess(Application*);

//Adds process to be ran every frame and returns index in the array
int funcAppend(Process func);

//Removes a process from the list so it won't be ran
int funcRemove(int index);