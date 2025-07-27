#include <pspdebug.h>
#include <stdio.h>

#include "logger.h"

//
void print(const char *string, ...) 
{
	printf(string);
	printf("\n");
}

void printValue(float value)
{
	char string[15];

	sprintf(string, "%f", value);
  	print(string);
}

void throwError(const char *string, ...) 
{
	pspDebugScreenSetTextColor(0x0000FF);
	printf("\x1b[31m");
	printf(string);
	printf("\x1b[0m\n");
}