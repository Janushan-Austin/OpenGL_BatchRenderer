#include "Material.h"

#include <iostream>
#include "OpenGLUtils/Includes/OpenGLIncludeHeaders.h"
#include "Shader.h"

MaterialID Material::NextMaterialID{ 0 };

int UniformTypeSize(UniformTypes type)
{
	switch (type)
	{
	case UniformTypes::FLOAT:
		return sizeof(float);
	case UniformTypes::FLOAT2:
		return sizeof(float) * 2;
	case UniformTypes::FLOAT3:
		return sizeof(float) * 3;
	case UniformTypes::FLOAT4:
		return sizeof(float) * 4;

	case UniformTypes::INT:
		return sizeof(int);
	case UniformTypes::INT2:
		return sizeof(int);
	case UniformTypes::INT3:
		return sizeof(int);
	case UniformTypes::INT4:
		return sizeof(int);

	case UniformTypes::MAT4:
		return sizeof(glm::mat4);
	default:
		return 0;
	}
}

Material::Material()
{
	materialID = NextMaterialID;
	NextMaterialID++;
	shader = nullptr;
}

Material::Material(Shader& _shader)
{
	shader = nullptr;
	materialID = NextMaterialID;
	NextMaterialID++;
	SetShader(_shader);
}

Shader* Material::GetShader()
{
	return shader;
}

void Material::SetShader(Shader& _shader)
{
	if (shader != &_shader)
	{
		shader = &_shader;
		uniforms.clear();
		uniforms.reserve(10);
	}
}

bool Material::RemoveUniform(const std::string& uniformName)
{
	bool uniformRemoved = false;

	if (shader != nullptr)
	{
		auto itr = uniforms.find(uniformName);
		if (itr != uniforms.end())
		{
			uniformRemoved = true;
			uniforms.erase(itr);
		}
	}
	else
	{
		std::cout << "No shader bound to Material! Cannot remove uniform data\n";
	}

	return uniformRemoved;
}

bool Material::UploadUniforms()
{
	bool uploadSuccesful = false;

	if (shader != nullptr)
	{
		shader->Bind();
		uploadSuccesful = true;
		static glm::mat4 matrix4x4;
		static float f1, f2, f3, f4;
		static int i1, i2, i3, i4;

		for (auto itr = uniforms.begin(); itr != uniforms.end(); itr++)
		{
			if (itr->second.requiresUpload == true)
			{
				itr->second.requiresUpload = false;
				switch (itr->second.type)
				{
				case UniformTypes::FLOAT:
					f1 = *(float*)itr->second.data;
					shader->SetFloat1(itr->second.uniformName, f1);
					break;
				case UniformTypes::FLOAT2:
					f1 = *(float*)itr->second.data;
					f2 = *(float*)((float*)itr->second.data + 1);
					shader->SetFloat2(itr->second.uniformName, f1, f2);
					break;
				case UniformTypes::FLOAT3:
					f1 = *(float*)itr->second.data;
					f2 = *(float*)((float*)itr->second.data + 1);
					f3 = *(float*)((float*)itr->second.data + 2);
					shader->SetFloat3(itr->second.uniformName, f1, f2, f3);
					break;
				case UniformTypes::FLOAT4:
					f1 = *(float*)itr->second.data;
					f2 = *(float*)((float*)itr->second.data + 1);
					f3 = *(float*)((float*)itr->second.data + 2);
					f4 = *(float*)((float*)itr->second.data + 3);
					shader->SetFloat4(itr->second.uniformName, f1, f2, f3, f4);
					break;

				case UniformTypes::INT:
					i1 = *(int*)itr->second.data;
					shader->SetInt1(itr->second.uniformName, i1);
					break;
				case UniformTypes::INT2:
					i1 = *(int*)itr->second.data;
					i2 = *(int*)((int*)itr->second.data + 1);
					shader->SetInt2(itr->second.uniformName, i1, i2);
					break;
				case UniformTypes::INT3:
					i1 = *(int*)itr->second.data;
					i2 = *(int*)((int*)itr->second.data + 1);
					i3 = *(int*)((int*)itr->second.data + 2);
					shader->SetInt3(itr->second.uniformName, i1, i2, i3);
					break;
				case UniformTypes::INT4:
					i1 = *(int*)itr->second.data;
					i2 = *(int*)((int*)itr->second.data + 1);
					i3 = *(int*)((int*)itr->second.data + 2);
					i4 = *(int*)((int*)itr->second.data + 3);
					shader->SetInt4(itr->second.uniformName, i1, i2, i3, i4);
					break;

				case UniformTypes::MAT4:
					float* data = (float*)itr->second.data;
					for (int i = 0; i < 4; i++)
					{
						f1 = *data;
						f2 = *(data + 1);
						f3 = *(data + 2);
						f4 = *(data + 3);
						glm::vec4 temp = { f1,f2,f3,f4 };
						data += 4;
						matrix4x4[i] = temp;
					}
					//matrix4x4 = *(glm::mat4*)itr->second.data;
					shader->SetMat4(itr->second.uniformName, matrix4x4);
					break;
				}
			}
		}
	}

	return uploadSuccesful;
}
