#include <string.h>
#include <malloc.h>
#include "file.h"
#include "logger.h"

#define FILE_CHAR_LIMIT 20

//FILE CHAR LIMIT + the path size
char textureFolder[FILE_CHAR_LIMIT + 18] = "assets/textures/";
char mp3Folder[FILE_CHAR_LIMIT + 13] = "assets/mp3/";
char assetFolder[FILE_CHAR_LIMIT + 9] = "assets/";

long getFileSize(FILE* file) 
{
    fseek(file, 0, SEEK_END);
    long length = ftell(file);
    fseek(file, 0, SEEK_SET);
    return length;
}

void* openFile(const char* targetFile)
{
    FILE* file = fopen(strcat(assetFolder, targetFile), "r");
    if(!file) 
    {
        throwError("failed to locate %s\n", assetFolder);
        return NULL;
    }
    long fileSize = getFileSize(file);
    if(fileSize < 0)
    {
        throwError("failed to get size of %s\n", assetFolder);
        return NULL;
    }

    void* fileContents = malloc(fileSize);
    fread(fileContents, sizeof(char), fileSize, file);
    fclose(file);

    strcpy(assetFolder, "assets/");

    return fileContents;
}

//TODO: texture revap
void* openTexFile(const char* targetTex) 
{
    FILE* file = fopen(strcat(textureFolder, targetTex), "r");
    if(!file) 
    {
        print("failed to locate %s\n", textureFolder);
        return NULL;
    }
    long fileSize = getFileSize(file);
    if(fileSize < 0)
    {
        print("failed to get size of %s\n", textureFolder);
        return NULL;
    }
    print("%s\n", textureFolder);

    void* fileContents = malloc(fileSize);
    fread(fileContents, sizeof(char), fileSize, file);
    fclose(file);

    strcpy(textureFolder, "assets/textures/");

    return fileContents;
}