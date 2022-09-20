/*
 * @Author       : xietao
 * @Date         : 2022-07-28 16:41:23
 * @LastEditors  : xietao
 * @LastEditTime : 2022-09-20 20:20:22
 * @FilePath     : /Learn_OpenGL/src/component/Transform.h
 * @Description  : 变换组件 设置位置、旋转和大小 也能获取物体欧拉角
 *                 位置、欧拉角(Y -> X -> Z)、lookat、变换矩阵
 * Copyright (c) 2022, All Rights Reserved.
 */

#ifndef _LEARN_OPENGL_SRC_COMPONENT_TRANSFORM_H_
#define _LEARN_OPENGL_SRC_COMPONENT_TRANSFORM_H_

#define GLM_ENABLE_EXPERIMENTAL
#include "glm/glm.hpp"
#include "glm/gtc/type_ptr.hpp"

namespace component {

class Transform {
 public:
  void rotation(const glm::vec3& axis, float angle);
  void rotation(const glm::quat& q);
  void rotation(const glm::vec3& eular);
  void ealar_ratation(const glm::vec3& eular);

  void translate(const glm::vec3& v);
  void scale(const glm::vec3& s);
  const glm::mat4& get_transform() const;
  const glm::vec3& get_eular() const;
  const glm::vec3& get_position() const;
  const glm::mat4 get_lookat() const;
  Transform();
  ~Transform();

  // 局部坐标
  glm::vec3 m_right, m_up, m_forward;

 private:
  void culate_asix();
  void culate_eular();

  // 变换矩阵
  glm::mat4 m_transform;

  glm::vec3 m_position;
  glm::vec3 m_scale;
  glm::vec3 m_eular;
  glm::quat m_rotation;
};

}  // namespace component

#endif