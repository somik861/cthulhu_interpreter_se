#pragma once

#include "program/details/utils.hpp"
#include "program/stack.hpp"

#include <stdexcept>

namespace cthu::program {
class SafeStack : public Stack {
  public:
    template <typename T>
    constexpr auto peek() const {
        checkThrow<T>("Peek");

        return details::toSafe(Stack::peek<T>());
    };

    template <typename T>
    constexpr auto pop() {
        checkThrow<T>("Pop");

        return details::toSafe(Stack::pop<T>());
    };

    template <typename T>
    constexpr void push(T elem) {
        if constexpr (std::is_pointer_v<T>)
            if ((elem & 0b111) != 0)
                throw std::runtime_error("3 low bits of pointer are not zeroes");

        if constexpr (std::is_same_v<T, FastInstruction>)
            if ((elem.value & 0b111) != 0)
                throw std::runtime_error("3 low bits of instruction are not zeroes");

        Stack::push(details::fromSafe(std::move(elem)));
    };

  protected:
    template <typename T>
    constexpr void checkThrow(const std::string& fun_name) const {
        if (empty())
            throw std::runtime_error(fun_name + " called on an empty stack");

        if (!checkTop<T>())
            throw std::runtime_error(fun_name + " called with invalid type specifier");
    }
};

static_assert(sizeof(SafeStack) == sizeof(Stack), "The safe stack has something that it should not have.");
} // namespace cthu::program
