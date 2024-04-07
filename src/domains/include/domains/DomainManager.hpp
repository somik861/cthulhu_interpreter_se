#pragma once

#include "builtin_structs/Dict.hpp"
#include "builtin_structs/FastInstruction.hpp"
#include "common/InterpreterException.hpp"

#include <array>
#include <memory>
#include <string_view>
#include <vector>

namespace program {
class State;
}

namespace domains {
class IDomain;

class DomainManagerException : public common::InterpreterException {
  public:
    using common::InterpreterException::InterpreterException;
};

class DomainManager {
  public:
    DomainManager();
    std::size_t getDomainID(std::string_view domain) const;
    IDomain* getDomain(std::string_view domain) const;
    IDomain* getDomain(std::size_t index) const;
    builtin_structs::InstructionResult execute(builtin_structs::FastInstruction instruction,
                                               program::State& state,
                                               std::vector<std::unique_ptr<builtin_structs::Dict>>& new_states) const;
    builtin_structs::FastInstruction compile(const builtin_structs::Instruction& instruction) const;

    ~DomainManager();

  private:
    std::array<std::unique_ptr<IDomain>, 4> m_domains;
};
} // namespace domains