#pragma once

#include <GLEW/glew.h>
#include <GLM/glm.hpp>

#include <ft2build.h>
#include FT_FREETYPE_H

#include <map>
#include <string>

#include "Buffer.h"

struct Character {
	GLuint TextureID;   // ID handle of the glyph texture
	glm::ivec2 Size;    // Size of glyph
	glm::ivec2 Bearing;  // Offset from baseline to left/top of glyph
	GLuint Advance;    // Horizontal offset to advance to next glyph
};

class TextRenderer
{
public:
	TextRenderer(std::string fontPath);
	~TextRenderer();

	void RenderText(std::string text, glm::vec2 pos, float scale, bool center = false, glm::vec3 color = glm::vec3(1.0, 1.0, 1.0));

private:
	std::map<GLchar, Character> _Characters;

	Buffer _VertexArray;
	Buffer _VertexBuffer;
};

