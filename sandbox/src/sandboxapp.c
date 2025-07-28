#include <pspkernel.h>
#include <pspdebug.h>
#include <entrypoint.h>
#include <graphics.h>
#include <logger.h>

#include "cmake_config.h"

#include "cube.h"
#include "camera.h"
#include "rainbow_plane.h"
#include "UI.h"

PSP_MODULE_INFO("SandboxApp", 0, VERSION_MAJOR, VERSION_MINOR);

PSP_MAIN_THREAD_ATTR(THREAD_ATTR_USER);

char startup = 1;
char cube = -1;

void masterProcess(float delta)
{
    //cubeUpdate(delta);
    //cameraUpdate(delta);
    //planeUpdate(delta);
    //UIUpdate(delta);


    if(cube == -1)
    {
        cube = (char)funcAppend(&cubeUpdate);
    }

    if(cube != -1 && startup == 0) 
    {
        funcRemove(cube);
        cube = -1;
        startup = -1;
    }

    if(startup == 1) 
    {
        cube = (char)funcAppend(&cubeUpdate);
        funcAppend(&cameraUpdate);
        funcAppend(&planeUpdate);
        funcAppend(&UIUpdate);
        startup = 0;
    }
}

//Required for engine
Application* createApplication() 
{
    Application* app = malloc(sizeof(Application));
    //add master process so engine can ran it
    app->processMaster = masterProcess;
    //app also wants a camera :)
    app->mainCamera = &thisCamera;
    return app;
}