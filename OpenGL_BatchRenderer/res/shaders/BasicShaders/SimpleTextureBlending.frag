#version 330 core

in vec2 fTexCoord;

out vec4 FragColor;

uniform sampler2D texture_diffuse1;
uniform sampler2D texture_diffuse2;

void main(){
	vec4 texColor = mix(texture(texture_diffuse1, fTexCoord), texture(texture_diffuse2, fTexCoord), 0.2);
	if(texColor.a < 0.1)
	{
		discard;
	}
	FragColor = texColor;
}