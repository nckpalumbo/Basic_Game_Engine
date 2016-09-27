#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <FreeImage.h>
#include "Engine.h"

#include <vector>
using std::vector;

int main()
{
	//Declare engine
	Engine engine;
	if (!engine.init()) return -1;
	if (!engine.bufferModels()) return -1;

	if (engine.useShaders() == false)
	{
		fgetc(stdin);
		return -1;
	}
		
	engine.gameLoop();

	//std::cin.get();
	return 0;
}