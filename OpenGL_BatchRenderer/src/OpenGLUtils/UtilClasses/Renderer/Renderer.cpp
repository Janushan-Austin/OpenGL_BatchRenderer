#include "Renderer.h"

glm::mat4 Renderer::ProjectionMatrix{ 1.0f }, Renderer::ViewMatrix{ 1.0f };
bool Renderer::Initialized{ false };

bool Renderer::Init()
{
	Initialized = true;

	return Initialized;
}

void Renderer::BeginScene(const glm::mat4 projectionMatrix, const Camera& camera)
{
	ProjectionMatrix = projectionMatrix;
	ViewMatrix = camera.GenerateViewMatrix();
}

void Renderer::Draw(Mesh& mesh, glm::mat4& transform)
{
	Material* material = mesh.GetMaterial();
	material->AddUniform("projection", UniformTypes::MAT4, ProjectionMatrix);
	material->AddUniform("view", UniformTypes::MAT4, ViewMatrix);
	material->AddUniform("model", UniformTypes::MAT4, transform);
	mesh.Draw();
}

void Renderer::Draw(Mesh& mesh, std::vector<glm::mat4>& transforms)
{
	for (auto& transform : transforms)
	{
		Draw(mesh, transform);
	}
}

void Renderer::Draw(std::vector<Mesh>& meshesToRender, std::vector<glm::mat4>& transforms)
{
	for (auto& mesh : meshesToRender)
	{
		Draw(mesh, transforms);
	}
}

void Renderer::EndScene()
{
}
