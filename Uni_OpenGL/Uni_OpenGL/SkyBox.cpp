#include "SkyBox.h"



SkyBox::SkyBox(std::string ShaderName, std::string FileName, std::string cubeMapName)
{
    _Shader = ShaderName;
    _CubeMapTexture = new CubeMap(FileName, cubeMapName);

    float vertices[] = {
        // positions          
        -1.0f,  1.0f, -1.0f,
        -1.0f, -1.0f, -1.0f,
        1.0f, -1.0f, -1.0f,
        1.0f, -1.0f, -1.0f,
        1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,

        -1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,

        1.0f, -1.0f, -1.0f,
        1.0f, -1.0f,  1.0f,
        1.0f,  1.0f,  1.0f,
        1.0f,  1.0f,  1.0f,
        1.0f,  1.0f, -1.0f,
        1.0f, -1.0f, -1.0f,

        -1.0f, -1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
        1.0f,  1.0f,  1.0f,
        1.0f,  1.0f,  1.0f,
        1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,

        -1.0f,  1.0f, -1.0f,
        1.0f,  1.0f, -1.0f,
        1.0f,  1.0f,  1.0f,
        1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f, -1.0f,

        -1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
        1.0f, -1.0f, -1.0f,
        1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
        1.0f, -1.0f,  1.0f
    };

    _VertexArray.Create(VAO);
    _VertexBuffer.Create(VBO);
    _VertexArray.Bind();
    _VertexBuffer.Fill(sizeof(vertices), vertices, STATIC);
    _VertexBuffer.AddAttribPointer(ShaderManager::Instance()->GetShader(_Shader)->GetID(), "aPos", 3, VT_FLOAT, 3 * sizeof(float));

    _DrawCount = 36;
    _VertexArray.Unbind();

}

SkyBox::~SkyBox()
{
    _VertexArray.Destroy();
    _VertexBuffer.Destroy();
}

void SkyBox::Draw()
{
    glDepthFunc(GL_LEQUAL);

    ShaderManager::Instance()->GetShader(_Shader)->SetInt("skybox", 0);
    _VertexArray.Bind();
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, _CubeMapTexture->GetID());
    glDrawArrays(GL_TRIANGLES, 0, 36);
    _VertexArray.Unbind();
    
    glDepthFunc(GL_LESS);
}
