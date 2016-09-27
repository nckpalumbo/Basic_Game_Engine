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
#include <fstream>
#include <sstream>

#include <vector>
using std::vector;

#include <glm/gtx/euler_angles.hpp>
#include <cmath>

struct Vertex
{
	glm::vec3 location;
	glm::vec2 uv;
	glm::vec3 normal;
};

struct VertInd
{
	unsigned int locInd;
	unsigned int uvInd;
	unsigned int normInd;
};

class Model
{

public:
	Model();
	~Model();
	bool buffer(std::string objFile);
	void render();

private:
	unsigned int vertCount;
	GLuint vertArr;
	GLuint vertBuf;
	vector<Vertex> vertBuffData;

};