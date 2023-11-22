#include "bv32.h"

#include "macros.h"
#include "program/stack_utils.h"
#include <format>
#include <stdexcept>

#define _CHECK_OP_COUNT(n) CHECK_OPERANDS_COUNT("bv32", n)
#define _TERNARY_NUM(op) TERNARY_NUMBER_OPERATION("bv32", op)

namespace cthu::builtins::impl {
interpreter::ExecutionState Bv32::executeOperation(
    const std::string& operation,
    const std::vector<std::string>& operands,
    program::IDict* state_dict,
    std::vector<std::unique_ptr<program::IDict>>& new_threads) /*  override */ {
    if (operation == "add")
        _TERNARY_NUM(+);
    if (operation == "lt")
        _TERNARY_NUM(<);
    if (operation == "mul")
        _TERNARY_NUM(*);
    if (operation == "sub")
        _TERNARY_NUM(-);

    if (operation == "swap") {
        _CHECK_OP_COUNT(2);
        std::size_t lhs = std::stoul(operands[0]);
        std::size_t rhs = std::stoul(operands[1]);

        auto lhs_number = program::stack_utils::popNumber(state_dict->at(lhs));
        auto rhs_number = program::stack_utils::popNumber(state_dict->at(rhs));

        state_dict->at(lhs)->push(std::move(rhs_number));
        state_dict->at(rhs)->push(std::move(lhs_number));

        return interpreter::ExecutionState::Running;
    }

    if (operation == "move") {
        _CHECK_OP_COUNT(2);
        std::size_t source = std::stoul(operands[0]);
        std::size_t dest = std::stoul(operands[1]);

        auto number = program::stack_utils::popNumber(state_dict->at(source));

        state_dict->at(dest)->push(std::move(number));

        return interpreter::ExecutionState::Running;
    }

    if (operation == "dup") {
        _CHECK_OP_COUNT(3);

        std::size_t source = std::stoul(operands[0]);
        std::size_t dest1 = std::stoul(operands[1]);
        std::size_t dest2 = std::stoul(operands[2]);

        auto number = program::stack_utils::popNumber(state_dict->at(source));
        state_dict->at(dest1)->push(number->cloneItem());
        state_dict->at(dest2)->push(std::move(number));

        return interpreter::ExecutionState::Running;
    }

    if (operation == "const") {
        _CHECK_OP_COUNT(2);
        std::size_t number = std::stoul(operands[0]);
        std::size_t dest = std::stoul(operands[1]);

        state_dict->at(dest)->push(program::Number::createUnique(number));

        return interpreter::ExecutionState::Running;
    }

    if (operation == "guard_nonzero") {
        _CHECK_OP_COUNT(1);

        std::size_t stack_idx = std::stoul(operands[0]);
        uint32_t val = program::stack_utils::popNumberValue(state_dict->at(stack_idx));
        if (val == 0)
            return interpreter::ExecutionState::FinishedWithError;

        return interpreter::ExecutionState::Running;
    }

    if (operation == "guard_zero") {
        _CHECK_OP_COUNT(1);

        std::size_t stack_idx = std::stoul(operands[0]);
        uint32_t val = program::stack_utils::popNumberValue(state_dict->at(stack_idx));
        if (val != 0)
            return interpreter::ExecutionState::FinishedWithError;

        return interpreter::ExecutionState::Running;
    }

    throw std::invalid_argument(std::format("Invalid operation: {}\n", operation));
}
} // namespace cthu::builtins::impl

namespace cthu::builtins {
/* static */ std::unique_ptr<IBuiltin> IBuiltin::createBuiltin_bv32() { return std::make_unique<impl::Bv32>(); }
} // namespace cthu::builtins
