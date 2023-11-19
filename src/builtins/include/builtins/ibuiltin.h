#pragma once

#include "interpreter/execution_state.h"
#include "program/idict.h"

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
     * @param[in,out] state_dict     state to modify using instruction
     * @param[out] new_threads       insert newly created threads, the vector will always be empty on call
     *
     * @return                      execution state
     */
    virtual interpreter::ExecutionState executeOperation(const std::string& operation,
                                                         const std::vector<std::string>& operands,
                                                         program::IDict* state_dict,
                                                         std::vector<std::unique_ptr<program::IDict>>& new_threads) = 0;

    // Just to be consistent
    std::unique_ptr<IBuiltin> clone() { throw std::runtime_error("Builtin can not be copied"); }
    virtual ~IBuiltin() = default;

    static std::unique_ptr<IBuiltin> createBuiltin_bv32();
};
} // namespace cthu::builtins
