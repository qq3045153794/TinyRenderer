#include <library/FBOLibrary.h>
#include <library/TextureLibrary.h>

namespace saber {

Library<::asset::FBO>::Library() {
  Add("Main FBO", std::make_shared<::asset::FBO>(1024, 576));
  m_library["Main FBO"]->set_color_texture();
  m_library["Main FBO"]->set_depth_texture();

  Add("Shadow FBO", std::make_shared<::asset::FBO>(1024, 1024));
  auto shadow_map = PublicSingleton<Library<::asset::Texture>>::GetInstance().Get("shadow_map");
  m_library["Shadow FBO"]->render_pass_texture(shadow_map);

  Add("Debug Shader FBO", std::make_shared<::asset::FBO>(1024, 576));
  m_library["Debug Shader FBO"]->set_color_texture();
  m_library["Debug Shader FBO"]->set_depth_texture();
}

std::shared_ptr<asset::FBO> Library<::asset::FBO>::GetMainFBO() {
  return m_library["Main FBO"];
}

std::shared_ptr<asset::FBO> Library<::asset::FBO>::GetShadowFBO() {
  return m_library["Shadow FBO"];
}

std::shared_ptr<asset::FBO> Library<::asset::FBO>::GetDeBugShadowFBO() {
  return m_library["Debug Shader FBO"];
}

}
