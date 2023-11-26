#pragma once

#include "domains/idomain.hpp"

namespace cthu::domains {
class Instr : public IDomain {
  public:
    enum class Operation : uint32_t { move, dup, swap, drop, zero, noop };
    constexpr interpreter::ThreadState call(const std::string& operation,
                                            const std::vector<std::string>& operands,
                                            program::SafeDict& state,
                                            std::vector<program::SafeDict>& new_threads) const override;

    constexpr interpreter::ThreadState call(uint32_t operation_code,
                                            program::Dict& state,
                                            std::vector<program::Dict>& new_threads) const override;

    constexpr uint32_t compile(const std::string& operation, const std::vector<std::string>& operands) const override;

  private:
    constexpr static Operation operationFromName(const std::string& name);
};

} // namespace cthu::domains