#include "InstanceRenderer.h"

#include "OpenGLUtils/Includes/OpenGLIncludeHeaders.h"

#include <iostream>

//static initialization section
std::unordered_map<MaterialID, InstanceRenderer::MaterialMapStruct> InstanceRenderer::MaterialMap{};
glm::mat4 InstanceRenderer::ProjectionMatrix{ 1.0f }, InstanceRenderer::ViewMatrix{ 1.0f };
uint32_t InstanceRenderer::InstancingVAO{ 0 }, InstanceRenderer::InstancingVBO{ 0 };
uint32_t InstanceRenderer::MaxInstances{ 1000000 };
bool InstanceRenderer::Initialized{ false };

bool InstanceRenderer::Init()
{
	MaterialMap.clear();

	glGenVertexArrays(1, &InstancingVAO);
	glBindVertexArray(InstancingVAO);

	glGenBuffers(1, &InstancingVBO);
	glBindBuffer(GL_ARRAY_BUFFER, InstancingVBO);
	glBufferData(GL_ARRAY_BUFFER, MaxInstances * sizeof(glm::mat4), nullptr, GL_DYNAMIC_DRAW);

	std::size_t vec4Size = sizeof(glm::vec4);
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (void*)0);
	glEnableVertexAttribArray(4);
	glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (void*)(1 * vec4Size));
	glEnableVertexAttribArray(5);
	glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (void*)(2 * vec4Size));
	glEnableVertexAttribArray(6);
	glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (void*)(3 * vec4Size));

	glVertexAttribDivisor(3, 1);
	glVertexAttribDivisor(4, 1);
	glVertexAttribDivisor(5, 1);
	glVertexAttribDivisor(6, 1);

	Initialized = true;

	return Initialized;
}

void InstanceRenderer::BeginScene(const glm::mat4 projectionMatrix, const Camera& camera)
{
	//MaterialMap.clear();
	ProjectionMatrix = projectionMatrix;
	ViewMatrix = camera.GenerateViewMatrix();
}

void InstanceRenderer::Draw(Mesh& mesh, glm::mat4& transform)
{
	MeshID meshID = mesh.GetMeshID();
	Material* material = mesh.GetMaterial();
	MaterialID materialID = material->GetMaterialID();
	std::vector<glm::mat4> transforms{ transform };
	PutInRenderingBucket(material, materialID, &mesh, meshID, transforms);
}

void InstanceRenderer::Draw(Mesh& mesh, std::vector<glm::mat4>& transforms)
{
	MeshID meshID = mesh.GetMeshID();
	Material* material = mesh.GetMaterial();
	MaterialID materialID = material->GetMaterialID();

	PutInRenderingBucket(material, materialID, &mesh, meshID, transforms);
}

void InstanceRenderer::Draw(std::vector<Mesh>& meshesToRender, std::vector<glm::mat4>& transforms)
{
	for (int i = 0; i < meshesToRender.size(); i++)
	{
		Mesh& mesh = meshesToRender[i];
		MeshID meshID = mesh.GetMeshID();
		Material* material = mesh.GetMaterial();
		MaterialID materialID = material->GetMaterialID();

		PutInRenderingBucket(material, materialID, &mesh, meshID, transforms);
	}
}

void InstanceRenderer::Draw(std::vector<Mesh>& meshesToRender, std::vector<std::vector<glm::mat4>>& transforms)
{
	for (int i = 0; i < transforms.size(); i++)
	{
		Draw(meshesToRender, transforms[i]);
	}
}

void InstanceRenderer::EndScene()
{
	Flush();
}

