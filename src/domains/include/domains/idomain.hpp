#pragma once

#include "interpreter/thread_state.hpp"
#include "program/dict.hpp"
#include "program/fast_instruction.hpp"
#include "program/instruction.hpp"
#include "program/safe_dict.hpp"

#include <vector>

namespace cthu::domains {
class IDomain {
  public:
    /**
     * @brief Call instruction of given domain
     *
     * Throws std::runtime_error with human-friendly message when something illegal happens.
     *
     * @param[in] instruction        instruction specification
     * @param[in,out] state          current program state
     * @param[out] new_threads       new threads that should be created (vector will always be empty at call)
     * @return                       new thread state
     */
    constexpr virtual interpreter::ThreadState call(
        const program::Instruction& instruction,
        program::SafeDict& state,
        std::vector<std::unique_ptr<program::SafeDict>>& new_threads) const = 0;

    /**
     * @brief Call instruction of given domain
     *
     * @param[in] instruction        instruction specification
     * @param[in,out] state          current program state
     * @param[out] new_threads       new threads that should be created (vector will always be empty at call)
     * @return                       new thread state
     */
    constexpr virtual interpreter::ThreadState call(const program::FastInstruction& instruction,
                                                    program::Dict& state,
                                                    std::vector<std::unique_ptr<program::Dict>>& new_threads) const = 0;

    /**
     * @brief Compile instruction to fast instruction
     *
     * Throws std::invalid_argument if instruction can not be compiled
     *
     * @param[in] instruction            instruction to compile
     * @return                           compiled instruction
     */
    constexpr virtual program::FastInstruction compile(const program::Instruction& instruction) const = 0;

    constexpr virtual ~IDomain() noexcept = default;
};
} // namespace cthu::domains
