#include "ShaderManager.h"
#include <fstream>
#include <iostream>
#include <string>


GLuint ShaderManager::loadShader(const char * file, GLenum shaderType)
{
	std::ifstream inFileBinary(file, std::ios::binary);
	if (!inFileBinary.is_open()) //If unable to read
	{
		std::cout << "Unable to read." << std::endl;
		return 0;
	}

	//STEP A
	//Find length of file
	inFileBinary.seekg(0, std::ios::end);
	unsigned int length = (unsigned int)inFileBinary.tellg();
	inFileBinary.seekg(0, std::ios::beg);
	//Save that many chars on heap
	char* fileContents = new char[length + 1];
	//Read file into char array
	inFileBinary.read(fileContents, length);
	fileContents[length] = '\0';
	//Print char array
	//std::cout << fileContents;

	inFileBinary.close();

	//STEP B
	GLuint shader = glCreateShader(shaderType);
	glShaderSource(shader, 1, &fileContents, 0);
	glCompileShader(shader);

	//Delete char array
	delete[] fileContents;

	//STEP C
	GLint * testiv = new GLint;
	glGetShaderiv(shader, GL_COMPILE_STATUS, testiv);
	if (testiv != 0)
		return shader;

	//STEP D
	GLint logLength;
	glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &logLength);

	//STEP E
	GLchar* logArray = new GLchar[logLength];
	glGetShaderInfoLog(shader, logLength, 0, logArray);
	std::cout << "LOG: " << logArray;
	glDeleteShader;
	delete[] logArray;
	delete testiv;

	return 0;
}

ShaderManager::ShaderManager()
{

}

ShaderManager::~ShaderManager()
{
}

GLuint ShaderManager::getProgram() const
{
	return program;
}

bool ShaderManager::loadShaders(const char * vertexFile, const char * fragmentFile)
{
	//STEP A
	GLuint load = loadShader(vertexFile, GL_VERTEX_SHADER);
	if (load == 0)
		return false;

	//STEP B
	GLuint loadF = loadShader(fragmentFile, GL_FRAGMENT_SHADER);
	if (loadF == 0)
		return false;

	//STEP C
	program = glCreateProgram();
	glAttachShader(program, load);
	glAttachShader(program, loadF);
	glLinkProgram(program);

	//STEP D
	GLint* linked = new GLint;
	glGetProgramiv(program, GL_LINK_STATUS, linked);
	if (*linked == GL_TRUE)
		return true;

	//STEP E
	GLint logLength;
	glGetProgramiv(program, GL_INFO_LOG_LENGTH, &logLength);

	GLchar* logArray = new GLchar[logLength];
	glGetShaderInfoLog(program, logLength, 0, logArray);
	std::cout << "LOG: " << logArray;
	glDeleteProgram(program);
	delete[] logArray;
	delete linked;

	return false;
}
