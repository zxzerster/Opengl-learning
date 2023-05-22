#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "textures/stb_image.h"

#include <glm/vec3.hpp> // glm::vec3
#include <glm/vec4.hpp> // glm::vec4
#include <glm/mat4x4.hpp> // glm::mat4
#include <glm/ext/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale
#include <glm/ext/matrix_clip_space.hpp> // glm::perspective
#include <glm/ext/scalar_constants.hpp> // glm::pi
#include <glm/gtc/type_ptr.hpp>

#include <iostream>
#include <memory>

#include "shader/shader_util.h"

#include "Cube.h"

#define WINDOW_WIDTH 1440
#define WINDOW_HEIGHT 900

GLuint CUBE_VAO = 0, CUBE_VBO = 0, CUBE_EBO = 0, WOOD_TEXTURE = 0, FACE_TEXTURE = 0;
GLuint cube_program = 0;

Cube cube0, cube1;

const GLfloat cubeVertices[] = {
    -0.5f, -0.5f, -0.5f,
     0.5f, -0.5f, -0.5f,
     0.5f,  0.5f, -0.5f,
     0.5f,  0.5f, -0.5f,
    -0.5f,  0.5f, -0.5f,
    -0.5f, -0.5f, -0.5f,
    -0.5f, -0.5f,  0.5f,
     0.5f, -0.5f,  0.5f,
     0.5f,  0.5f,  0.5f,
     0.5f,  0.5f,  0.5f,
    -0.5f,  0.5f,  0.5f,
    -0.5f, -0.5f,  0.5f,
    -0.5f,  0.5f,  0.5f,
    -0.5f,  0.5f, -0.5f,
    -0.5f, -0.5f, -0.5f,
    -0.5f, -0.5f, -0.5f,
    -0.5f, -0.5f,  0.5f,
    -0.5f,  0.5f,  0.5f,
     0.5f,  0.5f,  0.5f,
     0.5f,  0.5f, -0.5f,
     0.5f, -0.5f, -0.5f,
     0.5f, -0.5f, -0.5f,
     0.5f, -0.5f,  0.5f,
     0.5f,  0.5f,  0.5f,
    -0.5f, -0.5f, -0.5f,
     0.5f, -0.5f, -0.5f,
     0.5f, -0.5f,  0.5f,
     0.5f, -0.5f,  0.5f,
    -0.5f, -0.5f,  0.5f,
    -0.5f, -0.5f, -0.5f,
    -0.5f,  0.5f, -0.5f,
     0.5f,  0.5f, -0.5f,
     0.5f,  0.5f,  0.5f,
     0.5f,  0.5f,  0.5f,
    -0.5f,  0.5f,  0.5f,
    -0.5f,  0.5f, -0.5f,
};

const GLfloat texCoords[] = {
    0.0f, 0.0f,
    1.0f, 0.0f,
    1.0f, 1.0f,
    1.0f, 1.0f,
    0.0f, 1.0f,
    0.0f, 0.0f,
    0.0f, 0.0f,
    1.0f, 0.0f,
    1.0f, 1.0f,
    1.0f, 1.0f,
    0.0f, 1.0f,
    0.0f, 0.0f,
    1.0f, 0.0f,
    1.0f, 1.0f,
    0.0f, 1.0f,
    0.0f, 1.0f,
    0.0f, 0.0f,
    1.0f, 0.0f,
    1.0f, 0.0f,
    1.0f, 1.0f,
    0.0f, 1.0f,
    0.0f, 1.0f,
    0.0f, 0.0f,
    1.0f, 0.0f,
    0.0f, 1.0f,
    1.0f, 1.0f,
    1.0f, 0.0f,
    1.0f, 0.0f,
    0.0f, 0.0f,
    0.0f, 1.0f,
    0.0f, 1.0f,
    1.0f, 1.0f,
    1.0f, 0.0f,
    1.0f, 0.0f,
    0.0f, 0.0f,
    0.0f, 1.0f,
};

