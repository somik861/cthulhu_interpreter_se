#pragma once

#include "misc/type_traits.hpp"
#include "program/fast_instruction.hpp"
#include "program/instruction.hpp"
#include "program/word.hpp"

#include <cassert>
#include <format>
#include <string>
#include <tuple>
#include <vector>

namespace cthu::program {
class Dict;

namespace details {
// reject all by default
template <typename T>
struct is_supported_in_stack : public std::false_type {};

// allow pointers
template <typename ptr_t>
struct is_supported_in_stack<ptr_t*> : public std::true_type {};

// allow FastInstruction
template <>
struct is_supported_in_stack<FastInstruction> : public std::true_type {};

// allow sizeof(elem) <= 7
template <typename T>
    requires(sizeof(T) <= 7)
struct is_supported_in_stack<T> : public std::true_type {};

} // namespace details

class Stack {
  protected:
    // specify supported types below
    using m_supported_types = std::tuple<Stack*, Dict*, Instruction*, Word, FastInstruction>;

    // automatic tests and helper usings
    using m_plain_types = misc::traits::to_plain_tuple_t<m_supported_types>;
    using m_owning_types = misc::traits::to_owning_tuple_t<m_supported_types>;
    // check size(m_supported_types) < 8
    static_assert(std::tuple_size_v<m_supported_types> < 8, "There is too much types for the stack to optimize");
    // check that each element is either a pointer, FastInstruction, or sizeof(elem) <= 7
    static_assert(misc::traits::all_of_tuple_v<m_supported_types, details::is_supported_in_stack>,
                  "Some type is not a pointer/FastInstruction and its size is larger than 7");
    // check that pointers are 64 bits (yea, we do not support 32bit architectures)
    static_assert(sizeof(void*) == 8, "non 64-bit architectures are not supported");

  public:
    constexpr Stack() noexcept = default;
    constexpr Stack(const Stack& o)
        : Stack(o.cloneStorage()) {}
    constexpr Stack(Stack&&) noexcept = default;
    constexpr Stack& operator=(const Stack& o) {
        clear();
        m_storage = o.cloneStorage();
        return *this;
    }
    constexpr Stack& operator=(Stack&& o) noexcept {
        clear();
        m_storage = std::move(o.m_storage);
        return *this;
    };
    constexpr ~Stack() noexcept { clear(); }

    template <typename T>
        requires misc::traits::is_any_of_tuple_v<T, m_plain_types>
    constexpr auto peek() const noexcept {
        return decryptItem<T>(m_storage.back());
    };

    template <typename T>
        requires misc::traits::is_any_of_tuple_v<T, m_plain_types>
    constexpr auto pop() {
        auto data = peek<T>();
        m_storage.pop_back();
        if constexpr (std::is_pointer_v<decltype(data)>)
            return std::unique_ptr<T>(data);
        else
            return data;
    };

    template <typename T>
        requires misc::traits::is_any_of_tuple_v<T, m_owning_types>
    constexpr void push(T item) {
        uint64_t encrypted;

        if constexpr (misc::traits::is_unique_ptr_v<T>)
            encrypted = encryptItem(item.release());
        else
            encrypted = encryptItem(item);

        m_storage.emplace_back(encrypted);
    };

    template <typename T>
        requires misc::traits::is_any_of_tuple_v<T, m_plain_types>
    constexpr bool checkTop() const noexcept {
        return checkItem<T>(m_storage.back());
    }

    constexpr std::size_t size() const noexcept { return m_storage.size(); }
    constexpr bool empty() const noexcept { return m_storage.empty(); }
    constexpr void clear() {
        while (!empty())
            popAndDeleteTop();
    }
    std::string toShortString(bool is_on_top = true) const;
    std::string toJson(std::size_t indent = 0) const;

  protected:
    constexpr Stack(std::vector<uint64_t> storage) noexcept
        : m_storage(std::move(storage)) {}

    template <typename T>
        requires misc::traits::is_any_of_tuple_v<T, m_plain_types>
    constexpr static bool checkItem(uint64_t item) noexcept {
        return (item & 0b111) == misc::traits::idx_of_tuple_v<T, m_plain_types>;
    }

    template <typename T>
        requires misc::traits::is_any_of_tuple_v<T, m_plain_types>
    constexpr static uint64_t cloneItem(uint64_t item) noexcept {
        using decrypted_t =
            misc::traits::type_of_tuple_t<misc::traits::idx_of_tuple_v<T, m_plain_types>, m_supported_types>;

        if constexpr (std::is_pointer_v<decrypted_t>) {
            decrypted_t decrypted = decryptItem<T>(item);
            return encryptItem(new std::remove_pointer_t<decrypted_t>(*decrypted));
        } else
            return item;
    }

    template <typename T>
        requires misc::traits::is_any_of_tuple_v<T, m_plain_types>
    constexpr static auto decryptItem(uint64_t item) noexcept {
        using return_t =
            misc::traits::type_of_tuple_t<misc::traits::idx_of_tuple_v<T, m_plain_types>, m_supported_types>;

        if constexpr (std::is_pointer_v<return_t> || std::is_same_v<return_t, FastInstruction>) {
            uint64_t untagged = item & 0b000;
            return *reinterpret_cast<return_t*>(&untagged);
        } else {
            uint64_t shifted = item >> 3;
            return *reinterpret_cast<return_t*>(&shifted);
        }
    }

    template <typename T>
        requires misc::traits::is_any_of_tuple_v<T, m_supported_types>
    constexpr static uint64_t encryptItem(T item) noexcept {
        uint64_t before_tag;

        if constexpr (std::is_pointer_v<T> || std::is_same_v<T, FastInstruction>)
            before_tag = *reinterpret_cast<uint64_t*>(&item);
        else
            before_tag = ((*reinterpret_cast<uint64_t*>(&item)) /* & (uint64_t(-1) >> (64 - sizeof(T) * 8)) */) << 3;

        // place tag at the bottom 3 bits
        return before_tag | misc::traits::idx_of_tuple_v<T, m_supported_types>;
    }

    // apply function on item with correct type
    template <typename tuple_t = m_plain_types>
    struct Apply {
        template <typename fun_t>
        static constexpr auto run(uint64_t item, fun_t fun) {
            // this will never happen
            assert(false);
            return fun(Word(0));
        }
    };
    template <typename first_t, typename... rest_t>
    struct Apply<std::tuple<first_t, rest_t...>> {
        template <typename fun_t>
        static constexpr auto run(uint64_t item, fun_t fun) {
            if (checkItem<first_t>(item))
                return fun(decryptItem<first_t>(item));
            else
                return Apply<std::tuple<rest_t...>>::run(item, fun);
        }
    };

    constexpr void popAndDeleteTop() {
        Apply<>::run(m_storage.front(), [this](auto item) { pop<misc::traits::to_plain_type_t<decltype(item)>>(); });
    }

    // clone storage
    constexpr std::vector<uint64_t> cloneStorage() const {
        std::vector<uint64_t> out;
        out.reserve(m_storage.size());
        for (const auto& item : m_storage)
            out.emplace_back(Apply<>::run(item, [item](auto x) -> uint64_t {
                return cloneItem<misc::traits::to_plain_type_t<decltype(x)>>(item);
            }));

        return out;
    }

    std::vector<uint64_t> m_storage;
};
} // namespace cthu::program

inline std::ostream& operator<<(std::ostream& stream, const cthu::program::Stack& stack) {
    stream << stack.toShortString();
    return stream;
}
