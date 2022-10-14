/**
 * @file camera.h
 * @author l1ang70
 * @brief The class that wraps the camera
 * @version 0.1
 * @date 2022-10-12
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#ifndef _OPENGL_CAMERA_H_
#define _OPENGL_CAMERA_H_

#include <glm/glm.hpp>

namespace opengl {

class Camera {
    // Camera Attributes
    glm::vec3 position_;
    glm::vec3 front_;
    glm::vec3 up_;
    glm::vec3 right_;
    glm::vec3 world_up_;

    // Euler angles
    float yaw_;
    float pitch_;
    
    // Camera options
    float move_speed_;
    float mouse_sensitivity_;
    float zoom_;

public:
    enum MoveDirection {
        FORWARD,
        BACKWARD,
        LEFT,
        RIGHT
    };

    // Default camera values
    constexpr static float YAW         = -90.0f;
    constexpr static float PITCH       =  0.0f;
    constexpr static float SPEED       =  2.5f;
    constexpr static float SENSITIVITY =  0.1f;
    constexpr static float ZOOM        =  45.0f;

    Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), float yaw = YAW, float pitch = PITCH);

    glm::mat4 viewMatrix();

    // processes input received from any keyboard-like input system. Accepts input parameter in the form of camera defined ENUM (to abstract it from windowing systems).
    void processKeyboard(MoveDirection direction, float delta_time);

    // processes input received from a mouse input system. Expects the offset value in both the x and y direction.
    void processMouseMove(float x_offset, float y_offset, bool constrain_pitch = true);

    // processes input received from a mouse scroll-wheel event. Only requires input on the vertical wheel-axis
    void processMouseScroll(float y_offset);

private:
    void update();

};

}

#endif // !_OPENGL_CAMERA_H_