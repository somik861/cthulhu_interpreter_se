#pragma once

#include <format>
#include <stdexcept>

#define CHECK_OPERANDS_COUNT(structure, n)                                                                             \
    if (operands.size() != n)                                                                                          \
        throw std::invalid_argument(std::format("Unsupported operand count of {} operation {}, got {}, expected {}",   \
                                                structure, operation, operands.size(), n));

#define TERNARY_OPERATION(structure, popType, pushType, op)                                                            \
    {                                                                                                                  \
        CHECK_OPERANDS_COUNT(structure, 3);                                                                            \
        std::size_t lhs_stack = std::stoul(operands[0]);                                                               \
        std::size_t rhs_stack = std::stoul(operands[1]);                                                               \
        std::size_t res_stack = std::stoul(operands[2]);                                                               \
        auto lhs = program::stack_utils::popType(state_dict->at(lhs_stack));                                           \
        auto rhs = program::stack_utils::popType(state_dict->at(rhs_stack));                                           \
        state_dict->at(res_stack)->push(program::pushType::createUnique(lhs op rhs));                                  \
        return interpreter::ExecutionState::Running;                                                                   \
    }

#define TERNARY_NUMBER_OPERATION(structure, op) TERNARY_OPERATION(structure, popNumberValue, Number, op);
