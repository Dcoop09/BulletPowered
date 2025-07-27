#include <stdlib.h>

#include "application.h"
#include "callbacks.h"

//Tells engine that there a app to run
extern Application* createApplication();

int main()
{
	Application* app = createApplication();
	runProcess(app);
	free(app);

	return 0;
}
