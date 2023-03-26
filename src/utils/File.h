#ifndef _LEARN_OPENGL_SRC_UTILS_FILE_H_
#define _LEARN_OPENGL_SRC_UTILS_FILE_H_

#include <string>
#include <fstream>
#include <iostream>
#include <sstream>
#include "yaml-cpp/yaml.h"
namespace utils {

class File {
 public:
  static std::string read_file(const std::string& flie_path);
  static void write_yml_file(const std::string& flie_path, YAML::Emitter& out);
  static void parser_yml_file(const std::string& file_path, YAML::Node& doc);
};

}  // namespace utils

#endif
