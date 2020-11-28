#include "FlyingCamera.h"



FlyingCamera::FlyingCamera(glm::vec3 pos, unsigned int w, unsigned int h, float fov, float y, float p, float r) :Camera(pos, w, h, fov, y, p, r)
{
	ApplyPitch(0);
	ApplyYaw(0);
	ApplyRoll(0);
}


FlyingCamera::~FlyingCamera()
{
}

void FlyingCamera::ProcessMouseMovement(float xOffset, float yOffset)
{
	xOffset *= MouseSensitivity, yOffset *= MouseSensitivity;

	ApplyYaw(xOffset);
	ApplyPitch(yOffset);
}

void FlyingCamera::ProcessKeyboardInput(MovementDirection direction, float deltaTime)
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
	else if (direction == MovementDirection::ROLL_LEFT) {
		ApplyRoll(CameraSpeed * deltaTime * 100 );
	}
	else if (direction == MovementDirection::ROLL_RIGHT) {
		ApplyRoll(-CameraSpeed * deltaTime * 100);
	}
}

void FlyingCamera::ProcessMouseScroll(float yOffset)
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

void FlyingCamera::ApplyPitch(float pitchOffset)
{
	pitch -= pitchOffset;
	cameraFront = glm::normalize((cameraFront * cosf(glm::radians(-pitchOffset))) + (cameraUp * sinf(glm::radians(-pitchOffset))));

	cameraUp = glm::cross(cameraRight, cameraFront);
}

void FlyingCamera::ApplyYaw(float yawOffset)
{
	yaw += yawOffset;
	cameraFront = glm::normalize((cameraFront * cosf(glm::radians(yawOffset))) + (cameraRight * sinf(glm::radians(yawOffset))));

	cameraRight = glm::cross(cameraFront, cameraUp);
}

void FlyingCamera::ApplyRoll(float rollOffset)
{
	roll += rollOffset;
	cameraRight = glm::normalize((cameraRight * cosf(glm::radians(rollOffset))) + (cameraUp * sinf(glm::radians(rollOffset))));

	cameraUp = glm::cross(cameraRight, cameraFront);
}
