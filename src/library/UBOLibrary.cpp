#include <asset/Buffer.h>
#include <library/UBOLibrary.h>

using UBO = ::asset::UBO;
namespace saber {
Library<UBO>::Library() {
  {
    // camera ubo
    std::vector<GLuint> offset = {0U, 64U, 128U, 144U};
    std::vector<GLuint> lenght = {64U, 64U, 16U, 16U};
    GLuint sz = 160U;
    Add("CameraData", std::make_shared<UBO>(offset, lenght, sz));
  }
  {
    // 平行光
    std::vector<GLuint> offset = {0U, 16U, 32U};
    std::vector<GLuint> lenght = {16U, 16U, 4U};
    GLuint sz = 36U;
    Add("DL", std::make_shared<UBO>(offset, lenght, sz));
  }
  {
    // 点光源
    // std::vector<GLuint> offset = {0U, 16U, 32U, 36U, 40U, 44U};
    // std::vector<GLuint> lenght = {16U, 16U, 4U, 4U, 4U, 4U};
    // GLuint sz = 48U;
    // Add("PL", std::make_shared<UBO>(offset, lenght, sz));

    // 最大光照数量
    // 需要和shader里面的对应
    GLuint data_count = 32U;

    std::vector<GLuint> offset = {0U, 16 * data_count, 2 * 16 * data_count, 3 * 16 * data_count, 4 * 16 * data_count, 5 * 16 * data_count};
    std::vector<GLuint> lenght = {16U, 16U, 16U, 16U, 16U, 16U};
    GLuint sz = 16U * 32U * 6U;
    Add("PL", std::make_shared<UBO>(offset, lenght, sz));
  }
  {
    // 相机光
    std::vector<GLuint> offset = {0U, 16U, 32U, 48U, 52U, 56U, 60U};
    std::vector<GLuint> lenght = {16U, 16U, 16U, 4U, 4U, 4U, 4U};
    GLuint sz = 64U;
    Add("SL", std::make_shared<UBO>(offset, lenght, sz));
  }
}
}  // namespace saber
