#pragma once

#include <GL/glew.h>
#include <vector>
#include <string>
#include <unordered_map>

class Cube
{
public:
    Cube();
    ~Cube();

    void Draw(GLuint points);
public:
    void loadVertexAttribute(unsigned int location, GLfloat* vertices, unsigned int size, unsigned int components);
    void loadShaders(const std::string& vertexPath, const std::string& fragmentPath);
    void loadTexture(const std::string& texturePath);
    void connectTexture(GLenum tex, std::string uniform);
private:
    unsigned int VAO = 0, Shader_Program = 0;
    std::vector<GLuint> VBOs, EBOs;
    std::vector<unsigned int> TEXTUREs;
    std::unordered_map<GLenum, GLuint> TexMap;
};