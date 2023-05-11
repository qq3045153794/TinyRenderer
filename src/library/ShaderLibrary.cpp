#include <asset/Shader.h>
#include <library/ShaderLibrary.h>
namespace saber {

Library<Shader>::Library() {
  Add("default", std::make_shared<Shader>("../resource/shader/shader.vs", "../resource/shader/shader.fs"));
  Add("skybox", std::make_shared<Shader>("../resource/shader/skybox.vs", "../resource/shader/skybox.fs"));
  Add("pbr", std::make_shared<Shader>("../resource/shader/pbr.vs", "../resource/shader/pbr.fs"));

  Add("irradian", std::make_shared<Shader>("../resource/shader/irradian.vs", "../resource/shader/irradian.fs"));
  Add("prefiler", std::make_shared<Shader>("../resource/shader/prefiler.vs", "../resource/shader/prefilter.fs"));
  Add("BRDF_LUT", std::make_shared<Shader>("../resource/shader/BRDF_LUT.vs", "../resource/shader/BRDF_LUT.fs"));
  Add("Shadow Map", std::make_shared<Shader>("../resource/shader/shadow.vs", "../resource/shader/shadow.fs"));
  Add("Shadow Debug", std::make_shared<Shader>("../resource/shader/shadow_debug.vs", "../resource/shader/shadow_debug.fs"));
}

std::shared_ptr<Shader> Library<Shader>::GetShadowShader() { return m_library["Shadow Map"]; }

std::shared_ptr<Shader> Library<Shader>::GetDefaultShader() { return m_library["default"]; }

std::shared_ptr<Shader> Library<Shader>::GetPbrShader() { return m_library["pbr"]; }

std::shared_ptr<Shader> Library<Shader>::GetSkyBoxShader() { return m_library["skybox"]; }

}  // namespace saber
