#include "core/App.h"
#include "core/Log.h"

int main() {
  App::instand().init();
  bool run = true;
  while (run) {
    App::instand().render_update();
    App::instand().event_update();
  }
}