#pragma once
#include "Camera.h"
class FlyingFPSCamera :
	public Camera
{
public:
	FlyingFPSCamera(glm::vec3 pos = glm::vec3(0.0f), glm::vec3 worldUp_ = glm::vec3(0.0f, 1.0f, 0.0f), unsigned int w = 800, unsigned int h = 600, float fov = 90.0f, float y = 0.0f, float p = 0.0f);
	~FlyingFPSCamera();

	// Inherited via Camera
	virtual void ProcessMouseMovement(float xOffset, float yOffset) override;
	virtual void ProcessKeyboardInput(MovementDirection direction, float deltaTime) override;
	virtual void ProcessMouseScroll(float yOffset) override;

protected:
	glm::vec3 worldUp;
	virtual void UpdateCameraVectors();
	//virtual void ApplyPitch(float pitchOffset) override;
};

