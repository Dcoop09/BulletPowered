#pragma once
#include <stdio.h>

#define BASICFILE 0
#define AUDIOFILE 1
#define ASSETFILE 2
#define TEXTUREFILE 3

//looks for file in asset folder and returns it's contents
int openFile(const char* targetFile, char type);

void* readTexFile(const char* targetTex);

void closeFile();

void* getFileData(int fd);

int getFileBuffer(int fd);
