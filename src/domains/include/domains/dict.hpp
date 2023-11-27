#pragma once

#include "domains/idomain.hpp"

namespace cthu::domains {
class Dict : public IDomain {
  public:
    enum class Operation : uint32_t { move, dup, swap, drop, zero, fetch, store };

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
};
} // namespace cthu::domains
