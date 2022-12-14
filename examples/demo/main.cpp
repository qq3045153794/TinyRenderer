#include "core/App.h"

int main() {
  App::instand().init();
  bool run = true;
  while (run) {
    App::instand().render_update();
    App::instand().event_update();
  }
}