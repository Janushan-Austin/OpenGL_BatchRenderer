#version 330 core

layout(location =0) in vec3 vPos;
layout(location =1) in vec3 vNormal;
layout(location =2) in vec2 vTexCoord;
//locations 3 through 6 are used by instanceTransforms
layout(location = 3) in mat4 instanceTransform;

uniform int useInstancing = 0;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec3 fNormal;
out vec3 fPos;
out vec2 fTexCoord;

void main(){

	if(useInstancing == 1)
	{
		vec4 fPos4Component = (instanceTransform * vec4(vPos, 1.0));
		gl_Position = projection * view * fPos4Component;
		fNormal = mat3(transpose(inverse(instanceTransform))) * vNormal;
		fPos = fPos4Component.xyz;
	}
	else
	{
		vec4 fPos4Component = (model * vec4(vPos, 1.0));
		gl_Position = projection * view *  fPos4Component;
		fNormal = mat3(transpose(inverse(model))) * vNormal;
		fPos = fPos4Component.xyz;
	}

	fTexCoord = vTexCoord;
}