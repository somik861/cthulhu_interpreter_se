#pragma once

#include "misc/type_traits.hpp"
#include "program/dict.hpp"
#include "program/instr.hpp"
#include "program/instr_fast.hpp"
#include "program/word.hpp"

#include <tuple>
#include <vector>

namespace cthu::program {
namespace details {
// reject all by default
template <typename T>
struct is_supported_in_stack : public std::false_type {};

// allow pointers
template <typename ptr_t>
struct is_supported_in_stack<ptr_t*> : public std::true_type {};

// allow InstrFast
template <>
struct is_supported_in_stack<InstrFast> : public std::true_type {};

// allow sizeof(elem) <= 7
template <typename T>
    requires(sizeof(T) <= 7)
struct is_supported_in_stack<T> : public std::true_type {};

} // namespace details

class Stack {
  private:
    // specify supported types below
    using m_supported_types = std::tuple<Stack*, Dict*, Instr*, Word, InstrFast>;

    // automatic tests and helper usings
    using m_plain_types = misc::traits::to_plain_tuple_t<m_supported_types>;
    using m_owning_types = misc::traits::to_owning_tuple_t<m_supported_types>;
    // check size(m_supported_types) < 8
    static_assert(std::tuple_size_v<m_supported_types> < 8, "There is too much types for the stack to optimize");
    // check that each element is either a pointer, InstrFast, or sizeof(elem) <= 7
    static_assert(misc::traits::all_of_tuple_v<m_supported_types, details::is_supported_in_stack>,
                  "Some type is not a pointer/InstrFast and its size is larger than 7");

  public:
    template <typename T>
        requires misc::traits::is_any_of_tuple_v<T, m_plain_types>
    constexpr auto peek() noexcept {
        using return_t =
            misc::traits::type_of_tuple_t<misc::traits::idx_of_tuple_v<T, m_plain_types>, m_supported_types>;
    };

    template <typename T>
        requires misc::traits::is_any_of_tuple_v<T, m_plain_types>
    constexpr auto pop() {
        using supported_t =
            misc::traits::type_of_tuple_t<misc::traits::idx_of_tuple_v<T, m_plain_types>, m_supported_types>;
    };

    template <typename T>
        requires misc::traits::is_any_of_tuple_v<T, m_owning_types>
    constexpr void push(T item){};

    template <typename T>
        requires misc::traits::is_any_of_tuple_v<T, m_plain_types>
    constexpr bool checkTop() noexcept {
        return false;
    }

    constexpr std::size_t size() const noexcept { return m_storage.size(); }
    constexpr bool empty() const noexcept { return m_storage.empty(); }
    constexpr void clear() {}
    constexpr std::string toShortString(bool is_on_top = true) { return ""; }
    constexpr std::string toJson(std::size_t indent = 0) { return ""; }

  private:
    template <typename T>
        requires misc::traits::is_any_of_tuple_v<T, m_supported_types>
    constexpr auto getTopConverted() noexcept {}

    std::vector<uint64_t> m_storage;
};
} // namespace cthu::program