const GLfloat colors[] = {
    0.0f, 0.0f, 0.8f,
    1.0f, 0.0f, 0.8f,
    1.0f, 1.0f, 0.8f,
    1.0f, 1.0f, 0.8f,
    0.0f, 1.0f, 0.8f,
    0.0f, 0.0f, 0.8f,
    0.0f, 0.0f, 0.8f,
    1.0f, 0.0f, 0.8f,
    1.0f, 1.0f, 0.8f,
    1.0f, 1.0f, 0.8f,
    0.0f, 1.0f, 0.8f,
    0.0f, 0.0f, 0.8f,
    1.0f, 0.0f, 0.8f,
    1.0f, 1.0f, 0.8f,
    0.0f, 1.0f, 0.8f,
    0.0f, 1.0f, 0.8f,
    0.0f, 0.0f, 0.8f,
    1.0f, 0.0f, 0.8f,
    1.0f, 0.0f, 0.8f,
    1.0f, 1.0f, 0.8f,
    0.0f, 1.0f, 0.8f,
    0.0f, 1.0f, 0.8f,
    0.0f, 0.0f, 0.8f,
    1.0f, 0.0f, 0.8f,
    0.0f, 1.0f, 0.8f,
    1.0f, 1.0f, 0.8f,
    1.0f, 0.0f, 0.8f,
    1.0f, 0.0f, 0.8f,
    0.0f, 0.0f, 0.8f,
    0.0f, 1.0f, 0.8f,
    0.0f, 1.0f, 0.8f,
    1.0f, 1.0f, 0.8f,
    1.0f, 0.0f, 0.8f,
    1.0f, 0.0f, 0.8f,
    0.0f, 0.0f, 0.8f,
    0.0f, 1.0f, 0.8f,
};

const GLuint cubeVertexIndicies[] = {
    0, 1, 2, 2, 3, 0, // Front face
    1, 5, 6, 6, 2, 1, // Right face
    4, 0, 3, 3, 7, 4, // Left face
    5, 4, 7, 7, 6, 5, // Back face
    3, 2, 6, 6, 7, 3, // Top face
    0, 4, 5, 5, 1, 0  // Bottom face
};

glm::vec3 cubePositions[] = {
    glm::vec3( 0.0f, 0.0f, -1.2f),
#if 0
    glm::vec3( 2.0f, 5.0f, -80.0f),
    glm::vec3(-1.5f, -2.2f, -2.5f),
    glm::vec3(-3.8f, -2.0f, -12.3f),
    glm::vec3( 2.4f, -0.4f, -3.5f),
    glm::vec3(-1.7f, 3.0f, -7.5f),
    glm::vec3( 1.3f, -2.0f, -2.5f),
    glm::vec3( 1.5f, 2.0f, -2.5f),
    glm::vec3( 1.5f, 0.2f, -1.5f),
    glm::vec3(-1.3f, 1.0f, -1.5f)
#endif
};


