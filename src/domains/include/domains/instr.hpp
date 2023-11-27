#pragma once

#include "domains/idomain.hpp"
#include "program/fast_instruction.hpp"

namespace cthu::domains {
class Instr : public IDomain {
  public:
    constexpr Instr(std::size_t domain_code) noexcept;
    enum class Operation : uint32_t { move, dup, swap, drop, zero, noop };
    constexpr interpreter::ThreadState call(const std::string& operation,
                                            const std::vector<std::string>& operands,
                                            program::SafeDict& state,
                                            std::vector<program::SafeDict>& new_threads) const override;

    constexpr interpreter::ThreadState call(uint32_t operation_code,
                                            program::Dict& state,
                                            std::vector<program::Dict>& new_threads) const override;

    constexpr uint32_t compile(const std::string& operation, const std::vector<std::string>& operands) const override;
    constexpr static Operation operationFromName(const std::string& name);
    constexpr static std::size_t getOperationArity(Operation op);
    static program::FastInstruction noop_instruction;
};

} // namespace cthu::domains
