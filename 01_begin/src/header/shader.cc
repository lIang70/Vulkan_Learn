#include "shader.h"

#include <glad/glad.h>

#include <string>
#include <iostream>
#include <fstream>
#include <sstream>

namespace opengl {

Shader::Shader(const char * path, ShaderType type) {
    std::string     shader_code{};
    std::ifstream   shader_file{};
    shader_file.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    try {
        shader_file.open(path);
        std::stringstream shader_stream;
        shader_stream << shader_file.rdbuf();
        shader_file.close();
        shader_code = shader_stream.str();
    } catch (std::ifstream::failure &e) {
        std::cout << "[Error] Fail to read shader file:\n" << path << "\n" << e.what() << std::endl;
    }
    const char * source = shader_code.c_str();
    shader_id_ = glCreateShader(type);
    glShaderSource(shader_id_, 1, &source, nullptr);
    glCompileShader(shader_id_);
    glGetShaderiv(shader_id_, GL_COMPILE_STATUS, &compile_success_);
    if (!compile_success_) {
        GLint info_len = 0;
        glGetShaderiv(shader_id_, GL_INFO_LOG_LENGTH, &info_len);
        GLchar * info = new GLchar[info_len];
        glGetShaderInfoLog(shader_id_, info_len, nullptr, info);
        fprintf(stdout, "[Error] Failed to compile shader:\n %s\n", info);
        delete[] info;
    }
}

Shader::~Shader() {
    glDeleteShader(shader_id_);
    shader_id_ = 0;
}

}