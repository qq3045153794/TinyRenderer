#ifndef _SRC_ASSET_ASSERT_H_
#define _SRC_ASSET_ASSERT_H_
#include <memory>

#include "glad/glad.h"

namespace asset {

class Asset {
 public:
  Asset() : id(0) {}
  Asset(const Asset&) = delete;
  Asset& operator=(const Asset&) = delete;
  virtual ~Asset(){};
  virtual void bind() const = 0;
  virtual void ubind() const = 0;
  GLuint get_id() { return id; }

 protected:
  GLuint id;
};

}  // namespace asset
#endif
