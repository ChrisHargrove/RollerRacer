#version 330 core

in vec3 Normal;  
in vec3 FragPos;
in vec2 TexCoords;

out vec4 FragColor;
uniform sampler2D textureImage;
uniform vec3 color;
uniform bool RenderingText;
uniform bool RenderingDepth;

float near_plane = 1.0f;
float far_plane = 7.5f;

float LinearizeDepth(float depth)
{
    float z = depth * 2.0 - 1.0; // Back to NDC 
    return (2.0 * near_plane * far_plane) / (far_plane + near_plane - z * (far_plane - near_plane));	
}

void main()
{
	if(RenderingText){
		vec4 sampled = vec4(1.0, 1.0, 1.0, texture(textureImage, TexCoords).r);
		FragColor = vec4(color, 1.0) * sampled;
	}	
	else if (RenderingDepth){
		float depthValue = texture(textureImage, TexCoords).r;
		FragColor = vec4(vec3(LinearizeDepth(depthValue) / far_plane), 1.0); // perspective
		//FragColor = vec4(vec3(depthValue), 1.0);
	}
	else{
		FragColor = texture(textureImage, TexCoords) * vec4(color, 1.0f);
	}
}