#version 330 core

layout(location = 0) in vec3 vPos;
layout(location = 1) in vec3 vCol;

out vec4 fragColor;

uniform float xOffset;
void main(){
	gl_Position = vec4(vPos.x + xOffset, vPos.yz, 1.0);
	fragColor = vec4(vCol, 1.0);
}