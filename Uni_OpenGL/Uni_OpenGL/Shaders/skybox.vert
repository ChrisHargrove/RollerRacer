#version 330 core

in vec3 aPos;

out vec3 fColor;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
	fColor = aPos;

    mat4 camFix = mat4(mat3(view));

	gl_Position = (projection * camFix * model * vec4(aPos, 1.0f)).xyww;
}
