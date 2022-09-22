#include "scene/Render.h"

namespace scene {

std::queue<entt::entity> Render::render_queue;

Render::Render() {

}

Render::~Render() {
}

void Render::clear_buffer() {
    glClearColor(0.0f, 0.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
}

void Render::render_scene() {
    auto& reg = curr_scene->registry;
    
    auto mesh_group = reg.group<Mesh>(entt::get<Transform, Tag, Material>);
}

}