#pragma once
#include <unordered_map>
#include <string>
#include <iostream>

#include "OpenGLUtils/UtilClasses/Shader/Shader.h"

typedef uint32_t MaterialID;

enum class UniformTypes
{
	NONE = 0,
	FLOAT, FLOAT2, FLOAT3, FLOAT4,
	INT, INT2, INT3, INT4,
	MAT4
};

int UniformTypeSize(UniformTypes type);

struct UniformProps
{
	std::string uniformName;
	int uniformLocation = 0;
	UniformTypes type = UniformTypes::NONE;
	int typeSize = 0;
	void* data = nullptr;

	bool requiresUpload = true;

	UniformProps(){}

	UniformProps(const UniformProps& other)
	{
		*this = other;
	}

	const UniformProps& operator = (const UniformProps& other)
	{
		uniformName = other.uniformName;
		uniformLocation = other.uniformLocation;
		type = other.type;
		typeSize = other.typeSize;
		requiresUpload = other.requiresUpload;
		if (typeSize > 0)
		{
			data = new char[typeSize];
			memcpy(data, other.data, typeSize);
		}

		return other;
	}

	~UniformProps()
	{
		if (data != nullptr)
		{
			delete [] data;
			data = nullptr;
		}
	}
};

class Material
{
public:

	Material();
	Material(Shader&);

	Shader* GetShader();
	void SetShader(Shader&);

	template<typename T>
	bool AddUniform(const std::string&, UniformTypes uniformType, const T& data);
	bool RemoveUniform(const std::string& uniformName);

	bool UploadUniforms();

	MaterialID GetMaterialID() const { return materialID; }
private:
	MaterialID materialID;
	static MaterialID NextMaterialID;
	std::unordered_map<std::string, UniformProps> uniforms;
	Shader* shader;
};

//TODO use variadic
template <typename T>
bool Material::AddUniform(const std::string& uniformName, UniformTypes uniformType, const T& data)
{
	bool uniformUpdated = false;
	auto itr = uniforms.find(uniformName);
	if (itr != uniforms.end())
	{
		if (itr->second.type == uniformType)
		{
			itr->second.requiresUpload = true;
			memcpy(itr->second.data, &data, itr->second.typeSize);
			/*char* dataPtr = (char*)&data;
			char* propsDataPtr = (char*)itr->second.data;
			for (int i = 0; i < itr->second.typeSize; i++, dataPtr++, propsDataPtr++)
			{
				*propsDataPtr = *dataPtr;
			}*/

			uniformUpdated = true;
		}
		else
		{
			std::cout << "Error Updating Material Uniform! " << uniformName << "uniform type mismatch\n";
		}
	}
	else if (shader != nullptr)
	{
		UniformProps props;
		props.uniformName = uniformName;
		props.uniformLocation = shader->GetUniformLocation(props.uniformName);
		props.type = uniformType;
		props.typeSize = UniformTypeSize(props.type);
		props.data = new char[props.typeSize];
		props.requiresUpload = true;

		memcpy(props.data, &data, props.typeSize);
		//char* dataPtr = (char*)&data;
		//char* propsDataPtr = (char*)props.data;

		//for (int i = 0; i < props.typeSize; i++, dataPtr++, propsDataPtr++)
		//{
		//	*propsDataPtr = *dataPtr;
		//}

		if (props.uniformLocation >= 0)
		{
			uniforms[props.uniformName] = props;
			uniformUpdated = true;
		}
	}
	else
	{
		std::cout << "No shader bound to Material! Cannot upload uniform data\n";
	}

	return uniformUpdated;
}