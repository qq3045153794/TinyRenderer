#ifndef _SRC_CORE_PUBLIC_SINGLETON_H_
#define _SRC_CORE_PUBLIC_SINGLETON_H_
#include <iostream>
namespace saber {

template <typename T>
class PublicSingleton {
 protected:
  PublicSingleton() = default;

 public:
  [[nodiscard]] static T& GetInstance() noexcept(
      std::is_nothrow_constructible<T>::value) {
    static T instance;
    return instance;
  }
  virtual ~PublicSingleton() noexcept = default;
  PublicSingleton(const PublicSingleton&) = delete;
  PublicSingleton& operator=(const PublicSingleton&) = delete;
};

}
#endif
