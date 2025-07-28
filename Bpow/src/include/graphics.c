#include <pspdisplay.h>
#include <pspgu.h>
#include <pspgum.h>
#include <malloc.h>
#include <string.h>

#include "graphics.h"
#include "file.h"

#define BUF_WIDTH (512)
#define SCR_WIDTH (480)
#define SCR_HEIGHT (272)

static Camera3D mainCam;

//262144 is the closest power of 2 to 512 * 480 without going under(pow2(BUF_WIDTH * SCR_HEIGHT))
static unsigned int __attribute__((aligned(16))) list[262144];

static unsigned int getMemorySize(unsigned int width, unsigned int height, unsigned int psm)
{
	switch (psm)
	{
		case GU_PSM_T4:
			return (width * height) >> 1;

		case GU_PSM_T8:
			return width * height;

		case GU_PSM_5650:
		case GU_PSM_5551:
		case GU_PSM_4444:
		case GU_PSM_T16:
			return 2 * width * height;

		case GU_PSM_8888:
		case GU_PSM_T32:
			return 4 * width * height;

		default:
			return 0;
	}
}

//gets unused address
void* getStaticVramBuffer(unsigned int width, unsigned int height, unsigned int psm)
{
	static unsigned int staticOffset = 0;
	unsigned int memSize = getMemorySize(width,height,psm);
	void* result = (void*)staticOffset;
	staticOffset += memSize;

	return result;
}

//returns address in the vram
void* getStaticVramTexture(unsigned int width, unsigned int height, unsigned int psm)
{
	void* result = getStaticVramBuffer(width,height,psm);
	return (void*)(((unsigned int)result) + ((unsigned int)sceGeEdramGetAddr()));
}

//gets lowest power of 2 without going under
unsigned int pow2(const unsigned int value) 
{
	unsigned int poweroftwo = 1;
	while (poweroftwo < value) {
		poweroftwo *= 2;
	}
	return poweroftwo;
}

//TODO: Learn bit swizzling
/*void swizzle_fast(u8 *out, const u8 *in, const unsigned int width, const unsigned int height) {
    unsigned int blockx, blocky;
    unsigned int j;

    unsigned int width_blocks = (width / 16);
    unsigned int height_blocks = (height / 8);

    unsigned int src_pitch = (width - 16) / 4;
    unsigned int src_row = width * 8;

    const u8 *ysrc = in;
    u32 *dst = (u32 *)out;

    for (blocky = 0; blocky < height_blocks; ++blocky) {
        const u8 *xsrc = ysrc;
        for (blockx = 0; blockx < width_blocks; ++blockx) {
            const u32 *src = (u32 *)xsrc;
            for (j = 0; j < 8; ++j) {
                *(dst++) = *(src++);
                *(dst++) = *(src++);
                *(dst++) = *(src++);
                *(dst++) = *(src++);
                src += src_pitch;
            }
            xsrc += 16;
        }
        ysrc += src_row;
    }
}*/


void initGraphics() 
{
	void* fbp0 = guGetStaticVramBuffer(BUF_WIDTH,SCR_HEIGHT,GU_PSM_5551);
	void* fbp1 = guGetStaticVramBuffer(BUF_WIDTH,SCR_HEIGHT,GU_PSM_5551);
	void* zbp = guGetStaticVramBuffer(BUF_WIDTH,SCR_HEIGHT,GU_PSM_4444);

	sceGuInit();

	sceGuStart(GU_DIRECT,list);
	sceGuDrawBuffer(GU_PSM_5551,fbp0,BUF_WIDTH);
	sceGuDispBuffer(SCR_WIDTH,SCR_HEIGHT,fbp1,BUF_WIDTH);
	sceGuDepthBuffer(zbp,BUF_WIDTH);
	sceGuOffset(2048 - (SCR_WIDTH/2),2048 - (SCR_HEIGHT/2));
	sceGuViewport(2048,2048,SCR_WIDTH,SCR_HEIGHT);
	sceGuDepthRange(65535,0);
	sceGuScissor(0,0,SCR_WIDTH,SCR_HEIGHT);
	sceGuEnable(GU_SCISSOR_TEST);
	sceGuDepthFunc(GU_GEQUAL);
	sceGuEnable(GU_DEPTH_TEST);
	sceGuFrontFace(GU_CW);
	sceGuShadeModel(GU_SMOOTH);
	sceGuEnable(GU_ALPHA_TEST);
	sceGuAlphaFunc(GU_GREATER, 128, 0xFF);
	sceGuEnable(GU_CULL_FACE);
	sceGuEnable(GU_TEXTURE_2D);
	sceGuEnable(GU_CLIP_PLANES);
	sceGuEnable(GU_DITHER);
	sceGuFinish();
	sceGuSync(0,0);

	sceDisplayWaitVblankStart();
	sceGuDisplay(GU_TRUE);
	return;
}

