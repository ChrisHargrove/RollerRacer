#include "TextRenderer.h"
#include "ShaderManager.h"



TextRenderer::TextRenderer(std::string fontPath)
{
	FT_Library _Freetype;
	if (FT_Init_FreeType(&_Freetype)) {
		return; //Error occured
	}
	FT_Face _Face;
	if (FT_New_Face(_Freetype, fontPath.c_str(), 0, &_Face)) {
		return; //Error occured
	}
	FT_Set_Pixel_Sizes(_Face, 0, 48);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	for (GLubyte c = 0; c < 128; c++)
	{
		// Load character glyph 
		if (FT_Load_Char(_Face, c, FT_LOAD_RENDER))
		{
			continue; //failed to load a glyph
		}
		// Generate texture
		GLuint texture;
		glGenTextures(1, &texture);
		glBindTexture(GL_TEXTURE_2D, texture);
		glTexImage2D(
			GL_TEXTURE_2D,
			0,
			GL_RED,
			_Face->glyph->bitmap.width,
			_Face->glyph->bitmap.rows,
			0,
			GL_RED,
			GL_UNSIGNED_BYTE,
			_Face->glyph->bitmap.buffer
		);
		// Set texture options
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		// Now store character for later use
		Character character = {
			texture,
			glm::ivec2(_Face->glyph->bitmap.width, _Face->glyph->bitmap.rows),
			glm::ivec2(_Face->glyph->bitmap_left, _Face->glyph->bitmap_top),
			(GLuint)_Face->glyph->advance.x
		};
		_Characters.insert(std::pair<GLchar, Character>(c, character));
	}
	glBindTexture(GL_TEXTURE_2D, 0);
	// Destroy FreeType once we're finished
	FT_Done_Face(_Face);
	FT_Done_FreeType(_Freetype);

	_VertexArray.Create(VAO);
	_VertexBuffer.Create(VBO);

	_VertexArray.Bind();
	_VertexBuffer.Fill(sizeof(GLfloat) * 6 * 4, NULL, DYNAMIC);
	_VertexBuffer.AddAttribPointer(ShaderManager::Instance()->GetShader("texture")->GetID(), "aPos", 3, VT_FLOAT, 5 * sizeof(float));
	_VertexBuffer.AddAttribPointer(ShaderManager::Instance()->GetShader("texture")->GetID(), "aTexCoords", 3, VT_FLOAT, 5 * sizeof(float), 3 * sizeof(float));

}


TextRenderer::~TextRenderer()
{
}

void TextRenderer::RenderText(std::string text, glm::vec2 pos, float scale, bool center, glm::vec3 color)
{
	ShaderManager::Instance()->GetShader("texture")->SetVec3("color", color);
	ShaderManager::Instance()->GetShader("texture")->SetInt("textureImage", 0);
	ShaderManager::Instance()->GetShader("texture")->SetBool("RenderingText", true);
	_VertexArray.Bind();
	glActiveTexture(GL_TEXTURE0);
	// Iterate through all characters

	std::string::const_iterator c;
	glm::vec2 offset = glm::vec2(0.0f, 0.0f);
	if (center) { //if text is centered calculate the size of the string.
		for (c = text.begin(); c != text.end(); c++) {
			offset.x += _Characters[*c].Size.x * scale + _Characters[*c].Bearing.x * scale;
		}
	}
	for (c = text.begin(); c != text.end(); c++)
	{
		Character ch = _Characters[*c];

		//get positions for each quad using position and offset if centered.
		GLfloat xpos = (pos.x - offset.x * 0.5f) + ch.Bearing.x * scale;
		GLfloat ypos = (pos.y - offset.y * 0.5f) - (ch.Size.y - ch.Bearing.y) * scale;

		GLfloat w = ch.Size.x * scale;
		GLfloat h = ch.Size.y * scale;
		// Update VBO for each character
		GLfloat vertices[6][5] = {
		{ xpos,     ypos + h,0.0,   0.0, 0.0 },
		{ xpos,     ypos,0.0,       0.0, 1.0 },
		{ xpos + w, ypos,0.0,       1.0, 1.0 },

		{ xpos,     ypos + h,0.0,   0.0, 0.0 },
		{ xpos + w, ypos,0.0,       1.0, 1.0 },
		{ xpos + w, ypos + h,0.0,   1.0, 0.0 }
		};
		// Render glyph texture over quad
		glBindTexture(GL_TEXTURE_2D, ch.TextureID);
		// Update content of VBO memory
		_VertexBuffer.Fill(sizeof(vertices), vertices, DYNAMIC);
		// Render quad
		glDrawArrays(GL_TRIANGLES, 0, 6);
		// Now advance cursors for next glyph (note that advance is number of 1/64 pixels)
		pos.x += (ch.Advance >> 6) * scale; // Bitshift by 6 to get value in pixels (2^6 = 64)
	}
	glBindTexture(GL_TEXTURE_2D, 0);
}
