#version 330 core

in vec2 fTexCoord;
in vec4 fCol;

out vec4 FragCol;

uniform sampler2D texture1;
uniform sampler2D texture2;

void main(){
	FragCol = mix(texture(texture1, fTexCoord), texture(texture2,fTexCoord),0.2);
}