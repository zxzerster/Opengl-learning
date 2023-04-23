#include "shader_util.h"
#include <fstream>

#define make_error_string(msg) std::make_shared<std::string>(msg)

static std::string ReadFile(const char* file_path) {
    std::string content;
    std::ifstream file_stream(file_path, std::ios::in);

    if (!file_stream.is_open()) {
        return "";
    }

    std::string line = "";
    while (!file_stream.eof()) {
        std::getline(file_stream, line);
        content.append(line + "\n");
    }

    file_stream.close();
    return content;
}

static GLuint compileShader(const char* shader_code, GLenum shader_type, ErrorString& error) {
    GLuint shader = glCreateShader(shader_type);
    glShaderSource(shader, 1, &shader_code, nullptr);
    glCompileShader(shader);

    GLint result = GL_FALSE;
    int info_log_length = 0;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &result);
    glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &info_log_length);
    if (info_log_length > 0) {
        std::string error_message(info_log_length + 1, '\0');
        glGetShaderInfoLog(shader, info_log_length, nullptr, &error_message[0]);
        error = make_error_string(error_message);
        return 0;
    }

    return shader;
}

static GLuint linkProgram(GLuint vertex_shader, GLuint fragment_shader, ErrorString& error) {
    GLuint program = glCreateProgram();
    glAttachShader(program, vertex_shader);
    glAttachShader(program, fragment_shader);
    glLinkProgram(program);

    GLint result = GL_FALSE;
    int info_log_length = 0;
    glGetProgramiv(program, GL_LINK_STATUS, &result);
    glGetProgramiv(program, GL_INFO_LOG_LENGTH, &info_log_length);
    if (info_log_length > 0) {
        std::string error_message(info_log_length + 1, '\0');
        glGetProgramInfoLog(program, info_log_length, nullptr, &error_message[0]);
        error = make_error_string(error_message);
        return 0;
    }

    return program;
}

GLuint LoadShaders(const char * vertex_file_path,const char * fragment_file_path, ErrorString& error) {
    GLuint program = 0;
    GLuint vertex_shader = 0, fragment_shader = 0;

    auto vertex_shader_code = ReadFile(vertex_file_path); 
    if (vertex_shader_code.empty()) {
        error = make_error_string("Failed to read vertex shader file");
        return 0;
    }

    auto fragment_shader_code = ReadFile(fragment_file_path);
    if (fragment_shader_code.empty()) {
        error = make_error_string("Failed to read fragment shader file");
        return 0;
    }

    vertex_shader = compileShader(&vertex_shader_code[0], GL_VERTEX_SHADER, error);
    if (vertex_shader == 0) {
        return 0;
    }

    fragment_shader = compileShader(&fragment_shader_code[0], GL_FRAGMENT_SHADER, error);
    if (fragment_shader == 0) {
        return 0;
    }

    program = linkProgram(vertex_shader, fragment_shader, error);
    if (program == 0) {
        return 0;
    }

    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);

    return program;
}