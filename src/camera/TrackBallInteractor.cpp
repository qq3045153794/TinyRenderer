#include "TrackBallInteractor.h"

namespace gl_cameras {

const glm::vec3 TrackBallInteractor::X(1.0f, 0.0, 0.0);
const glm::vec3 TrackBallInteractor::Y(0.0f, 1.0, 0.0);
const glm::vec3 TrackBallInteractor::Z(0.0f, 0.0, 1.0);

TrackBallInteractor::TrackBallInteractor()
    : rotation_(1.0f, 0.0f, 0.0f, 0.0f),
      rotation_sum_(1.0f, 0.0f, 0.0f, 0.0f),
      is_left_click_(false),
      is_dragging_(false),
      speed_(0.25f),
      height_(1.0f),
      width_(1.0f) { }



// https://www.opengl.org/wiki/Object_Mouse_Trackball
void TrackBallInteractor::compute_point_on_sphere(const glm::vec2 &point, 
                                                  glm::vec3& result) {
  GLfloat x = (2.0f * point.x - width_) / width_;
  GLfloat y = (height_ - 2.0f * point.y) / height_;

  GLfloat lenght2 = x * x + y * y;

  if(lenght2 <= 0.5) {
    result.z = std::sqrt(1.0 - lenght2);
  } else {
    result.z = 0.5 / std::sqrt(lenght2);
  }               

  GLfloat norm = 1.0 / sqrt(lenght2 + result.z * result.z);

  result.x = x * norm;
  result.y = y * norm;
  result.z = result.z * norm;
}

void TrackBallInteractor
  ::compute_rotation_between_vectors(const glm::vec3& u, 
                                     const glm::vec3 & v, 
                                    glm::quat& result) {
  GLfloat cos_theta = glm::dot(u,v);
  glm::vec3 rotation_axis;
  const GLfloat& kEpsiLon = 1.0e-5f;
  if(cos_theta < -1.0f + kEpsiLon) {
    rotation_axis = glm::cross(glm::vec3(0.0f, 0.0f, 1.0f), u);
    if(glm::length2(rotation_axis) < 0.01) {
      rotation_axis = glm::cross(glm::vec3(1.0f, 0.0f, 0.0f), u);
    }
    rotation_axis = glm::normalize(rotation_axis);
    result = glm::angleAxis(180.0f, rotation_axis);
  } else if (cos_theta > 1.0f -kEpsiLon) {
    result = glm::quat(1, 0, 0, 0);
  } else {
    GLfloat theta = acos(cos_theta);
    rotation_axis = glm::cross(u, v);

    rotation_axis = glm::normalize(rotation_axis);
    result = glm::angleAxis(theta * speed_, rotation_axis);

  }
}

void TrackBallInteractor::drag() {
  if(prev_click_point_ == click_point_) {
    return;
  }

  compute_point_on_sphere(click_point_, stop_vector_);
  compute_rotation_between_vectors(start_vector_, stop_vector_, rotation_);
  rotation_ = glm::inverse(rotation_);
  drag_arc();
}

void TrackBallInteractor::drag_arc() {
  rotation_sum_ = rotation_sum_ * rotation_;
  update_camera_eye_up(true, true);
}

void TrackBallInteractor::update_camera_eye_up(bool is_eye, bool is_up) {
  if(is_eye) {
    glm::vec3 eye;
    compute_camera_eye(eye);
    camera_->set_eye(eye);
  }

  if(is_up) {
    glm::vec3 up;
    compute_camera_up(up);
    camera_->set_up(up);
  }
}

void TrackBallInteractor::compute_camera_eye(glm::vec3& eye) {
  glm::vec3 orientation = rotation_sum_ * Z;
  eye = translate_length_ * orientation + camera_->get_center();
}

void TrackBallInteractor::compute_camera_up(glm::vec3& up) {
  up = glm::normalize(rotation_sum_ * Y);
}

void TrackBallInteractor::update() {
  const bool is_click = is_left_click_;
  if(!is_dragging_) {
    if(is_click) {
      is_dragging_ =true;
      compute_point_on_sphere(click_point_, start_vector_);
    }
  } else {
    if(is_click) {
      drag();
    } else {
      is_dragging_ = false;
    }
  }
}

void TrackBallInteractor::freeze_transform() {
  if(camera_) {
    rotation_sum_ = glm::inverse(glm::quat(camera_->get_view_mat()));
    translate_length_ = glm::length(camera_->get_eye() - camera_->get_center());
  }
}

void TrackBallInteractor::set_camera(Camera* c) {
  camera_ = c;
  freeze_transform();
}

void TrackBallInteractor::set_click_point(GLfloat x, GLfloat y) {
  prev_click_point_ = click_point_;
  click_point_ = glm::vec2(x, y);
}

void TrackBallInteractor::set_screen_size(GLfloat width, GLfloat height) {
  width_ = width;
  height_ = height;
}

void TrackBallInteractor::set_is_left_click(bool is_left_click) {
  is_left_click_ = is_left_click;
}

}