#include <pspkernel.h>
#include <graphics.h>

#include "rainbow_plane.h"

static Vertex __attribute__((aligned(16))) grid[2*3] =
{
	{1, 0, 0x801f,-10,-10, 0}, // 0
	{0, 0, 0x0000,-10, 10, 0}, // 1
	{0, 1, 0x83e0, 10, 10, 0}, // 2

	{1, 0, 0x801f,-10,-10, 0}, // 0
	{0, 1, 0x83e0, 10, 10, 0}, // 2
	{1, 1, 0xfc00, 10,-10, 0}, // 3
};

static void render() 
{
	ScePspFVector3 gridpos = {0.0f, 0.0f, -10.0f};
	ScePspFVector3 gridrot = {0.0f, 0.0f, -0.0f};
	loadTexture(NULL, 0, 0, 0, 0);
	renderMesh(gridpos, gridrot, (ScePspFVector3){1, 1, 1}, sizeof(grid)/sizeof(grid[0]), grid);
}

void planeUpdate(float delta)
{
    render();
}