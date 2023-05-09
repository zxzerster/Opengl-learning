#include "gl/glew.h"
#include "glfw/glfw3.h"
#include <cstdlib>
#include <string>
#include <iostream>
#include "shader/shader_util.h"

// OpenGL version
static const GLuint VERSION_MAJOR = 3;
static const GLuint VERSION_MINOR = 3;

// Window properties
static const std::string WINDOW_TITLE = "OpenGL Learning";

static const GLuint WINDOW_WIDTH = 1440;
static const GLuint WINDOW_HEIGHT = 900;

// OpenGL buffer objects
static GLuint VAO = 0, VBO = 0, EBO = 0;

static GLfloat color[] = {1.0f, 1.0f, 1.0f};

bool drawTrianle = true;

void onSizeChanged(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

void onKeyInput(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (key == GLFW_KEY_ESCAPE) {
        glfwSetWindowShouldClose(window, GL_TRUE);
    } else if (key == GLFW_KEY_T) {
        drawTrianle = true;
    } else if (key == GLFW_KEY_R) {
        drawTrianle = false;
    } else {
        drawTrianle = true;
    }
}


GLuint SingleTriangle, SingleTriangleBuffer, SingleTriangleProgram;
GLuint Rectangle, RectangleBuffer, ReactangleIndicesBuffer, RectangleProgram;

static void GLInit() {
    ErrorString error;
    SingleTriangleProgram = LoadShaders("../../shader/glsl/SingleTriangle.vert", "../../shader/glsl/SingleTriangle.frag", error);
    if (SingleTriangleProgram == 0) {
        std::cout << *error << std::endl;
        abort();
    }

// Single Triangle VAO
    glGenVertexArrays(1, &SingleTriangle);
    glGenBuffers(1, &SingleTriangleBuffer);

    glBindVertexArray(SingleTriangle);
    {        
        glBindBuffer(GL_ARRAY_BUFFER, SingleTriangleBuffer);
        {
            GLfloat vertices[] = {
                -0.5f, -0.5f, 0.0f,
                 0.5f, -0.5f, 0.0f,
                 0.0f,  0.5f, 0.0f
            };
            glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GL_FLOAT), (void*)0);
            glEnableVertexAttribArray(0);
        }
    }
    glBindVertexArray(0);

// Rectangle VAO
    glGenVertexArrays(1, &Rectangle);
    glGenBuffers(1, &RectangleBuffer);
    glGenBuffers(1, &ReactangleIndicesBuffer);

    glBindVertexArray(Rectangle);
    {
        GLfloat rectangleVertices[] = {
             0.5f,  0.5f, 0.0f,   // top right
             0.5f, -0.5f, 0.0f,   // bottom right
            -0.5f, -0.5f, 0.0f,   // bottom left
            -0.5f,  0.5f, 0.0f,   // top left 
        };
        GLuint indices[] = {
            0, 1, 3,
            1, 2, 3
        };

        glBindBuffer(GL_ARRAY_BUFFER, RectangleBuffer);
        glBufferData(GL_ARRAY_BUFFER, sizeof(rectangleVertices), rectangleVertices, GL_STATIC_DRAW);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GL_FLOAT), (void*)0);
        glEnableVertexAttribArray(0);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ReactangleIndicesBuffer);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    }
    glBindVertexArray(0);
}

static void _render_single_triangle() {
    glBindVertexArray(SingleTriangle);
    glDrawArrays(GL_TRIANGLES, 0, 3);
}

static void _render_rectangle() {
    glBindVertexArray(Rectangle);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}

static void GLRender() {
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    GLfloat green = (GLfloat)(sin(glfwGetTime()) / 2) + 0.5f;
    GLuint greenLoc = glGetUniformLocation(SingleTriangleProgram, "green");

    glUseProgram(SingleTriangleProgram);
    glUniform1f(greenLoc, green);
    
    if (drawTrianle) {
        _render_single_triangle();
    } else {
        _render_rectangle();
    }
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

    glfwSetFramebufferSizeCallback(window, onSizeChanged);
    glfwSetKeyCallback(window, onKeyInput);
    glfwMakeContextCurrent(window);

    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK) {
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);

    GLInit();
    while (glfwWindowShouldClose(window) == GL_FALSE) {
        GLRender();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}