/**
 * @file program.h
 * @author l1ang70
 * @brief The class that wraps the program
 * @version 0.1
 * @date 2022-10-06
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#ifndef _OPENGL_PROGRAM_H_
#define _OPENGL_PROGRAM_H_

#include <string>
#include <glm/glm.hpp>

namespace opengl {

class Shader;

class Program {
    unsigned int program_id_ = 0;
    
private:
    Program(unsigned int program_id = 0);

public:
    static Program* create();

    ~Program();

    inline bool isValid() { return program_id_ != 0; }

    void checkInfo(unsigned int name, int* params);

    void attachShader(Shader* shader);
    
    bool link();

    void setParam1(const std::string &name, bool value) const;
    void setParam1(const std::string &name, int value) const;
    void setParam1(const std::string &name, float value) const;

    void setMatrix4(const std::string &name, glm::mat4 value) const;

    void use();
};

}

#endif // !_OPENGL_PROGRAM_H_