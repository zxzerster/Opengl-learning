#include "Cube.h"
#include "shader/shader_util.h"
#include "textures/stb_image.h"

#include <type_traits>
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/ext.hpp>
#include <glm/gtx/string_cast.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/matrix.hpp>

Cube::Cube() {
    TexMap[GL_TEXTURE0] = 0;
    TexMap[GL_TEXTURE1] = 1;
    TexMap[GL_TEXTURE2] = 2;
    TexMap[GL_TEXTURE3] = 3;
    TexMap[GL_TEXTURE4] = 4;
    TexMap[GL_TEXTURE5] = 5;
}

Cube::~Cube() {

}

void Cube::loadVertexAttribute(unsigned int location, GLfloat* vertices, unsigned int size, unsigned int components)
{
    if (!vertices || size < 0) return;
    if (size % components != 0) return;
    if (VAO == 0)
        glGenVertexArrays(1, &VAO);

    GLuint VBO;
    glGenBuffers(1, &VBO);
    VBOs.push_back(VBO);
    glBindVertexArray(VAO);
    {
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);
        glVertexAttribPointer(location, components, GL_FLOAT, GL_FALSE, components * sizeof(GLfloat), (void*)0);
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

    glUseProgram(Shader_Program);
}

void Cube::loadTexture(const std::string& texturePath) {
    if (texturePath.empty()) return;
    int width = 0, height = 0, nChannels = 0;
    stbi_uc * data = stbi_load(texturePath.c_str(), &width, &height, &nChannels, 0);
    if (!data) {
        return;
    }
    GLuint texture;
    glGenTextures(1, &texture);
    TEXTUREs.push_back(texture);
    {
        glBindTexture(GL_TEXTURE_2D, texture);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
}

void Cube::connectTexture(GLenum tex, std::string uniform) {
    glActiveTexture(tex);
    GLint loc = glGetUniformLocation(Shader_Program, uniform.c_str());
    glUniform1i(loc, TexMap[tex]);
}

void Cube::Draw(GLuint points) {
    glBindVertexArray(VAO);
    // connectTexture(GL_TEXTURE0, "wood");

    glDrawArrays(GL_TRIANGLES, 0, points);
}
