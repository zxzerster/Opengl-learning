#include "gl/glew.h"
#include "glfw/glfw3.h"
#include <cstdlib>
#include <string>
#include <iostream>

#include "shader/shader_util.h"
#include "textures/stb_image.h"

// OpenGL version
static const GLuint VERSION_MAJOR = 3;
static const GLuint VERSION_MINOR = 3;

// Window properties
static const std::string WINDOW_TITLE = "OpenGL Learning";

static const GLuint WINDOW_WIDTH = 1440;
static const GLuint WINDOW_HEIGHT = 900;

// OpenGL buffer objects
static GLuint VAO = 0, VBO = 0, EBO = 0;

// Textures
GLuint wood_tex = 0, face_tex = 0;

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

static void _texture_wood();
static void _texture_face();

static void GLInit() {
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    ErrorString error;
    SingleTriangleProgram = LoadShaders("C:\\Users\\Zxzerster\\Documents\\Dev\\Opengl\\Opengl-learning\\shader\\glsl\\SingleTriangle.vert", "C:\\Users\\Zxzerster\\Documents\\Dev\\Opengl\\Opengl-learning\\shader\\glsl\\SingleTriangle.frag", error);
    if (SingleTriangleProgram == 0) {
        std::cout << *error << std::endl;
        // abort();
        return;
    }
    _texture_wood();
    _texture_face();

// Single Triangle VAO
    glGenVertexArrays(1, &SingleTriangle);
    glGenBuffers(1, &SingleTriangleBuffer);

    glBindVertexArray(SingleTriangle);
    {        
        glBindBuffer(GL_ARRAY_BUFFER, SingleTriangleBuffer);
        {
            GLfloat vertices[] = {
                /*pos*/             /*color*/         /*tex*/
                -0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, // left
                 0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
                 0.0f,  0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.5f, 1.0f
            };
            glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GL_FLOAT), (void*)0);
            glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GL_FLOAT), (void*)(3 * sizeof(GL_FLOAT)));
            glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GL_FLOAT), (void*)(6 * sizeof(GL_FLOAT)));
            glEnableVertexAttribArray(0);
            glEnableVertexAttribArray(1);
            glEnableVertexAttribArray(2);
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
             /*pos*/            /*color*/         /*tex*/
             0.5f,  0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,  // top right
             0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,  // bottom right
            -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,  // bottom left
            -0.5f,  0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f   // top left 
        };
        GLuint indices[] = {
            0, 1, 3,
            1, 2, 3
        };

        glBindBuffer(GL_ARRAY_BUFFER, RectangleBuffer);
        glBufferData(GL_ARRAY_BUFFER, sizeof(rectangleVertices), rectangleVertices, GL_STATIC_DRAW);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GL_FLOAT), (void*)0);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GL_FLOAT), (void*)(3 * sizeof(GL_FLOAT)));
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GL_FLOAT), (void*)(6 * sizeof(GL_FLOAT)));
        glEnableVertexAttribArray(0);
        glEnableVertexAttribArray(1);
        glEnableVertexAttribArray(2);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ReactangleIndicesBuffer);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    }
    glBindVertexArray(0);

    glUseProgram(SingleTriangleProgram);

    glUniform1i(glGetUniformLocation(SingleTriangleProgram, "wood"), 0);
    glUniform1i(glGetUniformLocation(SingleTriangleProgram, "face"), 1);
}

static void _render_single_triangle() {
    glBindVertexArray(SingleTriangle);
    glDrawArrays(GL_TRIANGLES, 0, 3);
}

static void _render_rectangle() {
    glBindVertexArray(Rectangle);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}

static void _texture_wood() {
    int width, height, nrChannels;
    glGenTextures(1, &wood_tex);
    
    unsigned char* wood = stbi_load("C:\\Users\\Zxzerster\\Documents\\Dev\\Opengl\\Opengl-learning\\textures\\container.jpg", &width, &height, &nrChannels, 0);
    if (!wood) {
        std::cout << "Failed to load container image" << std::endl;
        return;
    }
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, wood_tex);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, wood);

    glGenerateMipmap(GL_TEXTURE_2D);

    stbi_image_free(wood);
}

static void _texture_face() {
    int width, height, nrChannels;
    glGenTextures(1, &face_tex);

    unsigned char* face = stbi_load("C:\\Users\\Zxzerster\\Documents\\Dev\\Opengl\\Opengl-learning\\textures\\awesomeface.png", &width, &height, &nrChannels, 0);
    if (!face) {
        std::cout << "Failed to load awesomeface image" << std::endl;
        return;
    }
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, face_tex);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, face);

    glGenerateMipmap(GL_TEXTURE_2D);

    stbi_image_free(face);
}

static void GLRender() {
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    // glBindTexture(GL_TEXTURE_2D, wood_tex);

    GLfloat green = (GLfloat)(sin(glfwGetTime()) / 2) + 0.5f;
    GLuint greenLoc = glGetUniformLocation(SingleTriangleProgram, "green");

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