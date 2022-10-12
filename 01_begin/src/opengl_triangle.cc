#include "header/program.h"
#include "header/shader.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <cmath>
#include <iostream>
#include <string>
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

    // Build and compile our shader program.
    opengl::Program *program = opengl::Program::create();
    {
        std::string running_path = getcwd(nullptr, 0);
        running_path += "/resource/shader/";
        opengl::Shader vertex_shader((running_path + "triangle.vs").c_str(), opengl::VERTEX_SHADER);
        opengl::Shader fragment_shader((running_path + "triangle.fs").c_str(), opengl::FRAGMENT_SHADER);

        program->attachShader(&vertex_shader);
        program->attachShader(&fragment_shader);
    }
    if (!program->link())
        return -1;

    // Set up vertex data (and buffer(s)) and configure vertex attributes
    GLint error_code;
    GLchar log_info[1024];
    GLfloat vertices[] = {
         0.0f,  0.5f, 0.0f, 1.0f, 0.0f, 0.0f,
        -0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f,
         0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f
    };
    // Note that we start from 0!
    GLuint indices[] = {
        0, 1, 2,  // triangle
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

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // Note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
    glBindBuffer(GL_ARRAY_BUFFER, 0); 

    // Remember: do NOT unbind the EBO while a VAO is active as the bound element buffer object IS stored in the VAO; keep the EBO bound.
    // glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    // You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
    // VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
    glBindVertexArray(0);

    // Check whether the GLFW is required to exit.
    while (!glfwWindowShouldClose(gl_window)) {
        // Process Input
        ProcessInput(gl_window);

        // Render background
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // Use program.
        program->use();

        // Seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized
        glBindVertexArray(VAO);
        // glDrawArrays(GL_TRIANGLES, 0, 3);
        glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0);
        // No need to unbind it every time
        glBindVertexArray(0);

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