static void Cube0Init() {
    cube0.loadVertexAttribute(0, const_cast<GLfloat *>(cubeVertices), sizeof(cubeVertices), 3);
    cube0.loadVertexAttribute(1, const_cast<GLfloat *>(colors), sizeof(colors), 3);
    cube0.loadVertexAttribute(2, const_cast<GLfloat *>(texCoords), sizeof(texCoords), 2);
    cube0.loadShaders("C:\\Users\\Zxzerster\\Documents\\Dev\\Opengl\\Opengl-learning\\shader\\glsl\\cube.vert","C:\\Users\\Zxzerster\\Documents\\Dev\\Opengl\\Opengl-learning\\shader\\glsl\\cube.frag");

    cube0.loadTexture("C:\\Users\\Zxzerster\\Documents\\Dev\\Opengl\\Opengl-learning\\textures\\container.jpg");
    // cube.loadTexture("C:\\Users\\Zxzerster\\Documents\\Dev\\Opengl\\Opengl-learning\\textures\\awesomeface.png");

    cube0.connectTexture(GL_TEXTURE0, "wood");

    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(0.0f, 0.0f, -3.0f));
    // model = glm::rotate(model, glm::radians(-55.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    cube0.setUniform("model", model);

    glm::mat4 view = glm::mat4(1.0f);
    // view = glm::translate(view, glm::vec3(-0.3125f, 0.3125f, -3.0f));
    cube0.setUniform("view", view);

    glm::mat4 projection = glm::mat4(1.0f);
    projection = glm::perspective(glm::radians(45.0f), (float)1440 / (float)900, 0.1f, 100.0f);
    cube0.setUniform("projection", projection);

    cube0.enableDepthTest();

    cube1.loadVertexAttribute(0, const_cast<GLfloat *>(cubeVertices), sizeof(cubeVertices), 3);
    cube1.loadVertexAttribute(1, const_cast<GLfloat *>(colors), sizeof(colors), 3);
    cube1.loadVertexAttribute(2, const_cast<GLfloat *>(texCoords), sizeof(texCoords), 2);
    cube1.loadShaders("C:\\Users\\Zxzerster\\Documents\\Dev\\Opengl\\Opengl-learning\\shader\\glsl\\cube.vert","C:\\Users\\Zxzerster\\Documents\\Dev\\Opengl\\Opengl-learning\\shader\\glsl\\cube.frag");

    cube1.loadTexture("C:\\Users\\Zxzerster\\Documents\\Dev\\Opengl\\Opengl-learning\\textures\\container.jpg");
    // cube.loadTexture("C:\\Users\\Zxzerster\\Documents\\Dev\\Opengl\\Opengl-learning\\textures\\awesomeface.png");

    cube1.connectTexture(GL_TEXTURE0, "wood");

    model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(-7.0f, 1.0f, -13.0f));
    // model = glm::rotate(model, glm::radians(-15.0f), glm::vec3(0.5f, 1.0f, 0.0f));
    cube1.setUniform("model", model);

    view = glm::mat4(1.0f);
    // view = glm::translate(view, glm::vec3(-0.3125f, 0.3125f, -3.0f));
    cube1.setUniform("view", view);

    projection = glm::mat4(1.0f);
    projection = glm::perspective(glm::radians(45.0f), (float)1440 / (float)900, 0.1f, 100.0f);
    cube1.setUniform("projection", projection);
}

void GLInit() {
    Cube0Init();
}

void GLRendering() {
    cube0.useProgram();
    cube0.Draw(sizeof(cubeVertices) / (sizeof(GLfloat) * 3));

    cube1.useProgram();
    cube1.Draw(sizeof(cubeVertices) / (sizeof(GLfloat) * 3));
}

int main(int argc, char* argv[]) {
    if (glfwInit() != GLFW_TRUE) {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return EXIT_FAILURE;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); // set OpenGL version 3.3
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // use core profile
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // make MacOS happy
#endif

    GLFWwindow * pWindow = glfwCreateWindow(1440, 900, "LearnOpenGL", nullptr, nullptr);
    if (!pWindow) {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return EXIT_FAILURE;
    }
    glfwMakeContextCurrent(pWindow);

    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK) {
        std::cerr << "Failed to initialize GLEW" << std::endl;
        glfwTerminate();
        return EXIT_FAILURE;
    }

    glfwSetKeyCallback(pWindow, [](GLFWwindow* window, int key, int scancode, int action, int mode) {
        if (key == GLFW_KEY_ESCAPE) {
            glfwSetWindowShouldClose(window, GLFW_TRUE);
        }
    });
    glfwSetWindowSizeCallback(pWindow, [](GLFWwindow* window, int width, int height) {
        glViewport(0, 0, width, height);
    });

    glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);

    GLInit();
    while (!glfwWindowShouldClose(pWindow)) {
        glfwPollEvents();
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f); // set color
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // clear color buffer

        GLRendering();

        glfwSwapBuffers(pWindow);

        std::shared_ptr<unsigned char> d(new unsigned char[1024], [](unsigned char* ptr) { delete[] ptr; });
        std::shared_ptr<const GLvoid> data;
        
        data = std::shared_ptr<GLvoid>(malloc(1024), [](GLvoid* ptr) { free(ptr ); });
    }
}