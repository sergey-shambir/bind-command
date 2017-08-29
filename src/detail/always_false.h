#pragma once
#include <type_traits>

namespace detail
{
template<class T>
constexpr bool always_false = !std::is_same_v<T, T>;
}
