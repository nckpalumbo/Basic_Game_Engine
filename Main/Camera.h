#pragma once

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtx/euler_angles.hpp>
#include <GLFW/glfw3.h>

class Camera
{
public:
	Camera();
	~Camera();

	void updateCamera(float deltaTime);
	glm::mat4 getMat();

	struct rigidBody {
		glm::vec3 velocity, force;
		int mass;
	};

	glm::vec3 camRot;
	rigidBody body;
	glm::vec3 camLoc;

private:
	float zoom;
	int width;
	int height;

	float zNear;
	float zFar;

	glm::mat4 lookAtMat;
	glm::mat4 perspMat;
};