#include "gl/glew.h"
#include "glfw/glfw3.h"
#include <cstdlib>
#include <string>
#include <iostream>
#include "shader/shader_util.h"

// OpenGL version
static const GLuint VERSION_MAJOR = 4;
static const GLuint VERSION_MINOR = 5;

// Window properties
static const std::string WINDOW_TITLE = "OpenGL Learning";

static const GLuint WINDOW_WIDTH = 1440;
static const GLuint WINDOW_HEIGHT = 900;

// OpenGL buffer objects
static GLuint VAO = 0, VBO = 0, EBO = 0;

static GLfloat color[] = {1.0f, 1.0f, 1.0f};

static void GLInit() {
    ErrorString error;
    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK) {
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);

    glPointSize(40);

    auto program = LoadShaders("../../shader/glsl/v.vert", "../../shader/glsl/f.frag", error);
    if (program == 0) {
        std::cout << *error << std::endl;
        abort();
    }

    glUseProgram(program);

    glCreateVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
}

static void GLRender() {
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glDrawArrays(GL_POINTS, 0, 1);
}

int main(int argc, char* argv[]) {
    if (glfwInit() == GL_FALSE) {
        return EXIT_FAILURE;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, VERSION_MAJOR);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, VERSION_MINOR);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    auto window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, &WINDOW_TITLE[0], nullptr, nullptr);
    if (window == nullptr) {
        glfwTerminate();
        return EXIT_FAILURE;
    }

    glfwMakeContextCurrent(window);

    GLInit();
    while (glfwWindowShouldClose(window) == GL_FALSE) {
        glfwPollEvents();

        GLRender();

        glfwSwapBuffers(window);
    }

    return 0;
}