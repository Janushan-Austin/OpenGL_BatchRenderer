#version 330 core

layout(location =0) in vec3 vPos;
layout(location =1) in vec3 vCol;
layout(location =2) in vec2 vTexCoord;
//locations 3 through 6 are used by instanceTransforms

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec2 fTexCoord;

void main(){

	gl_Position = projection * view * model * vec4(vPos, 1.0);

	fTexCoord = vTexCoord;
}