void InstanceRenderer::Flush()
{
	static bool firstFlush = true;
	uint32_t maxSizeofBuffer = sizeof(glm::mat4) * MaxInstances;
	glBindVertexArray(InstancingVAO);

	for (auto& materialMapPair : MaterialMap)
	{
		Material* material = materialMapPair.second.material;
		std::unordered_map<MeshID, MeshMapStruct>& meshMap = materialMapPair.second.meshMap;
		material->AddUniform("projection", UniformTypes::MAT4, ProjectionMatrix);
		material->AddUniform("view", UniformTypes::MAT4, ViewMatrix);

		for (auto& meshMapPair : meshMap)
		{
			Mesh* mesh = meshMapPair.second.mesh;
			glm::mat4* meshInstanceTransforms = meshMapPair.second.meshTransforms.data();
			int32_t totalInstanceCount = meshMapPair.second.instanceCount;
			uint32_t instanceOffset = 0;

			//currently only uploads textures to shader
			mesh->UploadUniformsToMaterial();
			material->UploadUniforms();

			glBindBuffer(GL_ARRAY_BUFFER, mesh->GetVBO());

			// vertex positions
			glEnableVertexAttribArray(0);
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
			// vertex normals
			glEnableVertexAttribArray(1);
			glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));
			// vertex texture coords
			glEnableVertexAttribArray(2);
			glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TexCoord));


			glBindBuffer(GL_ARRAY_BUFFER, InstancingVBO);

			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->GetEBO());

			uint32_t indicesCount = mesh->GetIndicesCount();

			while (totalInstanceCount > MaxInstances)
			{
				glBufferSubData(GL_ARRAY_BUFFER, 0, maxSizeofBuffer, &meshInstanceTransforms[instanceOffset]);
				glDrawElementsInstanced(GL_TRIANGLES, indicesCount, GL_UNSIGNED_INT, nullptr, (GLsizei)MaxInstances);

				totalInstanceCount -= MaxInstances;
				instanceOffset += MaxInstances;
			}

			if (totalInstanceCount > 0)
			{
				glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(glm::mat4) * totalInstanceCount, &meshInstanceTransforms[instanceOffset]);
				glDrawElementsInstanced(GL_TRIANGLES, indicesCount, GL_UNSIGNED_INT, nullptr, (GLsizei)totalInstanceCount);
			}

			meshMapPair.second.instanceCount = 0;
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
			glBindBuffer(GL_ARRAY_BUFFER, 0);
		}

	}

	glBindVertexArray(0);
}

void InstanceRenderer::PutInRenderingBucket(Material* material, MaterialID materialID, Mesh* mesh, MeshID meshID, std::vector<glm::mat4>& transforms)
{
	const auto& materialIterator = MaterialMap.find(materialID);

	MaterialMapStruct* materialMapstruct;

	if (materialIterator != MaterialMap.end())
	{
		materialMapstruct = &materialIterator->second;
	}
	else
	{
		MaterialMap[materialID] = { material, {} };

		materialMapstruct = &MaterialMap[materialID];
	}

	auto& meshMap = materialMapstruct->meshMap;
	const auto& meshIterator = meshMap.find(meshID);

	if (meshIterator != meshMap.end())
	{
		std::vector<glm::mat4>& meshTransforms = meshIterator->second.meshTransforms;
		uint32_t tempInstanceCount = meshIterator->second.instanceCount + transforms.size();
		if (tempInstanceCount > meshTransforms.capacity())
		{
			meshTransforms.reserve(meshTransforms.size() * 2 + transforms.size());
		}

		//meshTransforms.insert(meshTransforms.begin() + meshIterator->second.instanceCount, transforms.begin(), transforms.end());
		uint32_t offset = meshIterator->second.instanceCount;
		memcpy(&meshTransforms.data()[offset], transforms.data(), transforms.size() * sizeof(glm::mat4));
		;		/*for (int i = 0; i < transforms.size(); i++, offset++)
				{
					meshTransforms[offset] = transforms[i];
				}*/
		meshIterator->second.instanceCount = tempInstanceCount;
	}
	else
	{
		MeshMapStruct meshMapStruct{ mesh, transforms, transforms.size() };
		meshMapStruct.meshTransforms.reserve(transforms.size() + 50);
		meshMap[meshID] = meshMapStruct;
	}
}
