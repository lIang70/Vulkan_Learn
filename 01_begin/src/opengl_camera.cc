#define STB_IMAGE_IMPLEMENTATION
#include "header/stb_image.h"
#include "header/program.h"
#include "header/shader.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <cmath>
#include <iostream>
#include <unistd.h>

// settings
const unsigned int g_screen_width  = 800;
const unsigned int g_screen_height = 600;

// camera
glm::vec3 camera_position   = glm::vec3(0.0f, 0.0f,  3.0f);
glm::vec3 camera_front      = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 camera_up         = glm::vec3(0.0f, 1.0f,  0.0f);

// timing
GLdouble g_delta_time = 0.0f;	// time between current frame and last frame
GLdouble g_last_frame = 0.0f;

// mouse
GLdouble fov = 55.0f;

void FrameBufferSizeChangedCB(GLFWwindow* gl_window, GLint width, GLint height) {
    // Change view port
    glViewport(0, 0, width, height);
}

void MouseCB(GLFWwindow* gl_window, GLdouble x_pos, GLdouble y_pos) {
    // mouse global
    static GLboolean    s_first_mouse = true;
    static GLdouble     s_last_x = g_screen_width / 2;
    static GLdouble     s_last_y = g_screen_height / 2;
    static GLdouble     yaw    = 0.0;
    static GLdouble     pitch  = 0.0;

    if (s_first_mouse) {
        s_last_x = x_pos;
        s_last_y = y_pos;
        s_first_mouse = false;
    }
    GLdouble x_offset = x_pos - s_last_x;
    GLdouble y_offset = s_last_y - y_pos; 
    s_last_x = x_pos;
    s_last_y = y_pos;

    GLdouble sensitivity = 0.05;
    x_offset *= sensitivity;
    y_offset *= sensitivity;

    yaw   += x_offset;
    pitch += y_offset;

    if(pitch > 89.0f)
        pitch = 89.0f;
    if(pitch < -89.0f)
        pitch = -89.0f;

    glm::vec3 front;
    front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    front.y = sin(glm::radians(pitch));
    front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    camera_front = glm::normalize(front);
}

void ScrollCB(GLFWwindow *gl_window, GLdouble xoffset, GLdouble yoffset) {
    if (fov >= 1.0f && fov <= 55.0f)
        fov -= yoffset;
    if (fov <= 1.0f)
        fov = 1.0f;
    if (fov >= 55.0f)
        fov = 55.0f;
}

void ProcessInput(GLFWwindow* gl_window) {
    if(glfwGetKey(gl_window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(gl_window, true);
    
    float camera_speed = static_cast<float>(2.5 * g_delta_time);
    if (glfwGetKey(gl_window, GLFW_KEY_W) == GLFW_PRESS)
        camera_position += camera_speed * camera_front;
    if (glfwGetKey(gl_window, GLFW_KEY_S) == GLFW_PRESS)
        camera_position -= camera_speed * camera_front;
    if (glfwGetKey(gl_window, GLFW_KEY_A) == GLFW_PRESS)
        camera_position -= glm::normalize(glm::cross(camera_front, camera_up)) * camera_speed;
    if (glfwGetKey(gl_window, GLFW_KEY_D) == GLFW_PRESS)
        camera_position += glm::normalize(glm::cross(camera_front, camera_up)) * camera_speed;
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

    glfwSetInputMode(gl_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    glfwSetFramebufferSizeCallback(gl_window, FrameBufferSizeChangedCB);
    glfwSetCursorPosCallback(gl_window, MouseCB);
    glfwSetScrollCallback(gl_window, ScrollCB);


    std::string running_path = getcwd(nullptr, 0);

    // configure global opengl state
    glEnable(GL_DEPTH_TEST);

    // Build and compile our shader program.
    opengl::Program *program = opengl::Program::Create();
    {
        running_path += "/resource/";
        opengl::Shader vertex_shader((running_path + "shader/camera.vs").c_str(), opengl::VERTEX_SHADER);
        opengl::Shader fragment_shader((running_path + "shader/camera.fs").c_str(), opengl::FRAGMENT_SHADER);

        program->AttachShader(&vertex_shader);
        program->AttachShader(&fragment_shader);
    }
    if (!program->Link())
        return -1;

    // Set up vertex data (and buffer(s)) and configure vertex attributes
    GLint  error_code;
    GLchar log_info[1024];

    // set up vertex data (and buffer(s)) and configure vertex attributes
    // ------------------------------------------------------------------
    float vertices[] = {
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
         0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
    };

    // world space positions of our cubes
    glm::vec3 cube_positions[] = {
        glm::vec3( 0.0f,  0.0f,  0.0f),
        glm::vec3( 2.0f,  5.0f, -15.0f),
        glm::vec3(-1.5f, -2.2f, -2.5f),
        glm::vec3(-3.8f, -2.0f, -12.3f),
        glm::vec3( 2.4f, -0.4f, -3.5f),
        glm::vec3(-1.7f,  3.0f, -7.5f),
        glm::vec3( 1.3f, -2.0f, -2.5f),
        glm::vec3( 1.5f,  2.0f, -2.5f),
        glm::vec3( 1.5f,  0.2f, -1.5f),
        glm::vec3(-1.3f,  1.0f, -1.5f)
    };

    unsigned int VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // texture coord attribute
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0); 
    glBindVertexArray(0);

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

    program->Use();
    program->SetParam1("texture_sampler", 0);

    // Check whether the GLFW is required to exit.
    while (!glfwWindowShouldClose(gl_window)) {
        // per-frame time logic
        float current_frame = static_cast<float>(glfwGetTime());
        g_delta_time = current_frame - g_last_frame;
        g_last_frame = current_frame;

        // Process Input
        ProcessInput(gl_window);

        // Render 
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // also clear the depth buffer now!

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture);

        // Use program
        program->Use();

        // create transformations
        glm::mat4 view = glm::mat4(1.0f);
        view = glm::lookAt(camera_position, camera_position + camera_front, camera_up);
        // pass transformation matrices to the shader
        program->SetMatrix4("view", view);
        glm::mat4 projection = glm::perspective(glm::radians(fov), (GLdouble)g_screen_width / (GLdouble)g_screen_height, 0.1, 100.0);
        program->SetMatrix4("projection", projection); // note: currently we set the projection matrix each frame, but since the projection matrix rarely changes it's often best practice to set it outside the main loop only once.

        // render boxes
        for (unsigned int i = 0; i < 10; i++) {
            glBindVertexArray(VAO);

            // calculate the model matrix for each object and pass it to shader before drawing
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, cube_positions[i]);
            float angle = 20.0f * i;
            model = glm::rotate(model, (float)glfwGetTime() * glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
            program->SetMatrix4("model", model);

            glDrawArrays(GL_TRIANGLES, 0, 36);
            glBindVertexArray(0);
        }

        // Check and call the event, swapping the buffer.
        glfwPollEvents();
        glfwSwapBuffers(gl_window);
    }

    // Optional: de-allocate all resources once they've outlived their purpose:
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);

    delete program;

    // Terminate, clearing all previously allocated GLFW resources.
    glfwTerminate();
    return 0;
}