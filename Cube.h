#pragma once

#include <GL/glew.h>
#include <vector>
#include <string>
#include <unordered_map>

#include <glm/vec3.hpp> // glm::vec3
#include <glm/vec4.hpp> // glm::vec4
#include <glm/mat4x4.hpp> // glm::mat4
#include <glm/ext/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale
#include <glm/ext/matrix_clip_space.hpp> // glm::perspective
#include <glm/ext/scalar_constants.hpp> // glm::pi
#include <glm/gtc/type_ptr.hpp>

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
    void useProgram();
    void connectTexture(GLenum tex, std::string uniform);
    template<typename T>
    void setUniform(const std::string& name, T value) {
        std::cout << "set uniform shader: " << Shader_Program << std::endl;
        GLuint loc = glGetUniformLocation(Shader_Program, name.c_str());
        if (loc == -1) {
            std::cout << "Uniform " << name << " not found" << std::endl;
            return;
        }

        if (std::is_same<T, GLint>()) {
            GLint v;
            memcpy(&v, &value, sizeof(GLint));
            glUniform1i(loc, v);
        } else if (std::is_same<T, GLfloat>()) {
            GLfloat v;
            memcpy(&v, &value, sizeof(GLfloat));
            glUniform1f(loc, v);
        } else if (std::is_same<T, glm::mat4>()) {
            glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(value));
        } else if (std::is_same<T, glm::mat3>()) {
            glUniformMatrix3fv(loc, 1, GL_FALSE, glm::value_ptr(value));
        } else {
            std::cout << "Can not handle this type yet" << std::endl;
        }
    }

    void enableDepthTest() {
        glEnable(GL_DEPTH_TEST);
    }

private:
    unsigned int VAO = 0, Shader_Program = 0;
    std::vector<GLuint> VBOs, EBOs;
    std::vector<unsigned int> TEXTUREs;
    std::unordered_map<GLenum, GLuint> TexMap;
};