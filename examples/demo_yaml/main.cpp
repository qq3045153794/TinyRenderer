#include <fstream>
#include <iostream>
#include <string>

#include "yaml-cpp/yaml.h"

int main() {
  YAML::Emitter out;
  out << YAML::BeginMap;
  out << YAML::Key << "name";
  out << YAML::Value << "Barack Obama";
  out << YAML::Key << "children";
  out << YAML::Value << YAML::BeginSeq << "Sasha"
      << "Malia" << YAML::EndSeq;
  out << YAML::EndMap;

  std::ofstream outfile("out.yml");
  outfile << out.c_str();
  outfile.close();

  return 0;
}