//translate the camera to the new position givin by it's transform
void apply_camera()
{
	sceGumMatrixMode(GU_VIEW);
	sceGumLoadIdentity();

	ScePspFVector3 pos = {mainCam.x, mainCam.y, mainCam.z};
	ScePspFVector3 rot = {(mainCam.pitch / 180.0f) * GU_PI, (mainCam.yaw / 180.0f) * GU_PI, 0.0f};
	sceGumRotateXYZ(&rot);
	sceGumTranslate(&pos);

	sceGumMatrixMode(GU_MODEL);
	sceGumLoadIdentity();
	return;
}

void startframe(Camera3D cam) 
{
	sceGuStart(GU_DIRECT,list);

	sceGuClearColor(0xff504030);
	sceGuClearDepth(0);
	sceGuClear(GU_COLOR_BUFFER_BIT|GU_DEPTH_BUFFER_BIT);

	sceGumMatrixMode(GU_PROJECTION);
	sceGumLoadIdentity();
	sceGumPerspective(75.0f,16.0f/9.0f,0.5f,1000.0f);

	sceGumMatrixMode(GU_VIEW);
	sceGumLoadIdentity();

	sceGumMatrixMode(GU_MODEL);
	sceGumLoadIdentity();

	mainCam = cam;
	apply_camera(mainCam);
	return;
}

void endframe() 
{
	sceGuFinish();
	sceGuSync(0,0);

	sceDisplayWaitVblankStart();
	sceGuSwapBuffers();
	return;
}

void loadTexture(void* texture, int textureX, int textureY, char vram, char sharp)
{
	if(texture == NULL) 
	{
		sceGuDisable(GU_TEXTURE_2D);
		return;

	} else 
	{
		sceGuEnable(GU_TEXTURE_2D);

		unsigned int pW, pH;
		pW = pow2(textureX);
		pH = pow2(textureY);

		void* tex = NULL;
		if(vram)
		{
			tex = getStaticVramTexture(pW, pH, GU_PSM_4444);
			tex = texture;
		} else
		{
			tex = texture;
		}

		//maybe add swizzle some time
		sceGuTexMode(GU_PSM_4444, 0, 0, 0);
		sceGuTexImage(0, pW, pH, pW, tex);
		sceGuTexFunc(GU_TFX_ADD, GU_TCC_RGBA);
		sceGuTexEnvColor(0xffff00);

		if(sharp) 
		{
			sceGuTexFilter(GU_NEAREST, GU_NEAREST);
		} else
		{
			sceGuTexFilter(GU_LINEAR, GU_LINEAR);
		}

		sceGuTexScale(1.0f, 1.0f);
		sceGuTexOffset(0.0f, 0.0f);
		sceGuAmbientColor(0xffffffff);
		return;
	}
}

void renderMesh(ScePspFVector3 pos, ScePspFVector3 rot, ScePspFVector3 scale, int verticeCount, void* vertices)
{
	ScePspFVector3 targetRot = {rot.x / 180.0f * GU_PI, rot.y / 180.0f * GU_PI, rot.z / 180.0f * GU_PI};
	sceGumLoadIdentity();
	sceGumTranslate(&pos);
	sceGumRotateXYZ(&targetRot);
	sceGumScale(&scale);

	sceGumDrawArray(GU_TRIANGLES, GU_TEXTURE_32BITF|GU_COLOR_5551|GU_VERTEX_32BITF|GU_TRANSFORM_3D, verticeCount, 0, vertices);
	return;
}

void renderSprite(ScePspFVector3 pos, float rot, ScePspFVector3 scale, int verticeCount, void* vertices)
{
	ScePspFVector3 targetRot = {(-mainCam.pitch / 180.0f) * GU_PI, (-mainCam.yaw / 180.0f) * GU_PI, rot / 180.0f * GU_PI};

	sceGumLoadIdentity();
	sceGumTranslate(&pos);
	sceGumRotateXYZ(&targetRot);
	sceGumScale(&scale);

	sceGumDrawArray(GU_TRIANGLES, GU_TEXTURE_32BITF|GU_COLOR_5551|GU_VERTEX_32BITF|GU_TRANSFORM_3D, verticeCount, 0, vertices);
	return;
}

