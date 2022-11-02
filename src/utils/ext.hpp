#ifndef _src_utils_ext_HPP_
#define _src_utils_ext_HPP_

#include <algorithm>
#include <iterator>
#include <string_view>
#include <type_traits>

namespace utils {
template <typename T>
constexpr auto type_name() noexcept {
  #ifdef _MSC_VER
    std::string_view p = __FUNCSIG__;
    return std::string_view(p.data() + 84, p.size() - 84 - 7);
  #elif defined(__GNUC__)
    std::string_view p = __PRETTY_FUNCTION__;
    return std::string_view(p.data() + 49, p.find(';', 49) - 49);
    #endif
}
}  // namespace utils

#endif