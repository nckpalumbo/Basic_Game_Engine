#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <FreeImage.h>
#include "ShaderManager.h"
#include <map>
#include <string>
using std::string;
#include <iostream>

#include <vector>
using std::vector;

#include <glm/gtx/euler_angles.hpp>
#include <cmath>
#include "Camera.h"
#include "Model.h"

class Engine
{
private:

	struct Vertex
	{
		glm::vec3 loc;
		glm::vec2 uv;
	};

	struct Transform
	{
		glm::vec3 loc;
		glm::vec3 rot;
		glm::vec3 size;
		glm::mat4 worldTransform;
	};

	enum ColliderType { none, aabb, sphere };
	struct Rigidbody
	{
		glm::vec3 velocity;
		glm::vec3 force;
		float mass;
	};
	struct Object
	{
		Transform tra;
		string filename;
		Rigidbody body;
		ColliderType colType;
		Model modelo;
	};

	vector<Object> objects;
	vector<string> filenames = { "luigi.png", "bricks.jpg", "boo.png", "mushroom.png" };

	float time;
	float prevTime;
	float deltaTime;
	glm::vec3 deltaV;
	glm::vec3 deltaV2;

	Camera cameo;

	GLFWwindow* GLFWwindowPtr;

	ShaderManager sm;

public:
	Engine();
	~Engine();

	bool init();
	bool bufferModels();
	bool gameLoop();
	bool collides(const Object & a, const Object & b);
	bool useShaders();
};
