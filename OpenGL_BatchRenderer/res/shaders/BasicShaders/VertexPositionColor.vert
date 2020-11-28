#version 330 core

layout(location = 0) in vec3 vPos;

out vec4 fragColor;

void main(){
	fragColor = gl_Position = vec4(vPos, 1.0);
}