#ifndef _LEARN_OPENGL_SRC_CAMERA_TRACKBALLINTERATOR_H_
#define _LEARN_OPENGL_SRC_CAMERA_TRACKBALLINTERATOR_H_

#define GLM_ENABLE_EXPERIMENTAL
#include "glm/gtc/constants.hpp" // pi
#include "glm/gtc/type_ptr.hpp"
#include "glm/gtx/norm.hpp"
#include "glm/vec3.hpp"
#include "glm/gtc/quaternion.hpp"

#include "ResourceManager.h"
#include "Camera.h"

namespace gl_cameras
{

  class TrackBallInteractor
  {
  public:
    TrackBallInteractor();
    ~TrackBallInteractor() {};
    void update();
    void set_click_point(GLfloat x, GLfloat y);
    void set_screen_size(GLfloat width, GLfloat height);
    void set_camera(Camera* c);
    void set_is_left_click(bool is_left_click);
  private:
    void compute_point_on_sphere(const glm::vec2 &point, glm::vec3 &result);
    void compute_rotation_between_vectors(const glm::vec3 &u, const glm::vec3 &v, glm::quat &result);
    void drag();
    void drag_arc();
    void update_camera_eye_up(bool is_eye, bool is_up);
    void compute_camera_eye(glm::vec3& eye); 
    void compute_camera_up(glm::vec3& up);
    void freeze_transform();
    static const glm::vec3 X, Y, Z;
    Camera* camera_;

    glm::vec2 click_point_;
    glm::vec2 prev_click_point_;
    glm::vec3 start_vector_;
    glm::vec3 stop_vector_;

    glm::quat rotation_;
    glm::quat rotation_sum_;
    GLfloat translate_length_;
    GLboolean is_left_click_;
    GLboolean is_dragging_;
    GLfloat speed_;
    GLfloat height_;
    GLfloat width_;
  };

}

#endif