#include "core/App.h"
#include "core/Log.h"

int main() {
  core::App::instand().init();
  bool run = true;
  while (run) {
    core::App::instand().render_update();
    core::App::instand().event_update();
  }
}