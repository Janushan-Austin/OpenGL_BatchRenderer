#pragma once
#include <iostream>
#include "stb_Image_helpers.h"
#include "OpenGLUtils/Includes/stb_image_include.h"

unsigned int loadTexture(char const * path, bool flip) {
	TextureProps props{path, flip, GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR};
	
	return loadTexture(props);

	//stbi_set_flip_vertically_on_load(flip);

	//unsigned int textureID;
	//glGenTextures(1, &textureID);

	//int width, height, nrComponents;
	//unsigned char *data = stbi_load(path, &width, &height, &nrComponents, 0);
	//if (data)
	//{
	//	GLenum format;
	//	if (nrComponents == 1)
	//		format = GL_RED;
	//	else if (nrComponents == 3)
	//		format = GL_RGB;
	//	else if (nrComponents == 4)
	//		format = GL_RGBA;

	//	glBindTexture(GL_TEXTURE_2D, textureID);
	//	glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
	//	glGenerateMipmap(GL_TEXTURE_2D);


	//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	//	stbi_image_free(data);
	//}
	//else
	//{
	//	std::cout << "Texture failed to load at path: " << path << std::endl;
	//	stbi_image_free(data);
	//}

	//return textureID;
}

unsigned int loadTexture(TextureProps props)
{
	stbi_set_flip_vertically_on_load(props.FlipVertically);

	unsigned int textureID;
	glGenTextures(1, &textureID);

	int width, height, nrComponents;
	unsigned char* data = stbi_load(props.TexturePath, &width, &height, &nrComponents, 0);
	if (data)
	{
		GLenum format;
		if (nrComponents == 1)
			format = GL_RED;
		else if (nrComponents == 3)
			format = GL_RGB;
		else if (nrComponents == 4)
			format = GL_RGBA;

		glBindTexture(GL_TEXTURE_2D, textureID);
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);


		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, props.Texture_Wrap_S_Prop);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, props.Texture_Wrap_T_Prop);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, props.Texture_Min_Filter_Prop);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, props.Texture_Mag_Filter_Prop);

		stbi_image_free(data);
	}
	else
	{
		std::cout << "Texture failed to load at path: " << props.TexturePath << std::endl;
		stbi_image_free(data);
	}

	return textureID;
}