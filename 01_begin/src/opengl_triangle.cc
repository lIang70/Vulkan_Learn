#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <cmath>
#include <iostream>

const GLchar* g_vertex_shader_source =
    "#version 460 core\n"
    "layout (location = 0) in vec3 position;\n"
    "layout (location = 1) in vec3 color;\n"
    "out vec3 vertex_color;\n"
    "void main() {\n"
    "    gl_Position = vec4(position.x, position.y, position.z, 1.0);\n"
    "    vertex_color = color;\n"
    "}\0";

const GLchar* g_fragment_shader_source = 
    "#version 330 core\n"
    "out vec4 frag_color;\n"
    "in vec3 vertex_color;\n"
    // "uniform vec4 vertex_color;\n"
    "void main() {\n"
    "    frag_color = vec4(vertex_color, 1.0);\n"
    "}\0";

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

    // Build and compile our shader program.
    GLint error_code;
    GLchar log_info[1024];

    // Compile vertex shader
    GLuint vertex_shader;
    vertex_shader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex_shader, 1, &g_vertex_shader_source, NULL);
    glCompileShader(vertex_shader);
    glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &error_code);
    if(!error_code) {
        glGetShaderInfoLog(vertex_shader, 1024, NULL, log_info);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << log_info << std::endl;
    }

    // Compile fragment shader
    GLuint fragment_shader;
    fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment_shader, 1, &g_fragment_shader_source, NULL);
    glCompileShader(fragment_shader);
    glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &error_code);
    if(!error_code) {
        glGetShaderInfoLog(fragment_shader, 1024, NULL, log_info);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << log_info << std::endl;
    }

    // Link shaders
    GLuint shader_program = glCreateProgram();
    glAttachShader(shader_program, vertex_shader);
    glAttachShader(shader_program, fragment_shader);
    glLinkProgram(shader_program);
    // Check for linking errors
    glGetProgramiv(shader_program, GL_LINK_STATUS, &error_code);
    if (!error_code) {
        glGetProgramInfoLog(shader_program, 1024, NULL, log_info);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << log_info << std::endl;
    }
    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);

    // Set up vertex data (and buffer(s)) and configure vertex attributes
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

    // Change view port
    glViewport(0, 0, 800, 600);
    glfwSetFramebufferSizeCallback(gl_window, FrameBufferSizeChangedCB);

    // Check whether the GLFW is required to exit.
    while (!glfwWindowShouldClose(gl_window)) {
        // Process Input
        ProcessInput(gl_window);

        // Render 
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // Draw our first triangle
        glUseProgram(shader_program);
        // Seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized
        glBindVertexArray(VAO);
        // glDrawArrays(GL_TRIANGLES, 0, 3);
        glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0);
        // No need to unbind it every time
        // glBindVertexArray(0);

        // Update color of 
        // float time_value = glfwGetTime();
        // float green_value = sin(time_value) / 2.0f + 0.5f;
        // int vertex_color_location = glGetUniformLocation(shader_program, "vertex_color");
        // glUniform4f(vertex_color_location, 0.0f, green_value, 0.0f, 1.0f);

        // Check and call the event, swapping the buffer.
        glfwPollEvents();
        glfwSwapBuffers(gl_window);
    }

    // Optional: de-allocate all resources once they've outlived their purpose:
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    glDeleteProgram(shader_program);

    // Terminate, clearing all previously allocated GLFW resources.
    glfwTerminate();
    return 0;
}