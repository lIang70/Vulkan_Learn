#define STB_IMAGE_IMPLEMENTATION
#include "header/stb_image.h"
#include "header/program.h"
#include "header/shader.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <cmath>
#include <iostream>
#include <unistd.h>

void FrameBufferSizeChangedCB(GLFWwindow* gl_window, int width, int height) {
    // Change view port
    glViewport(0, 0, width, height);
}

void ProcessInput(GLFWwindow* gl_window) {
    if(glfwGetKey(gl_window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(gl_window, true);
}

int main(int argc, char **argv) {

    // Initialize the glfw3 library.
    if (!glfwInit())
        return -1;
    
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* gl_window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);
    if (!gl_window) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(gl_window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // Change view port
    glViewport(0, 0, 800, 600);
    glfwSetFramebufferSizeCallback(gl_window, FrameBufferSizeChangedCB);

    std::string running_path = getcwd(nullptr, 0);

    // Build and compile our shader program.
    opengl::Program *program = opengl::Program::Create();
    {
        running_path += "/resource/";
        opengl::Shader vertex_shader((running_path + "shader/texture.vs").c_str(), opengl::VERTEX_SHADER);
        opengl::Shader fragment_shader((running_path + "shader/texture.fs").c_str(), opengl::FRAGMENT_SHADER);

        program->AttachShader(&vertex_shader);
        program->AttachShader(&fragment_shader);
    }
    if (!program->Link())
        return -1;

    // Set up vertex data (and buffer(s)) and configure vertex attributes
    GLint  error_code;
    GLchar log_info[1024];
    // +---+---+---+---+---+---+---+---+
    // | X | Y | Z | R | G | B | S | T |  <- Memory arrangement
    // +---+---+---+---+---+---+---+---+
    GLfloat vertices[] = {
        // positions        // colors         // texture coords
         0.5f,  0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, // top right
         0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, // button right
        -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, // top left
        -0.5f,  0.5f, 0.0f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f, // button left
    };
    // Note that we start from 0!
    GLuint indices[] = {
        0, 1, 3,  // top triangle
        1, 2, 3
    };
    GLuint VBO, VAO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    // Bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    // Position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // Color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    // Texture attribute
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    GLuint texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    // Set texture warpping parameters.
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // Set texture filtering parameters.
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // Load texture
    int texture_width, texture_height, nr_channels;
    stbi_set_flip_vertically_on_load(true);
    GLubyte *data = stbi_load((running_path + "texture/wall.jpg").c_str(), &texture_width, &texture_height, &nr_channels, 0);
    if (!data) {
        fprintf(stdout, "[Error] Fail to load texture!");
        return -1;
    } else {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, texture_width, texture_height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    stbi_image_free(data);
    program->SetParam1("texture_sampler", 0);

    // Check whether the GLFW is required to exit.
    while (!glfwWindowShouldClose(gl_window)) {
        // Process Input
        ProcessInput(gl_window);

        // Render 
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture);

        // Use program
        program->Use();
        // Seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        // Check and call the event, swapping the buffer.
        glfwPollEvents();
        glfwSwapBuffers(gl_window);
    }

    // Optional: de-allocate all resources once they've outlived their purpose:
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);

    delete program;

    // Terminate, clearing all previously allocated GLFW resources.
    glfwTerminate();
    return 0;
}