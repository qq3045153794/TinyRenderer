#include "utils/File.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include "core/Log.h"
namespace utils {
std::string File::read_file(const std::string& file_path) {
  std::string out;
  std::ifstream file;
  CORE_INFO("track");
  file.exceptions(std::ifstream::failbit | std::ifstream::badbit);
  CORE_INFO("track");
  try {
    file.open(file_path);
    std::stringstream file_stream;
    file_stream << file.rdbuf();
    out = file_stream.str();
    file.close();
  } catch(std::ifstream::failure& e) {
    file.close();
    CORE_ERROR("ERROR::SHADER::FILE_NO_SUCCESFULLY_READ");
  } catch(std::exception& e) {
    file.close();
    CORE_ERROR("Read file failure..");
  }
  return out;
}

}  // namespace utils
