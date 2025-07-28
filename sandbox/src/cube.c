#include <pspkernel.h>
#include <graphics.h>
#include <input.h>
#include <file.h>

Vertex __attribute__((aligned(16))) vertices[12*3] =
{
	{1, 1, 0x8000,-1,-1, 1}, // 0
	{1, 0, 0x8000,-1, 1, 1}, // 4
	{0, 0, 0x8000, 1, 1, 1}, // 5

	{1, 1, 0x8000,-1,-1, 1}, // 0
	{0, 0, 0x8000, 1, 1, 1}, // 5
	{0, 1, 0x8000, 1,-1, 1}, // 1

	{0, 0, 0xfc00,-1,-1,-1}, // 3
	{1, 0, 0xfc00, 1,-1,-1}, // 2
	{1, 1, 0xfc00, 1, 1,-1}, // 6

	{0, 0, 0xfc00,-1,-1,-1}, // 3
	{1, 1, 0xfc00, 1, 1,-1}, // 6
	{0, 1, 0xfc00,-1, 1,-1}, // 7

	{0, 0, 0x83e0, 1,-1,-1}, // 0
	{1, 0, 0x83e0, 1,-1, 1}, // 3
	{1, 1, 0x83e0, 1, 1, 1}, // 7

	{0, 0, 0x83e0, 1,-1,-1}, // 0
	{1, 1, 0x83e0, 1, 1, 1}, // 7
	{0, 1, 0x83e0, 1, 1,-1}, // 4

	{0, 0, 0x83e0,-1,-1,-1}, // 0
	{1, 0, 0x83e0,-1, 1,-1}, // 3
	{1, 1, 0x83e0,-1, 1, 1}, // 7

	{0, 0, 0x83e0,-1,-1,-1}, // 0
	{1, 1, 0x83e0,-1, 1, 1}, // 7
	{0, 1, 0x83e0,-1,-1, 1}, // 4

	{0, 0, 0x801f,-1, 1,-1}, // 0
	{1, 0, 0x801f, 1, 1,-1}, // 1
	{1, 1, 0x801f, 1, 1, 1}, // 2

	{0, 0, 0x801f,-1, 1,-1}, // 0
	{1, 1, 0x801f, 1, 1, 1}, // 2
	{0, 1, 0x801f,-1, 1, 1}, // 3

	{0, 0, 0x801f,-1,-1,-1}, // 4
	{1, 0, 0x801f,-1,-1, 1}, // 7
	{1, 1, 0x801f, 1,-1, 1}, // 6

	{0, 0, 0x801f,-1,-1,-1}, // 4
	{1, 1, 0x801f, 1,-1, 1}, // 6
	{0, 1, 0x801f, 1,-1,-1}, // 5
};

float val = 0;
float cubeX = 0.0f;
float cubeY = 0.0f;
void* texture = NULL;

void renderCube() 
{
	if(!texture)
	{
		texture = openTexFile("colors.data");
	}
	ScePspFVector3 pos = {cubeX, cubeY, -5.5f};
	ScePspFVector3 rot = {val * 0.79f, val * 0.98f, val * 1.32f};
	ScePspFVector3 scale = {1, 1, 1};
	loadTexture(texture, 64, 64, 0, 0);
	renderMesh(pos, rot, scale,sizeof(vertices)/sizeof(vertices[0]), vertices);
}

void cubeUpdate(float delta) 
{
    cubeX += getXAnalog() * delta * 10;
	cubeY -= getYAnalog() * delta * 10;

    renderCube();

    val += 50 * delta;
}