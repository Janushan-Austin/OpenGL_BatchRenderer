#pragma once
#include "Camera.h"

class FlyingCamera :
	public Camera
{
public:
	FlyingCamera(glm::vec3 pos = glm::vec3(0.0f), unsigned int w = 800, unsigned int h = 600, float fov = 90.0f, float y = 0.0f, float p = 0.0f, float r = 0.0f);
	~FlyingCamera();

	// Inherited via Camera
	virtual void ProcessMouseMovement(float xOffset, float yOffset) override;
	virtual void ProcessKeyboardInput(MovementDirection direction, float deltaTime) override;
	virtual void ProcessMouseScroll(float yOffset) override;


protected:

	virtual void ApplyPitch(float pitchOffset);
	virtual void ApplyYaw(float pitchOffset);
	virtual void ApplyRoll(float pitchOffset);
};

