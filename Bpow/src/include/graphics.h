#pragma once

//The camera the player views from
typedef struct
{
	float x,y,z;
	float yaw, pitch;
}Camera3D;

//One part of a mesh
typedef struct
{
	float u, v;
	unsigned short color;
	float x,y,z;
}Vertex;


//do not use
typedef struct
{
	Vertex tileuv[6];
}Tile;

//use with load font
typedef struct
{
	void* tex;
	int textureSizeX;
	int textureSizeY;
	int spriteSizeX;
	int spriteSizeY;
	Tile* tiles;
}Tilemap;

//Sets up graphic settings
void initGraphics();

//Begins rendering process
void startframe(Camera3D cam);

//Ends rendering process
void endframe();

//load texture into ram for next meshes (needs swizzle)
void loadTexture(void* textureName, int textureX, int textureY, char vram, char sharp);

//positions and renders vertices
void renderMesh(ScePspFVector3 pos, ScePspFVector3 rot, ScePspFVector3 scale, int verticeCount, void* vertices);

//render object that follows camera
void renderSprite(ScePspFVector3 pos, float rot, ScePspFVector3 scale, int verticeCount, void* vertices);

//renders object in front and in a orthographic view and it follows the camera
void renderUI(ScePspFVector3 pos, float rot, ScePspFVector3 scale, int verticeCount, void* vertices);

//loads font to use (texture, the width of texture, the height of the texture, character columns, character rows)
Tilemap* loadFont(char* textureName, int texSizeX, int texSizeY, int sizeX, int sizeY);

//draws a string using a tilemap
void drawText(Tilemap* t, ScePspFVector3 pos, float rot, float scale, const unsigned char* str);