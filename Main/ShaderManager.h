#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <FreeImage.h>

#include <vector>
using std::vector;

class ShaderManager
{
private:
	GLuint program;
	GLuint loadShader(const char* file, GLenum shaderType);

public:
	ShaderManager();
	~ShaderManager();

	GLuint getProgram() const;
	bool loadShaders(const char* vertexFile, const char* fragmentFile);

};

