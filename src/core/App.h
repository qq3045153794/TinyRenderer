#ifndef _LEARN_OPENGL_SRC_CORE_APP_H_
#define _LEARN_OPENGL_SRC_CORE_APP_H_
class App {
 public:
  static App& instand();
  void init();
  void run();
  static bool m_app_run;
 private:
  App();
  App(const App&) = delete;
  App& operator=(const App&) = delete;
  ~App();
};

#endif