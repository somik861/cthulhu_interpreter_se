#pragma once

#include <domains/IDomain.hpp>

#include <string>
#include <unordered_map>

namespace domains {
class Dictionary : public IDomain {
  public:
    std::string getName() const override;
    builtin_structs::FastInstruction compile(const builtin_structs::Instruction& instr,
                                             const DomainManager* domain_manager) const override;
    builtin_structs::InstructionResult execute(builtin_structs::FastInstruction instruction,
                                               program::State& state,
                                               std::vector<std::unique_ptr<builtin_structs::Dict>>& new_states,
                                               const DomainManager* domain_manager) const override;

  private:
    enum class Operation { move = 0, drop, dup };

    std::unordered_map<std::string, std::pair<Operation, uint8_t>> m_argument_map{{"move", {Operation::move, 2}},
                                                                                  {"drop", {Operation::drop, 1}},
                                                                                  {"dup", {Operation::dup, 3}}};
};
} // namespace domains