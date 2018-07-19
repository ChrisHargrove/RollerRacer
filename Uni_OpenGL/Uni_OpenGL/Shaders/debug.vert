#version 330 core

in vec3 aPos;
in vec3 aColor;

out vec3 fColor;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
	fColor = aColor;
	gl_Position = projection * view * model * vec4(aPos, 1.0f);
}