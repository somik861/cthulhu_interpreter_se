#pragma once

#include <memory>
#include <tuple>
#include <type_traits>

namespace common::traits {
// all_of_tuple
template <typename tuple_t, template <typename T> typename cond_t>
struct all_of_tuple;

template <typename... types_t, template <typename T> typename cond_t>
struct all_of_tuple<std::tuple<types_t...>, cond_t>
    : public std::conjunction<cond_t<types_t>...> {};

template <typename tuple_t, template <typename T> typename cond_t>
constexpr bool all_of_tuple_v = all_of_tuple<tuple_t, cond_t>::value;

// ptr to uptr
template <typename ptr_t>
struct ptr_to_uptr {
	static_assert(std::is_pointer_v<ptr_t>, "Type is not a pointer");
	using type = std::unique_ptr<std::remove_pointer_t<ptr_t>>;
};
template <typename ptr_t>
using ptr_to_uptr_t = ptr_to_uptr<ptr_t>::type;

// is unique ptr
template <typename T>
struct is_unique_ptr : public std::false_type {};

template <typename T, typename D>
struct is_unique_ptr<std::unique_ptr<T, D>> : public std::true_type {};

template <typename T>
constexpr bool is_unique_ptr_v = is_unique_ptr<T>::value;

// to owning type
template <typename T>
struct to_owning_type : public std::type_identity<T> {};
template <typename T>
struct to_owning_type<T*> : public ptr_to_uptr<T*> {};
template <typename T>
using to_owning_type_t = to_owning_type<T>::type;

// to owning tuple
template <typename tuple_t>
struct to_owning_tuple;
template <typename... types_t>
struct to_owning_tuple<std::tuple<types_t...>> {
	using type = std::tuple<to_owning_type_t<types_t>...>;
};
template <typename tuple_t>
using to_owning_tuple_t = to_owning_tuple<tuple_t>::type;

// to raw type
template <typename T>
struct to_raw_type : public std::type_identity<T> {};
template <typename T, typename D>
struct to_raw_type<std::unique_ptr<T, D>> : public std::type_identity<T*> {};
template <typename T>
using to_raw_type_t = to_raw_type<T>::type;

// is any of tuple
template <typename T, typename tuple_t>
struct is_any_of_tuple;
template <typename T, typename... types_t>
struct is_any_of_tuple<T, std::tuple<types_t...>>
    : public std::disjunction<std::is_same<T, types_t>...> {};
template <typename T, typename tuple_t>
constexpr bool is_any_of_tuple_v = is_any_of_tuple<T, tuple_t>::value;

// type at idx in tuple
template <std::size_t idx, typename tuple_t>
struct type_of_tuple : public std::tuple_element<idx, tuple_t> {};
template <std::size_t idx, typename tuple_t>
using type_of_tuple_t = type_of_tuple<idx, tuple_t>::type;

// idx of type in tuple
template <typename T, typename tuple_t>
struct idx_of_tuple;

namespace details {
template <std::size_t I, typename T, typename first_t, typename... rest_t>
struct idx_of_tuple_help : public idx_of_tuple_help<I + 1, T, rest_t...> {};

template <std::size_t I, typename T, typename... rest_t>
struct idx_of_tuple_help<I, T, T, rest_t...>
    : public std::integral_constant<std::size_t, I> {
	static_assert(!std::disjunction_v<std::is_same<T, rest_t>...>,
	              "Searched type appears in the tuple more than one time");
};
} // namespace details
template <typename T, typename... types_t>
struct idx_of_tuple<T, std::tuple<types_t...>>
    : public details::idx_of_tuple_help<0, T, types_t...> {};

template <typename T, typename tuple_t>
constexpr std::size_t idx_of_tuple_v = idx_of_tuple<T, tuple_t>::value;

// is tuple
template <typename T>
struct is_tuple : public std::false_type {};

template <typename... T>
struct is_tuple<std::tuple<T...>> : public std::true_type {};

template <typename T>
constexpr bool is_tuple_v = is_tuple<T>::value;

} // namespace common::traits