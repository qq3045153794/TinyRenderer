#include <library/UBOLibrary.h>
#include <asset/Buffer.h>

using UBO = ::asset::UBO;
namespace saber {
Library<UBO>::Library() {
  {
    // camera ubo
    std::vector<GLuint> offset = {0U, 64U, 128U, 144U};
    std::vector<GLuint> lenght = {64U, 64U, 16U, 16U};
    GLuint sz = 160U;
    Add("CameraData", std::make_shared<UBO>(offset, lenght, sz));
    m_library["CameraData"]->set_uid(0U);
  }
}
}

