#include "stck.h"

#include "macros.h"

#include "program/stack_utils.h"
#include <format>

#define _CHECK_OP_COUNT(n) CHECK_OPERANDS_COUNT("stck", n)

namespace cthu::builtins::impl {
interpreter::ExecutionState
Stck::executeOperation(const std::string& operation,
                       const std::vector<std::string>& operands,
                       program::IDict* state_dict,
                       std::vector<std::unique_ptr<program::IDict>>& new_threads) /*  override */ {

    if (operation == "split") {
        _CHECK_OP_COUNT(2);

        auto create_thread = [&](std::unique_ptr<program::IStack>& op_stack) {
            auto& new_dict = new_threads.emplace_back(state_dict->clone());
            new_dict->set(0, std::move(op_stack));
        };

        std::size_t first = std::stoul(operands[0]);
        std::size_t second = std::stoul(operands[1]);

        auto first_stack = program::stack_utils::popStack(state_dict->at(first));
        auto second_stack = program::stack_utils::popStack(state_dict->at(second));

        create_thread(first_stack);
        create_thread(second_stack);

        return interpreter::ExecutionState::FinishedWithError;
    }

    throw std::invalid_argument(std::format("Invalid operation: {}\n", operation));
}
} // namespace cthu::builtins::impl

namespace cthu::builtins {
/* static */ std::unique_ptr<IBuiltin> IBuiltin::createBuiltin_stck() { return std::make_unique<impl::Stck>(); }
} // namespace cthu::builtins
