#ifndef _LEARN_OPENGL_SRC_CAMERA_CAMERA_H_
#define _LEARN_OPENGL_SRC_CAMERA_CAMERA_H_

#include "glad/glad.h"
#include "glm/glm.hpp"
#include "glm/gtc/type_ptr.hpp"
namespace gl_cameras {

class Camera {
 public:
  Camera(float fov, float aspect, float near, float far);
  ~Camera(){};
  const glm::mat4& get_view_mat();
  const glm::mat4& get_projection_mat();
  const glm::vec3& get_eye();
  const glm::vec3& get_center();
  const glm::vec3& get_up();

  void set_eye(const glm::vec3& e);
  void set_center(const glm::vec3& c);
  void set_up(const glm::vec3& u);
  void look_at(const glm::vec3& e, const glm::vec3& c, const glm::vec3& u);

 private:
  void update();
  glm::vec3 eye_;
  glm::vec3 center_;
  glm::vec3 up_;

  glm::mat4 view_mat_;
  glm::mat4 projection_mat_;
};
}  // namespace gl_cameras

#endif