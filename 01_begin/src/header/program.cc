#include "program.h"
#include "shader.h"

#include <glad/glad.h>

#include <cstdio>

namespace opengl {

Program::Program(unsigned int program_id) 
    : program_id_(program_id) {}

Program*
Program::Create() {
    auto id = glCreateProgram();
    if (!id)
        return nullptr;
    
    return new Program(id);
}

Program::~Program() {
    glDeleteProgram(program_id_);
    program_id_ = 0;
}

void 
Program::CheckInfo(unsigned int name, int* params) {
    glGetProgramiv(program_id_, name, params);
}

void 
Program::AttachShader(Shader* shader) {
    glAttachShader(program_id_, shader->shader_id_);
}

bool
Program::Link() {
    glLinkProgram(program_id_);
    GLint success = 0;
    CheckInfo(GL_LINK_STATUS, &success);
    if (!success) {
        GLint info_len = 0;
        CheckInfo(GL_INFO_LOG_LENGTH, &info_len);
        GLchar * info = new GLchar[info_len];
        glGetProgramInfoLog(program_id_, info_len, nullptr, info);
        fprintf(stdout, "[Error] Failed to link program: %s\n", info);
        delete[] info;
        return false;
    }
    return true;
}

void
Program::SetParam1(const std::string &name, bool value) const {
    glUniform1i(glGetUniformLocation(program_id_, name.c_str()), value);
}

void
Program::SetParam1(const std::string &name, int value) const {
    glUniform1i(glGetUniformLocation(program_id_, name.c_str()), value);
}

void
Program::SetParam1(const std::string &name, float value) const {
    glUniform1f(glGetUniformLocation(program_id_, name.c_str()), value);
}

void
Program::Use() {
    glUseProgram(program_id_);
}

}
