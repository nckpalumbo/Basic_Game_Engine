#include "Engine.h"
#include "ShaderManager.h"

namespace {
	std::map<int, bool> inputIsDown;
	std::map<int, bool> inputWasDown;

	//void mouseClick(GLFWwindow * windowPtr, int button, int action, int mods)
	//{
	//	inputIsDown[button] = action;
	//}
	void keyCallBack(GLFWwindow * window, int key, int scancode, int action, int mods)
	{
		inputIsDown[key] = action;
	}
}

#include <vector>
using std::vector;

Engine::Engine()
{
}


Engine::~Engine()
{
}

bool Engine::init()
{
	if (glfwInit() == GL_FALSE)
		return false;

	//GLFWwindowPtr = glfwCreateWindow(800, 800, "Nick Palumbo DSA1 Engine", glfwGetPrimaryMonitor(), NULL);
	GLFWwindowPtr = glfwCreateWindow(800, 800, "Nick Palumbo DSA1 Engine", NULL, NULL);
	//glfwSetMouseButtonCallback(GLFWwindowPtr, mouseClick);
	glfwSetKeyCallback(GLFWwindowPtr, keyCallBack);

	cameo = Camera(); //Create camera

	objects = vector<Object>(4);
	//objects =
	//{
	//	{ { { .4,.1,0 },{ 0,0,0 },{ .2,.2,1 }, glm::mat4(1)  }, filenames[0],{ { 0,0,0 },{ 0,0,0 }, 1 } },
	//	{ { { -.4,.6,0 },{ 0,0,0 },{ .2,.2,1 }, glm::mat4(1)  }, filenames[1],{ { 0,0,0 },{ 0,0,0 }, 2 } },
	//	{ { { 0,-.55,0 },{ 0,0,0 },{ 1,.45,1 }, glm::mat4(1)  }, filenames[2],{ { 0,0,0 },{ 0,0,0 }, 10 } },
	//	{ { { -.8,.02,0 },{ 0,0,0 },{ .12,.12,1 }, glm::mat4(1)  }, filenames[3],{ { 0,0,0 },{ 0,0,0 }, 1 } }
	//};
	objects[0] = Object{ { { .4,.1,0 },{ 0,0,0 },{ .2,.2,1 }, glm::mat4(1) }, filenames[0],{ { 0,0,0 },{ 0,0,0 },1 }, aabb};
	objects[1] = Object{ { { -.4,.1,0 },{ 4.5,0,0 },{ .16,.16,.16 }, glm::mat4(1) }, filenames[1],{ { 0,0,0 },{ 0,0,0 }, 1 }, sphere };
	objects[2] = Object{ { { 0,-.55,0 },{ 0,0,0 },{ 1.3,.45,1.1 }, glm::mat4(1) }, filenames[2],{ { 0,0,0 },{ 0,0,0 }, 1 }, none };
	objects[3] = Object{ { { -.8,.02,0 },{ 0,0,0 },{ .12,.12,1 }, glm::mat4(1) }, filenames[3],{ { 0,0,0 },{ 0,0,0 }, 1 }, sphere };

	deltaV = { 0,0,0 };
	deltaV2 = { 0,0,0 };

	if (GLFWwindowPtr != nullptr)
		glfwMakeContextCurrent(GLFWwindowPtr);

	else
	{
		glfwTerminate();
		return false;
	}

	if (glewInit() != GLEW_OK)
	{
		glfwTerminate();
		return false;
	}

	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	return true;
}

bool Engine::bufferModels()
{
	if (!objects[0].modelo.buffer("quad.obj"))
		return false;
	if (!objects[1].modelo.buffer("sphere.obj"))
		return false;
	if (!objects[2].modelo.buffer("box.obj"))
		return false;
	if (!objects[3].modelo.buffer("quad.obj"))
		return false;

	return true;
}

