#pragma once

#include "builtin_structs/Dict.hpp"
#include "builtin_structs/FastInstruction.hpp"
#include "builtin_structs/Instruction.hpp"
#include "program/State.hpp"

#include <string>
#include <string_view>
#include <vector>

namespace domains {
class DomainManager;

class IDomain {
  public:
    virtual std::string getName() const = 0;
    virtual builtin_structs::FastInstruction compile(const builtin_structs::Instruction& instr,
                                                     const DomainManager* domain_manager) const = 0;
    virtual builtin_structs::InstructionResult execute(builtin_structs::FastInstruction instruction,
                                                       program::State& state,
                                                       std::vector<std::unique_ptr<builtin_structs::Dict>>& new_states,
                                                       const DomainManager* domain_manager) const = 0;

    virtual ~IDomain() = default;
};

} // namespace domains