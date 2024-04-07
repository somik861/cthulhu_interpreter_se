#pragma once

#include "builtin_structs/BitVector.hpp"
#include "builtin_structs/FastInstruction.hpp"
#include "builtin_structs/Instruction.hpp"
#include "common/InterpreterException.hpp"
#include "common/traits.hpp"
#include <cassert>
#include <type_traits>
#include <vector>

namespace builtin_structs {
class Dict;

namespace details {
template <typename T>
struct is_valid_stack_type :
    public std::conditional_t<std::is_pointer_v<T> || (sizeof(T) <= 8 && std::is_trivially_copyable_v<T>),
                              std::true_type,
                              std::false_type> {};
} // namespace details

class StackException : public common::InterpreterException {
  public:
    using common::InterpreterException::InterpreterException;
};

class Stack {
    struct Elem {
        uint64_t data;
        uint8_t type_id;
    };

  public:
    using backend_type = std::vector<Elem>;
    using raw_types = std::tuple<Stack*, Dict*, FastInstruction, Instruction*, BitVector8, BitVector16, BitVector32>;
    using owning_types = common::traits::to_owning_tuple_t<raw_types>;

    static_assert(common::traits::is_tuple_v<raw_types>);
    static_assert(std::tuple_size_v<raw_types> <= 256);
    static_assert(std::tuple_size_v<raw_types> >= 1);
    static_assert(common::traits::all_of_tuple_v<raw_types, details::is_valid_stack_type>);
    static_assert(std::is_same_v<typename backend_type::value_type, Elem>);

    Stack() = default;
    Stack(const Stack& s) {
        for (Elem elem : s.m_storage) {
            UnknownElem<>::visit(elem, [this](auto elem) {
                using elem_t = decltype(elem);
                if constexpr (std::is_pointer_v<elem_t>)
                    push(std::make_unique<std::remove_pointer_t<elem_t>>(*elem));
                else
                    push(elem);
            });
        }
    }
    Stack(Stack&& s) = default;
    Stack& operator=(const Stack& s) {
        Stack cpy = s;
        std::swap(cpy, *this);
        return *this;
    }
    Stack& operator=(Stack&& s) noexcept {
        clear();
        m_storage = std::move(s.m_storage);
        s.m_storage.clear();
        return *this;
    }
    ~Stack() noexcept { clear(); }

    std::size_t size() const noexcept { return m_storage.size(); }
    template <typename T>
        requires common::traits::is_any_of_tuple_v<T, owning_types>
    void push(T elem) {
        Elem stack_elem;
        if constexpr (common::traits::is_unique_ptr_v<T>)
            stack_elem.data = reinterpret_cast<uint64_t>(elem.release());
        else
            stack_elem.data = (*reinterpret_cast<uint64_t*>(&elem));
        stack_elem.type_id = common::traits::idx_of_tuple_v<T, owning_types>;
        m_storage.push_back(stack_elem);
    }

    template <typename T>
        requires common::traits::is_any_of_tuple_v<T, owning_types>
    T pop() {
        using raw_t = common::traits::to_raw_type_t<T>;
        auto rv = peek<raw_t>();
        m_storage.pop_back();
        if constexpr (std::is_pointer_v<raw_t>)
            return std::unique_ptr<std::remove_pointer_t<raw_t>>(rv);
        else
            return rv;
    }

    void pop() {
        if (size() == 0)
            throw StackException("Stack access error: Requested an element "
                                 "when size() == 0");
        UnknownElem<>::visit(m_storage.back(),
                             [this](auto x) { pop<common::traits::to_owning_type_t<decltype(x)>>(); });
    }

    template <typename T>
        requires common::traits::is_any_of_tuple_v<T, raw_types>
    T peek() const {
        if (size() == 0)
            throw StackException("Stack access error: Requested an element "
                                 "when size() == 0");

        return decryptElem<T>(m_storage.back());
    }

    void clear() noexcept {
        while (m_storage.size() > 0)
            pop();
    }
    std::string toString(int verbose_level) const {
        if (verbose_level <= 0)
            return "stack";
        std::string out = "| ";
        for (const auto& elem : m_storage)
            out += UnknownElem<>::visit(elem,
                                        [verbose_level](auto el) -> std::string {
                                            using elem_t = decltype(el);
                                            if constexpr (std::is_pointer_v<elem_t>)
                                                return el->toString(verbose_level <= 1 ? 0 : verbose_level - 1);
                                            else
                                                return common::string_utils::stringFromStreamable(el);
                                        }) +
                   " ";
        out += " >t";
        return out;
    }

  private:
    template <typename T>
        requires common::traits::is_any_of_tuple_v<T, raw_types>
    static bool checkElem(Elem elem) {
        constexpr std::size_t type_id = common::traits::idx_of_tuple_v<T, raw_types>;
        return type_id == elem.type_id;
    }

    template <typename T>
        requires common::traits::is_any_of_tuple_v<T, raw_types>
    static T decryptElem(Elem elem) {
        if (!checkElem<T>(elem))
            throw StackException("Stack access error: Requested invalid type");

        if constexpr (std::is_pointer_v<T>)
            return reinterpret_cast<T>(elem.data);
        return *reinterpret_cast<T*>(&elem.data);
    }

    template <typename T = raw_types>
    struct UnknownElem {
        template <typename fun_t>
        static auto visit(Elem elem, fun_t fun) {
            std::unreachable();
            assert(false);
            return std::invoke_result_t<fun_t, std::tuple_element_t<0, raw_types>>{};
        }
    };
    template <typename first_t, typename... rest_t>
    struct UnknownElem<std::tuple<first_t, rest_t...>> {
        template <typename fun_t>
        static auto visit(Elem elem, fun_t fun) {
            if (checkElem<first_t>(elem))
                return fun(decryptElem<first_t>(elem));
            return UnknownElem<std::tuple<rest_t...>>::visit(elem, fun);
        }
    };

    backend_type m_storage;
};
} // namespace builtin_structs

#include "builtin_structs.hpp"