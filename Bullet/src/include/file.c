#include <pspuser.h>
#include <stdio.h>
#include <string.h>
#include <malloc.h>
#include <cmake_config.h>
#include "file.h"
#include "logger.h"

#define FILE_CHAR_LIMIT 20

//FILE CHAR LIMIT + the path size

long getFileSize(int fd) 
{
    sceIoLseek(fd, 0, SEEK_END);
    long length = sceIoLseek(fd, 0, SEEK_CUR);
    sceIoLseek(fd, 0, SEEK_SET);
    return length;
}

int getFileBuffer(int fd) 
{
    if(!fd)
    {
        throwError("failed to locate %d\n", fd);
        return 0;
    }
    int buf = getFileSize(fd);
    if(buf < 0)
    {
        throwError("failed to get size of %d\n", fd);
        return 0;
    }
    return buf;
}

int openFile(const char* targetFile, char type)
{
    char* targetPath = "";
    switch (type)
    {
        case 0:
            targetPath = malloc(FILE_CHAR_LIMIT * sizeof(char));
            targetPath = targetFile;
            break;
        case 1:
            targetPath = malloc(FILE_CHAR_LIMIT + strlen(AUDIO_FOLDER) * sizeof(char));
            sprintf(targetPath, "%s%s", AUDIO_FOLDER, targetFile);
            break;
        case 2:
            targetPath = malloc(FILE_CHAR_LIMIT + strlen(ASSET_FOLDER) * sizeof(char));
            sprintf(targetPath, "%s%s", ASSET_FOLDER, targetFile);
            break;
        case 3:
            targetPath = malloc(FILE_CHAR_LIMIT + strlen(TEXTURE_FOLDER) * sizeof(char));
            sprintf(targetPath, "%s%s", TEXTURE_FOLDER, targetFile);
            break;
        default:
            break;
    }
    int fd = sceIoOpen(targetPath, PSP_O_RDONLY, 0777);
    if(!fd) 
    {
        throwError("failed to locate %s\n", targetFile);
        return -1;
    }

    return fd;
}

void closeFile(int fd) 
{
    sceIoClose(fd);
}

void* getFileData(int fd) 
{
    long fileSize = getFileSize(fd);
    if(fileSize < 0)
    {
        throwError("failed to get size of %d\n", fd);
        return NULL;
    }
    void* fileContents = malloc(fileSize);
    sceIoRead(fd, fileContents, fileSize);
    return fileContents;
}

void* readTexFile(const char* targetTex) 
{
    char buffer[FILE_CHAR_LIMIT + strlen(TEXTURE_FOLDER)];
    sprintf(buffer, "%s%s", TEXTURE_FOLDER, targetTex);
    int fd = sceIoOpen(buffer, PSP_O_RDONLY, 0777);
    if(!fd) 
    {
        throwError("failed to locate %s\n", targetTex);
        return NULL;
    }
    long fileSize = getFileSize(fd);
    if(fileSize < 0)
    {
        throwError("failed to get size of %s\n", targetTex);
        return NULL;
    }
    print("%s\n", targetTex);

    void* fileContents = malloc(fileSize);
    sceIoRead(fd, fileContents, fileSize);
    sceIoClose(fd);

    return fileContents;
}