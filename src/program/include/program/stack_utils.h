#pragma once

#include "program/idict.h"
#include "program/instruction.h"
#include "program/istack.h"
#include "program/istack_item.h"
#include "program/number.h"

namespace cthu::program::stack_utils {
namespace details {
template <typename T, typename S>
std::unique_ptr<T> unique_pointer_cast(std::unique_ptr<S>&& p) noexcept {
    auto converted = std::unique_ptr<T>{dynamic_cast<T*>(p.get())};
    if (converted) {
        p.release(); // no longer owns the pointer
    }
    return converted;
}
} // namespace details

inline std::unique_ptr<Number> popNumber(IStack* stack) {
    auto elem = stack->poptop();

    return details::unique_pointer_cast<Number>(std::move(elem));
}

inline uint32_t popNumberValue(IStack* stack) { return popNumber(stack)->getValue(); }

inline std::unique_ptr<Instruction> popInstruction(IStack* stack) {
    auto elem = stack->poptop();

    return details::unique_pointer_cast<Instruction>(std::move(elem));
}

inline std::unique_ptr<IStack> popStack(IStack* stack) {
    auto elem = stack->poptop();

    return details::unique_pointer_cast<IStack>(std::move(elem));
}

inline std::unique_ptr<IDict> popDict(IStack* stack) {
    auto elem = stack->poptop();

    return details::unique_pointer_cast<IDict>(std::move(elem));
}

} // namespace cthu::program::stack_utils