void renderUI(ScePspFVector3 pos, float rot, ScePspFVector3 scale, int verticeCount, void* vertices)
{

	ScePspFVector3 targetRot = {0.0f, 0.0f, rot / 180.0f * GU_PI};

	//set the camera to zero to render it
	sceGumMatrixMode(GU_VIEW);
	sceGumLoadIdentity();

	ScePspFVector3 camPos = {0, 0, 1};
	ScePspFVector3 camRot = {0, 0, 0};
	sceGumRotateXYZ(&camRot);
	sceGumTranslate(&camPos);

	sceGumMatrixMode(GU_MODEL);
	sceGumLoadIdentity();

	//No Depth (renders on top)
	sceGuDepthRange(65535,65535);

	sceGumLoadIdentity();
	sceGumTranslate(&pos);
	sceGumRotateXYZ(&targetRot);
	sceGumScale(&scale);

	sceGumDrawArray(GU_TRIANGLES, GU_TEXTURE_32BITF|GU_COLOR_5551|GU_VERTEX_32BITF|GU_TRANSFORM_3D, verticeCount, 0, vertices);

	//reset for other meshes

	sceGuDepthRange(65535,0);

	apply_camera();
	return;
}

Tilemap* loadFont(char* textureName, int texSizeX, int texSizeY, int sizeX, int sizeY) 
{
	Tilemap* tilemap = (Tilemap*)malloc(sizeof(Tilemap));
		if(tilemap == NULL) return NULL;

	tilemap->tex = (void*)malloc(sizeof(unsigned short) * texSizeX * texSizeY);
	if(tilemap->tex == NULL)
	{
		free(tilemap);
		return NULL;
	}

	tilemap->tiles = (Tile*)malloc(sizeof(Tile) * sizeX * sizeY);
	if(tilemap->tiles == NULL)
	{
		free(tilemap);
		free(tilemap->tex);
		return NULL;
	}


	memset(tilemap->tiles, 0, sizeof(Tile) * sizeX * sizeY);

	tilemap->tex = openTexFile(textureName);
	tilemap->textureSizeX = texSizeX;
	tilemap->textureSizeY = texSizeY;
	tilemap->spriteSizeX = texSizeX / sizeX;
	tilemap->spriteSizeY = texSizeY / sizeY;

	for(int i = 0; i < sizeX * sizeY; i++) 
	{
		int row = i / (int)sizeX;
		int column = i % (int)sizeX;

		float uvsizeX = 1.f / ((float)sizeX);
		float uvsizeY = 1.f / ((float)sizeY);

		float h = ((float)row * uvsizeY);  //(. , 1)
		float y = h + uvsizeY;             //(. , 0)
		float x = (float)column * uvsizeX; //(0 , .)
		float w = x + uvsizeX;             //(1 , .)

		tilemap->tiles[i].tileuv[0] = (Vertex){x, y, 0x8000,-1,-1, 0};
		tilemap->tiles[i].tileuv[1] = (Vertex){x, h, 0x8000,-1, 1, 0};
		tilemap->tiles[i].tileuv[2] = (Vertex){w, h, 0x8000, 1, 1, 0};

		tilemap->tiles[i].tileuv[3] = (Vertex){x, y, 0x8000,-1,-1, 0};
		tilemap->tiles[i].tileuv[4] = (Vertex){w, h, 0x8000, 1, 1, 0};
		tilemap->tiles[i].tileuv[5] = (Vertex){w, y, 0x8000, 1,-1, 0};


	}

	return tilemap;
}

void drawText(Tilemap* t, ScePspFVector3 pos, float rot, float scale, const unsigned char* str)
{
	int len = strlen(str);
	ScePspFVector3 targetScale = {scale, scale, scale};

	for(int i = 0; i < len; i++)
	{
		loadTexture(t->tex, t->textureSizeX, t->textureSizeY, 1, 1);
		renderUI(pos, rot, targetScale, 6, t->tiles[str[i]].tileuv);

		pos.x += 2.0f * scale;
	}
	return;
}