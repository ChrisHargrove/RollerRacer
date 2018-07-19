#version 330 core

in vec3 aPos;

uniform mat4 model;
uniform mat4 depthMVP;

void main()
{
	gl_Position = depthMVP * (model * vec4(aPos, 1.0));
}