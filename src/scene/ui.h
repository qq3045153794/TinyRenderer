#ifndef _SRC_SCENE_UI_H_
#define _SRC_SCENE_UI_H_

#include "core/Window.h"
#include "scene/Entity.hpp"

namespace scene::ui {

enum Gizmo { NONE, TRANSLATE, ROTATE, SCALE, BOUNDS };

void init();

void new_frame();

void end_frame();

void show_down();

void draw_menu_bar();

void draw_Gizmo(Entity& camera, Entity& target, Gizmo z);

}  // namespace scene::ui

#endif