#pragma once

#include "interpreter/execution_state.h"
#include "program/idict.h"
#include "program/stack_utils.h"
#include "utils/iset.h"

#include <format>
#include <memory>
#include <string>
#include <string_view>

namespace cthu::interpreter {
class ProgramState {
  public:
    /**
     * @brief line number corresponding to the last processed line
     */
    std::size_t last_line_number = 0;
    /**
     * @brief number of line that will be executed next.
     *
     * NOTE: setting this is mostly job of the interpreter for your convenience.
     * If you want to change the line to be executed, please make sure that the interpreter
     * also reads and takes it into consideration, otherwise it will have no effect.
     */
    std::size_t next_line_number = 0;

    /**
     * pointer to the state dict
     */
    std::unique_ptr<program::IDict> state_dict = nullptr;

    std::size_t thread_id = 0;

    ExecutionState execution_state = ExecutionState::Running;
    std::string error_message = "";

    std::string toString() const {
        std::string out = std::format("Last line number: {}\n", last_line_number);
        out += std::format("Next line number: {}\n", next_line_number);
        out += std::format("Thread ID: {}\n", thread_id);
        out += std::format("Running: {}\n", execution_state == ExecutionState::Running);
        if (!error_message.empty())
            out += std::format("Error message: {}\n", error_message);

        auto instruction_stack = state_dict->at(0);
        if (!instruction_stack->empty())
            out += std::format("Next execution: {}\n",
                               program::stack_utils::peekInstruction(instruction_stack)->toString());

        out += std::format("State: {}", state_dict ? state_dict->toShortString() : "nullptr");

        return out;
    }
};
} // namespace cthu::interpreter