//collision detection
bool Engine::collides(const Object & a, const Object & b)
{
	//no collision
	if (a.colType == none && b.colType == none)
		return false;

	//aabb with aabb
	if (a.colType == aabb && b.colType == aabb)
	{
		if (fabs(a.tra.loc.x - b.tra.loc.x) > ( a.tra.size.x + b.tra.size.x))
		{
			return false;
		}
		else if (fabs(a.tra.loc.y - b.tra.loc.y) > (a.tra.size.y + b.tra.size.y))
		{
			return false;
		}
		else if (fabs(a.tra.loc.z - b.tra.loc.z) > (a.tra.size.z + b.tra.size.z))
		{
			return false;
		}
		else
			return true;
	}

	//aabb with sphere
	if (a.colType == aabb && b.colType == sphere) //It works as is..
	{
		// is sphere center left of box left wall? if so, add that distance^2 to d^2
		// do that on x, y, and z
		// d^2 starts at 0, increases 3 times, then at the end, is it > or < radius^2
		if (std::abs((b.tra.loc.x - a.tra.loc.x)) > (b.tra.size.x/1.8 + a.tra.size.x/1.8)) //I divide to make it a smaller radius because the normal texture is a 
																						   //square, so if i just do size, it is too big and detects it what looks like it is early
		{
			return false;
		}
		else if (std::abs((b.tra.loc.y - a.tra.loc.y)) > (b.tra.size.y/1.5 + a.tra.size.y/1.5))
		{
			return false;
		}
		else if (std::abs((b.tra.loc.z - a.tra.loc.z)) > (b.tra.size.z/1.8 + a.tra.size.z/1.8))
		{
			return false;
		}
		else
			return true;
	}

	//sphere with sphere 
	if (a.colType == sphere && b.colType == sphere)
	{
		if (glm::distance(a.tra.loc, b.tra.loc) > a.tra.size.x + b.tra.size.x)
		{
			return false;
		}
		else if (glm::distance(a.tra.loc, b.tra.loc) > a.tra.size.y + b.tra.size.y)
		{
			return false;
		}
		else if (glm::distance(a.tra.loc, b.tra.loc) > a.tra.size.z + b.tra.size.z)
		{
			return false;
		}
		else 
			return true;


		/*
		if ((a.tra.size.x / 2 + b.tra.loc.x / 2) < std::abs((a.tra.loc.x - b.tra.loc.x)))
		{
			return false;
		}
		else if ((a.tra.size.y / 2 + b.tra.loc.y / 2) < std::abs((a.tra.loc.y - b.tra.loc.y)))
		{
			return false;
		}*/

		/*else if ((a.tra.size.z / 2 + b.tra.loc.z / 2) < std::abs((a.tra.loc.z - b.tra.loc.z)))
		{
			return false;
		}*/
	}

	return false;
}

