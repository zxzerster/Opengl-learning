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

#define WINDOW_WIDTH 1440
#define WINDOW_HEIGHT 900

GLuint CUBE_VAO = 0, CUBE_VBO = 0, CUBE_EBO = 0, WOOD_TEXTURE = 0, FACE_TEXTURE = 0;
GLuint cube_program = 0;

const GLfloat cubeVertices[] = {
#if 0
    -0.5f, -0.5f, -0.5f, // Vertex 0
     0.5f, -0.5f, -0.5f, // Vertex 1
     0.5f,  0.5f, -0.5f, // Vertex 2
    -0.5f,  0.5f, -0.5f, // Vertex 3
    -0.5f, -0.5f,  0.5f, // Vertex 4
     0.5f, -0.5f,  0.5f, // Vertex 5
     0.5f,  0.5f,  0.5f, // Vertex 6
    -0.5f,  0.5f,  0.5f  // Vertex 7
#endif
    -0.5f, -0.5f, -0.5f, 0.0f, 0.0f,
     0.5f, -0.5f, -0.5f, 1.0f, 0.0f,
     0.5f,  0.5f, -0.5f, 1.0f, 1.0f,
     0.5f,  0.5f, -0.5f, 1.0f, 1.0f,
    -0.5f,  0.5f, -0.5f, 0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f, 0.0f, 0.0f,
    -0.5f, -0.5f,  0.5f, 0.0f, 0.0f,
     0.5f, -0.5f,  0.5f, 1.0f, 0.0f,
     0.5f,  0.5f,  0.5f, 1.0f, 1.0f,
     0.5f,  0.5f,  0.5f, 1.0f, 1.0f,
    -0.5f,  0.5f,  0.5f, 0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f, 0.0f, 0.0f,
    -0.5f,  0.5f,  0.5f, 1.0f, 0.0f,
    -0.5f,  0.5f, -0.5f, 1.0f, 1.0f,
    -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f, 0.0f, 0.0f,
    -0.5f,  0.5f,  0.5f, 1.0f, 0.0f,
     0.5f,  0.5f,  0.5f, 1.0f, 0.0f,
     0.5f,  0.5f, -0.5f, 1.0f, 1.0f,
     0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
     0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
     0.5f, -0.5f,  0.5f, 0.0f, 0.0f,
     0.5f,  0.5f,  0.5f, 1.0f, 0.0f,
    -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
     0.5f, -0.5f, -0.5f, 1.0f, 1.0f,
     0.5f, -0.5f,  0.5f, 1.0f, 0.0f,
     0.5f, -0.5f,  0.5f, 1.0f, 0.0f,
    -0.5f, -0.5f,  0.5f, 0.0f, 0.0f,
    -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
    -0.5f,  0.5f, -0.5f, 0.0f, 1.0f,
     0.5f,  0.5f, -0.5f, 1.0f, 1.0f,
     0.5f,  0.5f,  0.5f, 1.0f, 0.0f,
     0.5f,  0.5f,  0.5f, 1.0f, 0.0f,
    -0.5f,  0.5f,  0.5f, 0.0f, 0.0f,
    -0.5f,  0.5f, -0.5f, 0.0f, 1.0f
};

const GLuint cubeVertexIndicies[] = {
    0, 1, 2, 2, 3, 0, // Front face
    1, 5, 6, 6, 2, 1, // Right face
    4, 0, 3, 3, 7, 4, // Left face
    5, 4, 7, 7, 6, 5, // Back face
    3, 2, 6, 6, 7, 3, // Top face
    0, 4, 5, 5, 1, 0  // Bottom face
};

void GLInit() {
    ErrorString error;

// Vertex attributes
    glGenVertexArrays(1, &CUBE_VAO);
    glBindVertexArray(CUBE_VAO);
    {
        glGenBuffers(1, &CUBE_VBO);
        glBindBuffer(GL_ARRAY_BUFFER, CUBE_VBO);
        {
            glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), cubeVertices, GL_STATIC_DRAW);
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (void *)0);
            glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (void *)(3 * sizeof(GLfloat)));
            glEnableVertexAttribArray(0);
            glEnableVertexAttribArray(1);
        }
    }
    glBindVertexArray(0);
// Texture
    glGenTextures(1, &WOOD_TEXTURE);
    glBindTexture(GL_TEXTURE_2D, WOOD_TEXTURE);
    {
        int width, height, nChannels;
        stbi_uc* cubeTexture = stbi_load("C:\\Users\\Zxzerster\\Documents\\Dev\\Opengl\\Opengl-learning\\textures\\container.jpg", &width, &height, &nChannels, 0);
        if (!cubeTexture) {
            abort();
        }
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, cubeTexture);
        glGenerateMipmap(GL_TEXTURE_2D);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    }

    glGenTextures(1, &FACE_TEXTURE);
    glBindTexture(GL_TEXTURE_2D, FACE_TEXTURE);
    {
        int width, height, nChannels;
        stbi_uc* faceTexture = stbi_load("C:\\Users\\Zxzerster\\Documents\\Dev\\Opengl\\Opengl-learning\\textures\\awesomeface.png", &width, &height, &nChannels, 0);
        if (!faceTexture) {
            abort();
        }
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, faceTexture);
        glGenerateMipmap(GL_TEXTURE_2D);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    }

// Shader program
    cube_program = LoadShaders("C:\\Users\\Zxzerster\\Documents\\Dev\\Opengl\\Opengl-learning\\shader\\glsl\\cube.vert", "C:\\Users\\Zxzerster\\Documents\\Dev\\Opengl\\Opengl-learning\\shader\\glsl\\cube.frag", error);
    if (cube_program == 0) {
        std::cout << error << std::endl;
        return;
    }

    glUseProgram(cube_program);

// model, view, projection
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::rotate(model, glm::radians(-55.0f), glm::vec3(0.5f, 1.0f, 0.0f));
    // transform = glm::translate(transform, glm::vec3(0.0f, 0.0f, 0.04f));
    glUniformMatrix4fv(glGetUniformLocation(cube_program, "model"), 1, GL_FALSE, glm::value_ptr(model));

    glm::mat4 view = glm::mat4(1.0f);
    view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));
    glUniformMatrix4fv(glGetUniformLocation(cube_program, "view"), 1, GL_FALSE, glm::value_ptr(view));

    glm::mat4 projection = glm::mat4(1.0f);
    projection = glm::perspective(glm::radians(45.0f), (float)1440 / (float)900, 0.1f, 100.0f);
    glUniformMatrix4fv(glGetUniformLocation(cube_program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

    glEnable(GL_DEPTH_TEST);
}

void GLRendering() {
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, WOOD_TEXTURE);
    glUniform1i(glGetUniformLocation(cube_program, "wood"), 0);

    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, FACE_TEXTURE);
    glUniform1i(glGetUniformLocation(cube_program, "face"), 1);

    glBindVertexArray(CUBE_VAO);
    glDrawArrays(GL_TRIANGLES, 0, 36);
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