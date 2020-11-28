#include "FlyingFPSCamera.h"



FlyingFPSCamera::FlyingFPSCamera(glm::vec3 pos, glm::vec3 worldUp_, unsigned int w, unsigned int h, float fov, float y, float p) :Camera(pos, w, h, fov, y, p)
{
	worldUp = worldUp_;
	UpdateCameraVectors();
}


FlyingFPSCamera::~FlyingFPSCamera()
{
}

void FlyingFPSCamera::ProcessMouseMovement(float xOffset, float yOffset)
{
	xOffset *= MouseSensitivity, yOffset *= MouseSensitivity;

	pitch -= yOffset;
	yaw += xOffset;

	if (pitch > 89.0f) {
		pitch = 89.0f;
	}
	else if (pitch < -89.0f) {
		pitch = -89.0f;
	}

	UpdateCameraVectors();
}

void FlyingFPSCamera::ProcessKeyboardInput(MovementDirection direction, float deltaTime)
{

	if (direction == MovementDirection::FORWARD) {
		cameraPos += cameraFront * CameraSpeed * deltaTime;
	}
	else if (direction == MovementDirection::BACKWARD) {
		cameraPos -= cameraFront * CameraSpeed * deltaTime;
	}
	else if (direction == MovementDirection::LEFT) {
		cameraPos -= cameraRight * CameraSpeed * deltaTime;
	}
	else if (direction == MovementDirection::RIGHT) {
		cameraPos += cameraRight * CameraSpeed * deltaTime;
	}
	else if (direction == MovementDirection::UP) {
		cameraPos += cameraUp * CameraSpeed * deltaTime;
	}
	else if (direction == MovementDirection::DOWN) {
		cameraPos -= cameraUp * CameraSpeed * deltaTime;
	}
}

void FlyingFPSCamera::ProcessMouseScroll(float yOffset)
{
	if (fov >= 1.0f && fov <= 120.0f) {
		fov -= yOffset;
	}

	if (fov < 1.0f) {
		fov = 1.0f;
	}
	else if (fov > 120.0f) {
		fov = 120.0f;
	}
}

void FlyingFPSCamera::UpdateCameraVectors()
{
	glm::vec3 front(0.0f);
	front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	front.y = sin(glm::radians(pitch));
	front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));

	cameraFront = glm::normalize(front);
	cameraRight = glm::normalize(glm::cross(cameraFront, worldUp));
	cameraUp = glm::normalize(glm::cross(cameraRight, cameraFront));
}