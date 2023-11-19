#include "bv32.h"

#include "program/stack_utils.h"
#include <format>

namespace cthu::builtins::impl {
interpreter::ExecutionState
Bv32::executeOperation(const std::string& operation,
                       const std::vector<std::string>& operands,
                       program::IDict* state_dict,
                       std::vector<std::unique_ptr<program::IDict>>& new_threads) /*  override */ {
    if (operation == "add") {
        std::size_t lhs_stack = std::stoul(operands[0]);
        std::size_t rhs_stack = std::stoul(operands[1]);
        std::size_t res_stack = std::stoul(operands[2]);

        uint32_t lhs = program::stack_utils::popNumberValue(state_dict->at(lhs_stack));
        uint32_t rhs = program::stack_utils::popNumberValue(state_dict->at(rhs_stack));

        state_dict->at(res_stack)->push(program::Number::createUnique(lhs + rhs));
        return interpreter::ExecutionState::Running;
    }

    throw std::invalid_argument(std::format("Invalid operation: {}\n", operation));
}
} // namespace cthu::builtins::impl

namespace cthu::builtins {
/* static */ std::unique_ptr<IBuiltin> IBuiltin::createBuiltin_bv32() { return std::make_unique<impl::Bv32>(); }
} // namespace cthu::builtins
