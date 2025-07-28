#pragma once
#include <stdio.h>

//looks for file in asset folder and returns it's contents
void* openFile(const char* targetFile);

void* openTexFile(const char* targetTex);