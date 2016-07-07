#include "Application.h"

int main(char** args)
{

	Application app = Application();

	if (!app.init())
		exit(-1);

	app.run();

	return 0;
}