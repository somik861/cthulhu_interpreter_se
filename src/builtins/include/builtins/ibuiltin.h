#pragma once

#include "interpreter/program_state.h"

#include <stdexcept>
#include <string>
#include <vector>

namespace cthu::builtins {
class IBuiltin {
  public:
    /**
     * @brief Execute operation with given operands
     *
     * std::invalid_argument is thrown if an operation is unknown or the operands are incorrect
     *
     * @param[in] opreation          operation to execute
     * @param[in] operands           operation operands
     * @param[in,out] state          state to modify using instruction
     */
    virtual void executeOperation(const std::string& operation,
                                  const std::vector<std::string>& operands,
                                  interpreter::ProgramState* state) = 0;

    // Just to be consistent
    std::unique_ptr<IBuiltin> clone() { throw std::runtime_error("Builtin can not be copied"); }
    virtual ~IBuiltin() = default;

    static std::unique_ptr<IBuiltin> createBuiltin_bv32();
};
} // namespace cthu::builtins
