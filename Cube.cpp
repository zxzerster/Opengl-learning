#include "Cube.h"
#include "shader/shader_util.h"
#include "textures/stb_image.h"

#include <iostream>

Cube::Cube() {
    TexMap[GL_TEXTURE0] = 0;
    TexMap[GL_TEXTURE1] = 1;
    TexMap[GL_TEXTURE2] = 2;
    TexMap[GL_TEXTURE3] = 3;
    TexMap[GL_TEXTURE4] = 4;
    TexMap[GL_TEXTURE5] = 5;

    glGenVertexArrays(1, &VAO);
}

Cube::~Cube() {

}

void Cube::loadVertexAttribute(unsigned int location, GLfloat* vertices, unsigned int size, unsigned int components)
{
    if (!vertices || size < 0) return;
    if (size % components != 0) return;

    GLuint EBO;
    glGenBuffers(1, &EBO);
    EBOs.push_back(EBO);

    glBindVertexArray(VAO);
    {
        glBindBuffer(GL_ARRAY_BUFFER, EBOs[EBOs.size() - 1]);
        glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);
        glVertexAttribPointer(location, components, GL_FLOAT, GL_FALSE, components * sizeof(float), (void*)0);
        glEnableVertexAttribArray(location);
    }
    glBindVertexArray(0);
}

void Cube::loadShaders(const std::string& vertexPath, const std::string& fragmentPath) {
    if (vertexPath.empty() || fragmentPath.empty()) return;
    ErrorString error;
    Shader_Program = LoadShaders(vertexPath.c_str(), fragmentPath.c_str(), error);
    if (Shader_Program == 0) {
        std::cout << error << std::endl;
        return;
    }
}

void Cube::loadTexture(const std::string& texturePath) {
    if (texturePath.empty()) return;

    int width = 0, height = 0, nChannels = 0;
    stbi_uc * data = stbi_load(texturePath.c_str(), &width, &height, &nChannels, 0);
    if (!data) {
        std::cout << "load texture failed" << std::endl;
        return;
    }

    GLuint texture;
    glGenTextures(1, &texture);
    TEXTUREs.push_back(texture);
    {
        glBindTexture(GL_TEXTURE_2D, texture);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
}

void Cube::connectTexture(GLenum tex, std::string uniform) {
    glActiveTexture(tex);
    GLint loc = glGetUniformLocation(Shader_Program, uniform.c_str());
    glUniform1i(loc, TexMap[tex]);
}

void Cube::Draw(GLuint points) {
    glUseProgram(Shader_Program);
    glBindVertexArray(VAO);
    connectTexture(GL_TEXTURE0, "wood");
    connectTexture(GL_TEXTURE1, "face");

    glDrawArrays(GL_TRIANGLES, 0, points);
}
