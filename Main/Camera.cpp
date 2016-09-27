#include "Camera.h"

Camera::Camera() //Camera
{
	camLoc = { 0, 0, 2 };
	camRot = { 0, 0, 0 };
	zoom = 1.5f;
	width = 800;
	height = 600;
	zNear = .01f;
	zFar = 1000.0f;

	body = rigidBody({ {0,0,0}, {0,0,0}, 0 }); //Give the camera a body to move
}

Camera::~Camera() {}

void Camera::updateCamera(float deltaTime) //Update the camera
{
	camLoc += (body.velocity * deltaTime);

	glm::mat3 rotMat = (glm::mat3)glm::yawPitchRoll(camRot.y, camRot.x, camRot.z);

	glm::vec3 eye = camLoc;
	glm::vec3 center = eye + rotMat * glm::vec3(0, 0, -1);
	glm::vec3 up = rotMat * glm::vec3(0, 1, 0);

	lookAtMat = glm::lookAt(eye, center, up);

	float fovy = 3.14159f * .4f / zoom;
	float aspect = (float)width / (float)height;

	perspMat = glm::perspective(fovy, aspect, zNear, zFar);
}

glm::mat4 Camera::getMat()
{
	return perspMat * lookAtMat;
}