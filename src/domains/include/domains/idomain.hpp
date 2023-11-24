#pragma once

#include "interpreter/thread_state.hpp"
#include "program/dict.hpp"
#include "program/dict_stack.hpp"
#include "program/instr.hpp"
#include "program/instr_fast.hpp"

namespace cthu::domains {
class IDomain {
  public:
    constexpr virtual interpreter::ThreadState call(const program::Instruction& instruction) const = 0;
    constexpr virtual interpreter::ThreadState call(const program::FastInstruction& instruction) const = 0;
    constexpr virtual program::FastInstruction compile(const program::Instruction& instruction) const = 0;

    constexpr virtual ~IDomain() noexcept = default;
};
} // namespace cthu::domains
