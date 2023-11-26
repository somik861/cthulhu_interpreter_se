#pragma once

#include "interpreter/thread_state.hpp"
#include "program/dict.hpp"
#include "program/instruction.hpp"
#include "program/safe_dict.hpp"

#include <vector>

namespace cthu::domains {
class IDomain {
  public:
    /**
     * @brief Call operation of given domain
     *
     * Throws std::runtime_error with human-friendly message when something illegal happens.
     *
     * @param[in] operation          operation to execute
     * @param[in] operands           operands
     * @param[in,out] state          current program state
     * @param[out] new_threads       new threads that should be created (vector will always be empty at call)
     * @return                       new thread state
     */
    constexpr virtual interpreter::ThreadState call(const std::string& operation,
                                                    const std::vector<std::string>& operands,
                                                    program::SafeDict& state,
                                                    std::vector<program::SafeDict>& new_threads) const = 0;

    /**
     * @brief Call operation of given domain
     *
     * @param[in] operation_code     code of the operation
     * @param[in,out] state          current program state
     * @param[out] new_threads       new threads that should be created (vector will always be empty at call)
     * @return                       new thread state
     */
    constexpr virtual interpreter::ThreadState call(uint32_t operation_code,
                                                    program::Dict& state,
                                                    std::vector<program::Dict>& new_threads) const = 0;

    /**
     * @brief Compile operation with arguments to operation code
     *
     * Throws std::invalid_argument if operation can not be compiled
     *
     * @param[in] operation            operation
     * @param[in] operands             operands
     * @return                         operation code
     */
    constexpr virtual uint32_t compile(const std::string& operation,
                                       const std::vector<std::string>& operands) const = 0;

    constexpr virtual ~IDomain() noexcept = default;
};
} // namespace cthu::domains
