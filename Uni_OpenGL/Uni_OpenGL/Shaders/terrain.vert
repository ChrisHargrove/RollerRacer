#version 330 core

in vec3 aPos;
in vec3 aNormal;
in vec2 aTexCoords;
in vec2 aTexData; 

out vec3 FragPos;
out vec3 Normal;
out vec2 TexCoords;

out vec2 TexData;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
	FragPos = vec3(model* vec4(aPos, 1.0));
    
	Normal = aNormal;
    TexCoords = aTexCoords;
	TexData = aTexData;
	
    gl_Position = projection * view * vec4(FragPos, 1.0);
} 