/**
 * @file shader.h
 * @author l1ang70
 * @brief The class that wraps the shader
 * @version 0.1
 * @date 2022-10-06
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#ifndef _SHADER_H_
#define _SHADER_H_

namespace opengl {

enum ShaderType : int {
    FRAGMENT_SHADER = 0x8B30,
    VERTEX_SHADER = 0x8B31
};

class Shader {
    friend class Program;

    unsigned int    shader_id_          = 0;
    int             compile_success_    = 0;

public:
    Shader(const char *file_path, ShaderType type = FRAGMENT_SHADER);
    ~Shader();

    inline bool CompileSucCess() { return compile_success_ != 0; }
};

}

#endif // !_SHADER_H_