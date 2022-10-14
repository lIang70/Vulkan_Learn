#include "camera.h"

#include <glm/gtc/matrix_transform.hpp>

namespace opengl {

Camera::Camera(glm::vec3 position, glm::vec3 up, float yaw, float pitch)
    : position_(position), up_(up), yaw_(yaw), pitch_(pitch),
      front_(glm::vec3(0.0f, 0.0f, -1.0f)), move_speed_(SPEED), 
      mouse_sensitivity_(SENSITIVITY), zoom_(ZOOM) {
    update();
}

glm::mat4 
Camera::viewMatrix() {
    return glm::lookAt(position_, position_ + front_, up_);
}

void
Camera::processKeyboard(MoveDirection direction, float delta_time) {
    float velocity = move_speed_ * delta_time;
    if (direction == FORWARD)
        position_ += front_ * velocity;
    if (direction == BACKWARD)
        position_ -= front_ * velocity;
    if (direction == LEFT)
        position_ -= right_ * velocity;
    if (direction == RIGHT)
        position_ += right_ * velocity;
}

void
Camera::processMouseMove(float x_offset, float y_offset, bool constrain_pitch) {
    x_offset *= mouse_sensitivity_;
    y_offset *= mouse_sensitivity_;

    yaw_    += x_offset;
    pitch_  += y_offset;

    // make sure that when pitch is out of bounds, screen doesn't get flipped
    if (constrain_pitch) {
        if (pitch_ > 89.0f)
            pitch_ = 89.0f;
        if (pitch_ < -89.0f)
            pitch_ = -89.0f;
    }

    // update Front, Right and Up Vectors using the updated Euler angles
    update();
}

void 
Camera::processMouseScroll(float y_offset) {
    zoom_ -= (float)y_offset;
    if (zoom_ < 1.0f)
        zoom_ = 1.0f;
    if (zoom_ > 45.0f)
        zoom_ = 45.0f; 
}

void
Camera::update() {
    // calculate the new front vector
    glm::vec3 front;
    front.x = cos(glm::radians(yaw_)) * cos(glm::radians(pitch_));
    front.y = sin(glm::radians(pitch_));
    front.z = sin(glm::radians(yaw_)) * cos(glm::radians(pitch_));
    front_  = glm::normalize(front);
    // also re-calculate the Right and Up vector
    right_  = glm::normalize(glm::cross(front_, world_up_));  // normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
    up_     = glm::normalize(glm::cross(right_, front_));
}

}