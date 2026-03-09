#include <pspkernel.h>
#include <graphics.h>
#include <file.h>
#include <logger.h>

#include "UI.h"

#define fontSizeX 128
#define fontSizeY 128
#define charSizeX 16
#define charSizeY 16

extern unsigned char font_start[];

Tilemap* font;
char* content;

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
		font = loadFont("font.data", fontSizeX, fontSizeY, charSizeX, charSizeY);
		content = openFile("test.txt");
	}
	
	ScePspFVector3 gridpos = {-100.0f, 70.0f, -100.0f};
	loadTexture(NULL, 0, 0, VRAM_FALSE, SHARP_FALSE);
	renderUI(gridpos, 45, (ScePspFVector3){1, 1, 1}, sizeof(grid)/sizeof(grid[0]), grid);

	drawText(font, (ScePspFVector3){-10.0, 5.0, -10.0}, 0.0f, 0.5f, content);
}

void UIUpdate(float delta)
{
    render();
}