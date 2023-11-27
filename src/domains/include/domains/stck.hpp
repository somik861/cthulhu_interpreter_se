#pragma once

#include "domains/idomain.hpp"

namespace cthu::domains {
class Stck : public IDomain {
  public:
    enum class Operation : uint32_t { move, dup, swap, drop, zero, exec, pivot };
    interpreter::ThreadState call(const std::string& operation,
                                  const std::vector<std::string>& operands,
                                  program::SafeDict& state,
                                  std::vector<program::SafeDict>& new_threads) const override;

    interpreter::ThreadState call(uint32_t operation_code,
                                  program::Dict& state,
                                  std::vector<program::Dict>& new_threads) const override;

    uint32_t compile(const std::string& operation, const std::vector<std::string>& operands) const override;

    static Operation operationFromName(const std::string& name);
    static std::size_t getOperationArity(Operation op);
};
} // namespace cthu::domains
