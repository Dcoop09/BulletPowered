#include <pspkernel.h>
#include <graphics.h>

#include "UI.h"

extern unsigned char font_start[];

Tilemap* font;

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
	if(font == NULL)
	{
		font = loadFont(font_start, 128, 128, 16, 16);
	}
	
	ScePspFVector3 gridpos = {-100.0f, 70.0f, -100.0f};
	loadTexture(NULL, 0, 0, 0, 0);
	renderUI(gridpos, 45, (ScePspFVector3){1, 1, 1}, sizeof(grid)/sizeof(grid[0]), grid);

	drawText(font, (ScePspFVector3){-10.0, 5.0, -10.0}, 0.0f, 0.5f, "Hello World");
}

void UIUpdate(float delta)
{
    render();
}