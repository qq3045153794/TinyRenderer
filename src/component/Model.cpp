#include "component/Model.h"
#include "core/Log.h"
#include <chrono>
namespace component {

Model::Model(const std::string& file_path, Quality quality) {
  // clang-format off
  u_int32_t improt_options = static_cast<unsigned int>(quality) 
    | aiProcess_FlipUVs 
    | aiProcess_Triangulate 
    | aiProcess_GenSmoothNormals 
    | aiProcess_FindInvalidData 
    | aiProcess_ValidateDataStructure 
    | aiProcess_CalcTangentSpace 
    | aiProcess_LimitBoneWeights;
  // clang-format on
  
  Assimp::Importer importer;

  auto start_time = std::chrono::high_resolution_clock::now();


  auto end_time = std::chrono::high_resolution_clock::now();
  
  std::chrono::duration<double, std::milli> loading_time = end_time - start_time;

  
}

}  // namespace component