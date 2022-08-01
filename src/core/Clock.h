#ifndef _LEARN_OPENGL_SRC_CORE_CLOCK_H_
#define _LEARN_OPENGL_SRC_CORE_CLOCK_H_


class Clock {
 public:
  Clock();
  ~Clock();
  static inline float dt = 0.0;
  static inline float this_frame = 0.0;
  static inline float last_frame = 0.0;
  
  static void update();
 private:
  
};

#endif