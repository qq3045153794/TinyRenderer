#ifndef _SRC_MANAGE_ASSET_MANAGE_H_
#define _SRC_MANAGE_ASSET_MANAGE_H_
#include <manage/BaseManage.h>

namespace saber {

class AssetManage : public BaseManage {
 public:
  virtual void Import(const std::string& import_path) override;
 private:
};

}  // namespace saber

#endif
