#pragma once

#include "OpenGLUtils/Includes/OpenGLIncludeHeaders.h"

struct TextureProps
{
	char const* TexturePath;
	bool FlipVertically;
	GLint Texture_Wrap_S_Prop;
	GLint Texture_Wrap_T_Prop;
	GLint Texture_Min_Filter_Prop;
	GLint Texture_Mag_Filter_Prop;
};

unsigned int loadTexture(char const *, bool = true);
unsigned int loadTexture(TextureProps);