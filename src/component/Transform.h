/*
 * @Author       : xietao
 * @Date         : 2022-07-28 16:41:23
 * @LastEditors  : xietao
 * @LastEditTime : 2022-12-20 14:57:46
 * @FilePath     : /Learn_OpenGL/src/component/Transform.h
 * @Description  : 变换组件 设置位置、旋转和大小 也能获取物体欧拉角
 *                 位置、欧拉角(Y -> X -> Z)、lookat、变换矩阵
 *                 物体变换顺序： 缩放->旋转->位移
 *                 debug: https://www.andre-gaschler.com/rotationconverter/
 * Copyright (c) 2022, All Rights Reserved.
 */

#ifndef _LEARN_OPENGL_SRC_COMPONENT_TRANSFORM_H_
#define _LEARN_OPENGL_SRC_COMPONENT_TRANSFORM_H_

#define GLM_ENABLE_EXPERIMENTAL
#include "glm/glm.hpp"
#include "glm/gtc/type_ptr.hpp"

namespace component {

const glm::vec3 world_right{1.0f, 0.0f, 0.0f};     // +x axis
const glm::vec3 world_up{0.0f, 1.0f, 0.0f};        // +y axis
const glm::vec3 world_forward{0.0f, 0.0f, -1.0f};  // -z axis
class Transform {
 public:
  /**
   * @description : 绕轴旋转
   * @param        {vec3&} axis 过圆心的旋转轴
   * @param        {float} angle 旋转角度 角度制
   * @return       {*}
   */
  void rotation(const glm::vec3& axis, float angle);

  /**
   * @description : 旋转多少四元数
   * @param        {quat&} q 四元数
   * @return       {*}
   */
  void rotation(const glm::quat& q);

  /**
   * @description : 旋转多少欧拉角 顺序Y->Z->X
   * @param        {vec3&} eular 欧拉角
   * @return       {*}
   */
  void rotation(const glm::vec3& eular);

  /**
   * @description : 平移
   * @param        {vec3&} v 平移向量
   * @return       {*}
   */
  void translate(const glm::vec3& v);

  /**
   * @description : 缩放
   * @param        {vec3&} s 缩放向量
   * @return       {*}
   */
  void scale(const glm::vec3& s);

  /**
   * @description : 设置欧拉角 顺序Y->Z->X
   * @param        {vec3&} eular 欧拉角
   * @return       {*}
   */
  void set_ealar_YZX(const glm::vec3& eular);

  /**
   * @description : 设置四元数
   * @param        {quat&} q 四元数
   * @return       {*}
   */
  void set_rotation(const glm::quat& q);

  /**
   * @description : 设置位移
   * @param        {vec3&} position 位移向量
   * @return       {*}
   */
  void set_position(const glm::vec3& position);

  /**
   * @description : 设置缩放
   * @param        {vec3&} size 缩放向量
   * @return       {*}
   */
  void set_scale(const glm::vec3& size);

  /**
   * @description : 获取物体的变换矩阵
   * @return       {*} 物体的变换矩阵
   */
  const glm::mat4& get_transform() const;

  /**
   * @description : 获取物体的欧拉角 Y->Z->X
   * @return       {*} 欧拉角
   */
  const glm::vec3& get_eular() const;

  /**
   * @description : 获取位置向量
   * @return       {*} 位置向量
   */
  const glm::vec3& get_position() const;

  /**
   * @description : 获取物体lookat矩阵
   * @return       {*} lookat矩阵
   */
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