#version 330 core

layout(location =0) in vec3 vPos;
layout(location =1) in vec3 vCol;
layout(location =2) in vec2 vTexCoord;

out vec2 fTexCoord;
out vec4 fCol;

uniform mat4 transformationMat;

void main(){
	gl_Position = transformationMat* vec4(vPos, 1.0);

	fTexCoord = vTexCoord;
	fCol = vec4(vCol, 1.0);
}