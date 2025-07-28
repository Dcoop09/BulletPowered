#include <pspkernel.h>
#include <pspdebug.h>
#include <time.h>
#include <malloc.h>
#include <string.h>

#include "application.h"
#include "callbacks.h"
#include "logger.h"
#include "graphics.h"
#include "input.h"
#include "file.h"

#define BUF_WIDTH (512)
#define SCR_WIDTH (480)
#define SCR_HEIGHT (272)

//dynamic array for functions
typedef struct
{
	size_t length;
	size_t capacity;
	size_t itemSize;
	Process items;
}FunctionArray;

FunctionArray processes;

//sets up the processes array
FunctionArray initProcessList(size_t itemSize, size_t initialCapacity)
{
	return (FunctionArray)
	{
		.length = 0,
		.capacity = initialCapacity,
		.itemSize = itemSize,
		.items = malloc(itemSize * initialCapacity)
	};
}

Process getFunc(int index)
{
	Process tmp = (Process)((unsigned int*)processes.items)[index];
	if(tmp) 
	{
		return tmp;
	} else
	{
		return NULL;
	}
}

int setFunc(int index, Process func)
{
	if(index > processes.capacity) 
	{
		return -1;
	} 
	memcpy(processes.items + index * processes.itemSize, &func, processes.itemSize);
	return 1;
}

int funcAppend(Process func)
{
	if(processes.length >= processes.capacity)
	{
		processes.capacity = processes.capacity > 0 ? processes.capacity * 2: 1;
		Process items = (Process)realloc(processes.items, processes.itemSize * processes.capacity);

		if(!items)
		{
			return -1;
		}
		processes.items = items;
	}

	unsigned int index;
	for(index = 0; index < processes.length; index++) 
	{
		if(!(getFunc(index)))
		{
			break;
		}
	}
	if(index >= processes.length)
	{
		processes.length++;
	}
	memcpy(processes.items + index * processes.itemSize, &func, processes.itemSize);

	return index;
}

int funcRemove(int index)
{
	if(!(getFunc(index))) 
	{
		return -1;
	}
	setFunc(index, (Process)0);
	return 1;
}

void runProcess(Application* app)
{
	setupCallbacks();

	initGraphics();

	ctrlInit();

	processes = initProcessList(sizeof(Process), 0);

	clock_t beginTime;
	clock_t endTime;
	float delta = 0.000001f;

	while(running()) 
	{
		beginTime = clock();

		startframe(*app->mainCamera);

		ctrlUpdate();

		print((char*)openFile("test.txt"));

		app->processMaster(delta);

		for(unsigned int i = 0; i <= processes.length - 1; i++)
		{
			Process process;
			process = (Process)((unsigned int*)processes.items)[i];
			if(process != 0)
			{
				process(delta);
			}
		}

		endframe();

		endTime = clock();
		delta = ((long)endTime - (long)beginTime)/ (float)CLOCKS_PER_SEC;
	}

	sceKernelExitGame();
	return;
}