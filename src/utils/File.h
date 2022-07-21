#ifndef _LEARN_OPENGL_SRC_UTILS_FILE_H_
#define _LEARN_OPENGL_SRC_UTILS_FILE_H_

#include <string>
namespace utils {

class File {
 public:
  static std::string read_file(const std::string& flie_path);
};

}  // namespace utils

#endif