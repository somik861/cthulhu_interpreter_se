#pragma once

#include <memory>
#include <type_traits>

namespace utils::details {
template <typename T>
struct is_unique_ptr : std::false_type {};

template <typename T>
struct is_unique_ptr<std::unique_ptr<T>> : std::true_type {};

template <typename T>
constexpr bool is_unique_ptr_v = is_unique_ptr<T>::value;
} // namespace utils::details
