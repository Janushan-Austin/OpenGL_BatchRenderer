#include "Mesh.h"
#include "OpenGLUtils/UtilClasses/Shader/Shader.h"
#include "OpenGLUtils/UtilClasses/Shader/Material.h"

MeshID Mesh::NextMeshID{ 0 };

Mesh::Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures)
{
	meshID = NextMeshID;
	NextMeshID++;
	Vertices = vertices;
	Indices = indices;
	Textures = textures;

	SetUniformContainer("");

	SetupMesh();
}

void Mesh::Draw(Shader& shader, const std::string& uniformName)
{
	unsigned int diffuseTextrueNumber = 1;
	unsigned int specularTextrueNumber = 1;
	unsigned int normalTextrueNumber = 1;
	unsigned int heightTextrueNumber = 1;

	std::string textureName;
	std::string textureNumber;

	std::string uniformComplete = uniformName;
	if (uniformName != "") {
		uniformComplete += ".";
	}
	for (unsigned int i = 0; i < Textures.size(); i++) {
		glActiveTexture(GL_TEXTURE0 + i);

		if (Textures[i].type == TextureType::DIFFUSE) {
			textureName = "texture_diffuse";
			textureNumber = std::to_string(diffuseTextrueNumber);
			diffuseTextrueNumber++;
		}
		else if (Textures[i].type == TextureType::SPECULAR) {
			textureName = "texture_specular";
			textureNumber = std::to_string(specularTextrueNumber);
			specularTextrueNumber++;
		}
		else if (Textures[i].type == TextureType::NORMAL) {
			textureName = "texture_normal";
			textureNumber = std::to_string(normalTextrueNumber);
			normalTextrueNumber++;
		}
		else if (Textures[i].type == TextureType::HEIGHT) {
			textureName = "texture_height";
			textureNumber = std::to_string(heightTextrueNumber);
			heightTextrueNumber++;
		}

		shader.SetInt1(uniformComplete + textureName + textureNumber, i);
		glBindTexture(GL_TEXTURE_2D, Textures[i].id);
	}
	glActiveTexture(GL_TEXTURE0);


	//draw the mesh
	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, (GLsizei)Indices.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

void Mesh::Draw()
{
	if (MeshMaterial == nullptr)
	{
		return;
	}
	UploadTexturesToMaterial();
	MeshMaterial->UploadUniforms();

	//draw the mesh
	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, (GLsizei)Indices.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

void Mesh::UploadTexturesToMaterial(const std::string& uniformName)
{
	if (MeshMaterial != nullptr)
	{
		if (uniformName != UniformContainerName)
		{
			SetUniformContainer(uniformName);
		}
		UploadTexturesToMaterial();
	}
}

void Mesh::UploadTexturesToMaterial()
{
	if (MeshMaterial != nullptr)
	{
		unsigned int diffuseTextrueNumber = 1;
		unsigned int specularTextrueNumber = 1;
		unsigned int normalTextrueNumber = 1;
		unsigned int heightTextrueNumber = 1;

		std::string textureName;
		std::string textureNumber;

		bool diffuse_used = false;
		bool specular_used = false;
		bool normal_used = false;

		std::string uniformComplete = UniformContainerName;
		if (uniformComplete != "") {
			uniformComplete += ".";
		}
		for (unsigned int i = 0; i < Textures.size(); i++) {
			glActiveTexture(GL_TEXTURE0 + i);

			if (Textures[i].type == TextureType::DIFFUSE) {
				textureName = "texture_diffuse";
				textureNumber = std::to_string(diffuseTextrueNumber);
				diffuseTextrueNumber++;
			}
			else if (Textures[i].type == TextureType::SPECULAR) {
				textureName = "texture_specular";
				textureNumber = std::to_string(specularTextrueNumber);
				specularTextrueNumber++;
			}
			else if (Textures[i].type == TextureType::NORMAL) {
				textureName = "texture_normal";
				textureNumber = std::to_string(normalTextrueNumber);
				normalTextrueNumber++;
			}
			else if (Textures[i].type == TextureType::HEIGHT) {
				textureName = "texture_height";
				textureNumber = std::to_string(heightTextrueNumber);
				heightTextrueNumber++;
			}

			//MeshMaterial->GetShader()->SetInt1(uniformComplete + textureName + textureNumber, i);
			if (MeshMaterial->AddUniform((uniformComplete + textureName + textureNumber).c_str(), UniformTypes::INT, &i))
			{
				glBindTexture(GL_TEXTURE_2D, Textures[i].id);
			}
		}
		glActiveTexture(GL_TEXTURE0);
	}
}

void Mesh::SetMaterial(Material& material)
{
	MeshMaterial = &material;
}

void Mesh::SetUniformContainer(const std::string& uniformContainerName)
{
	UniformContainerName = uniformContainerName;
}

void Mesh::SetupMesh()
{
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	glBufferData(GL_ARRAY_BUFFER, Vertices.size() * sizeof(Vertex), &Vertices[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, Indices.size() * sizeof(unsigned int),
		&Indices[0], GL_STATIC_DRAW);

	// vertex positions
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
	// vertex normals
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));
	// vertex texture coords
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TexCoord));

	glBindVertexArray(0);
}
