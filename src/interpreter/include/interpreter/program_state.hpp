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

        auto instruction_stack = state.at(0);
        if (!instruction_stack->empty()) {
            std::string next_instr;
            if constexpr (std::is_same_v<dict_t, program::SafeDict>)
                next_instr = instruction_stack->peek<program::Instruction>()->toShortString();
            else
                next_instr = instruction_stack->peek<program::FastInstruction>().toShortString();
            out += std::format("Next execution: {}\n", next_instr);
        }

        out += std::format("State: {}", state.toShortString());

        return out;
    }
};

} // namespace cthu::interpreter

template <typename T>
inline std::ostream& operator<<(std::ostream& stream, const cthu::interpreter::ProgramState<T>& state) {
    stream << state.toShortString();
    return stream;
}
