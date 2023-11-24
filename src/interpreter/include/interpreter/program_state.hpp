#pragma once

#include "interpreter/thread_state.hpp"

namespace cthu::interpreter {
template <typename dict_t>
class ProgramState {
  public:
    dict_t state;
    std::size_t last_line_number = 0;
    std::size_t next_line_number = 0;
    std::size_t thread_id = 0;
    ThreadState thread_state = ThreadState::Running;

    std::string toShortString() const {
        std::string out = std::format("Last line number: {}\n", last_line_number);
        out += std::format("Next line number: {}\n", next_line_number);
        out += std::format("Thread ID: {}\n", thread_id);
        out += std::format("Running: {}\n", thread_state == ThreadState::Running);

        /*
        auto instruction_stack = state_dict->at(0);
        if (!instruction_stack->empty())
            out += std::format("Next execution: {}\n",
                               program::stack_utils::peekInstruction(instruction_stack)->toString());
        */
        out += std::format("State: {}", state.toShortString());

        return out;
    }
};

} // namespace cthu::interpreter