bool Engine::gameLoop()
{
	//Load textures
	FIBITMAP* images[4];
	images[0] = FreeImage_Load(FreeImage_GetFileType("images/luigi.png", 0), "images/luigi.png");
	images[1] = FreeImage_Load(FreeImage_GetFileType("images/boo.png", 0), "images/boo.png");
	images[2] = FreeImage_Load(FreeImage_GetFileType("images/bricks.jpg", 0), "images/bricks.jpg");
	images[3] = FreeImage_Load(FreeImage_GetFileType("images/mushroom.png", 0), "images/mushroom.png");

	if (images[0] == nullptr || images[1] == nullptr || images[2] == nullptr || images[3] == nullptr)
		std::cout << "Not working.";

	FIBITMAP* image32Bit = FreeImage_ConvertTo32Bits(images[0]);
	FreeImage_Unload(images[0]);
	FIBITMAP* image32Bit2 = FreeImage_ConvertTo32Bits(images[1]);
	FreeImage_Unload(images[1]);
	FIBITMAP* image32Bit3 = FreeImage_ConvertTo32Bits(images[2]);
	FreeImage_Unload(images[2]);
	FIBITMAP* image32Bit4 = FreeImage_ConvertTo32Bits(images[3]);
	FreeImage_Unload(images[3]);

	unsigned int texID[4];
	glGenTextures(4, &texID[0]);

	//Transparency
	glEnable(GL_BLEND);
	glEnable(GL_CULL_FACE);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	//upload texture 0
	glBindTexture(GL_TEXTURE_2D, texID[0]);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_SRGB_ALPHA, FreeImage_GetWidth(image32Bit),
		FreeImage_GetHeight(image32Bit), 0, GL_BGRA, GL_UNSIGNED_BYTE,
		(void*)FreeImage_GetBits(image32Bit));
	//upload texture 1
	glBindTexture(GL_TEXTURE_2D, texID[1]);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_SRGB_ALPHA, FreeImage_GetWidth(image32Bit2),
		FreeImage_GetHeight(image32Bit2), 0, GL_BGRA, GL_UNSIGNED_BYTE,
		(void*)FreeImage_GetBits(image32Bit2));
	//upload texture 2
	glBindTexture(GL_TEXTURE_2D, texID[2]);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_SRGB_ALPHA, FreeImage_GetWidth(image32Bit3),
		FreeImage_GetHeight(image32Bit3), 0, GL_BGRA, GL_UNSIGNED_BYTE,
		(void*)FreeImage_GetBits(image32Bit3));
	//upload texture 3
	glBindTexture(GL_TEXTURE_2D, texID[3]);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_SRGB_ALPHA, FreeImage_GetWidth(image32Bit4),
		FreeImage_GetHeight(image32Bit4), 0, GL_BGRA, GL_UNSIGNED_BYTE,
		(void*)FreeImage_GetBits(image32Bit4));

	//Start game timer
	time = (float)glfwGetTime();

	bool gameNotOver = true;
	while (!glfwWindowShouldClose(GLFWwindowPtr))
	{
		inputWasDown = inputIsDown;
		glfwPollEvents();

		//Update game timers
		prevTime = time;
		time = (float)glfwGetTime();
		deltaTime = time - prevTime;
		
		if (inputIsDown[GLFW_KEY_ESCAPE])
			glfwSetWindowShouldClose(GLFWwindowPtr, GL_TRUE);

		if (gameNotOver) //run processing until game ending collision occurs -- end state printed in console
		{
			//Input processing
			if ((inputIsDown[GLFW_KEY_A] || inputIsDown[GLFW_KEY_LEFT]) && (!(inputIsDown[GLFW_KEY_D]) || !(inputIsDown[GLFW_KEY_RIGHT])))
				objects[0].body.force.x = -1;
			else if ((inputIsDown[GLFW_KEY_D] || inputIsDown[GLFW_KEY_RIGHT]) && (!(inputIsDown[GLFW_KEY_A]) || !(inputIsDown[GLFW_KEY_LEFT])))
				objects[0].body.force.x = 1;
			else
				objects[0].body.force.x = 0; //No move if no press

			if (objects[0].tra.loc.x <= -.95 && objects[0].body.velocity.x < 0) //Keep him in bounds
				objects[0].body.velocity.x = 0;
			else if (objects[0].tra.loc.x >= .95 && objects[0].body.velocity.x > 0) //Keep him in bounds
				objects[0].body.velocity.x = 0;

			//ghost -- floats up and down on repeat -- forever
			float dist = .45 - objects[1].tra.loc.y;
			objects[1].body.force.y = 1 * dist;

			//Camera processing
			if (inputIsDown[GLFW_KEY_I] && !(inputIsDown[GLFW_KEY_K]))
				cameo.body.velocity.z = -.5;
			else if (inputIsDown[GLFW_KEY_K] && !(inputIsDown[GLFW_KEY_I]))
				cameo.body.velocity.z = .5;
			else
				cameo.body.velocity.z = 0;
			if (inputIsDown[GLFW_KEY_J] && !(inputIsDown[GLFW_KEY_L]))
				cameo.body.velocity.x = -.5;
			else if (inputIsDown[GLFW_KEY_L] && !(inputIsDown[GLFW_KEY_J]))
				cameo.body.velocity.x = .5;
			else
				cameo.body.velocity.x = 0;

			//if (inputIsDown[GLFW_KEY_U] && !(inputIsDown[GLFW_KEY_O]))
			//	cameo.body.velocity.x = -.5;
			//else if (inputIsDown[GLFW_KEY_O] && !(inputIsDown[GLFW_KEY_U]))
			//	cameo.body.velocity.x = .5;
			//else
			//	cameo.body.velocity.x = 0;

			//Mouse view
			float sens = .0003;
			int w = 800, h = 600;
			double x, y;
			glfwGetCursorPos(GLFWwindowPtr, &x, &y);

			cameo.camRot.y -= sens * (x - w * .5f); //Yaw
			cameo.camRot.x -= sens * (y - h * .5f); //Pitch
			cameo.camRot.x = glm::clamp(cameo.camRot.x, (-.5f * 3.14159f), (.5f * 3.14159f));

			glfwSetCursorPos(GLFWwindowPtr, 800 * .5f, 600 * .5f);
			glfwSetInputMode(GLFWwindowPtr, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);

			cameo.updateCamera(deltaTime);

			//update luigi force
			deltaV = (objects[0].body.force / objects[0].body.mass) * deltaTime;
			objects[0].body.velocity += deltaV;
			
			//update boo force
			deltaV2 = (objects[1].body.force / objects[1].body.mass) * deltaTime;
			objects[1].body.velocity += deltaV2;

			//Update position based on velocity
			objects[0].tra.loc += (objects[0].body.velocity * deltaTime);
			objects[1].tra.loc += (objects[1].body.velocity * deltaTime);

			//check for collisions
			if (collides(objects[0], objects[1]) == true)
			{
				std::cout << "YOU DIED! "; //touch boo
				gameNotOver = false;
			}
			if (collides(objects[0], objects[3]) == true)
			{
				std::cout << "YOU WON! "; //touch shroom
				gameNotOver = false;
			}
		}

		//render
		glClear(GL_COLOR_BUFFER_BIT);

		//updating in game loop
		for (int i = 0; i < 4; i++)
		{
			glm::mat4 scale = glm::scale(objects[i].tra.size);
			glm::mat4 rotate = glm::yawPitchRoll(objects[i].tra.rot.x, objects[i].tra.rot.y, objects[i].tra.rot.z);
			glm::mat4 translate = glm::translate(objects[i].tra.loc);

			objects[i].tra.worldTransform = translate * rotate * scale;

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

			glm::mat4 camMat = cameo.getMat();
			glm::vec3 lightLoc = {0, 8, 0};

			//Once per object
			glBindTexture(GL_TEXTURE_2D, texID[i]);
			glUniformMatrix4fv(2, 1, GL_FALSE, &objects[i].tra.worldTransform[0][0]);
			glUniformMatrix4fv(3, 1, GL_FALSE, &camMat[0][0]);
			glUniformMatrix3fv(5, 1, GL_FALSE, &cameo.camLoc[0]);
			glUniformMatrix3fv(6, 1, GL_FALSE, &lightLoc[0]);
			objects[i].modelo.render();
		}
		/*
		//render
		glClear(GL_COLOR_BUFFER_BIT);

		//one per obj
		glBindVertexArray(vertArr);
		glBindTexture(GL_TEXTURE_2D, texID[0]);
		glUniformMatrix4fv(2, 1, GL_FALSE, &objects[0].tra.worldTransform[0][0]);
		glDrawArrays(GL_TRIANGLES, 0, vertCount);

		glBindVertexArray(vertArr);
		glBindTexture(GL_TEXTURE_2D, texID[1]);
		glUniformMatrix4fv(2, 1, GL_FALSE, &objects[1].tra.worldTransform[0][0]);
		glDrawArrays(GL_TRIANGLES, 0, vertCount);

		glBindVertexArray(vertArr);
		glBindTexture(GL_TEXTURE_2D, texID[2]);
		glUniformMatrix4fv(2, 1, GL_FALSE, &objects[2].tra.worldTransform[0][0]);
		glDrawArrays(GL_TRIANGLES, 0, vertCount);

		glBindVertexArray(vertArr);
		glBindTexture(GL_TEXTURE_2D, texID[3]);
		glUniformMatrix4fv(2, 1, GL_FALSE, &objects[3].tra.worldTransform[0][0]);
		glDrawArrays(GL_TRIANGLES, 0, vertCount);
		*/
		glfwSwapBuffers(GLFWwindowPtr);
	}

	//unload images
	FreeImage_Unload(image32Bit);
	FreeImage_Unload(image32Bit2);
	FreeImage_Unload(image32Bit3);
	FreeImage_Unload(image32Bit4);

	glfwTerminate();

	return true;
}

bool Engine::useShaders()
{
	if (sm.loadShaders("shaders/vShader.glsl", "shaders/fShader.glsl"))
	{
		glUseProgram(sm.getProgram());
		return true;
	}

	return false;
}