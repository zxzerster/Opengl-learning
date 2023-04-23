#pragma once

#include "GL/glew.h"

#include <memory>
#include <string>

using ErrorString = std::shared_ptr<std::string>;

GLuint LoadShaders(const char * vertex_file_path, const char * fragment_file_path, ErrorString& error);