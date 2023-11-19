#pragma once

#include "interpreter/execution_state.h"
#include "program/idict.h"
#include "utils/iset.h"

#include <memory>
#include <string>
#include <string_view>

namespace cthu::interpreter {
class ProgramState {
  public:
    /**
     * @brief line number corresponding to the last processed line
     */
    int last_line_number = 0;
    /**
     * @brief number of line that will be executed next.
     *
     * NOTE: setting this is mostly job of the interpreter for your convenience.
     * If you want to change the line to be executed, please make sure that the interpreter
     * also reads and takes it into consideration, otherwise it will have no effect.
     */
    int next_line_number = 0;

    /**
     * pointer to the state dict
     */
    std::unique_ptr<program::IDict> state_dict = nullptr;

    std::size_t thread_id = 0;

    ExecutionState execution_state = ExecutionState::Running;
    std::string error_message = "";
};
} // namespace cthu::interpreter
