#include "domains/instr.hpp"

#include "details.hpp"

namespace cthu::domains {
constexpr interpreter::ThreadState Instr::call(const std::string& operation,
                                               const std::vector<std::string>& operands,
                                               program::SafeDict& state,
                                               std::vector<program::SafeDict>& new_threads) const /* override */ {
    return interpreter::ThreadState::Running;
}

constexpr interpreter::ThreadState Instr::call(uint32_t operation_code,
                                               program::Dict& state,
                                               std::vector<program::Dict>& new_threads) const /* override */ {
    return interpreter::ThreadState::Running;
}

constexpr uint32_t Instr::compile(const std::string& operation, const std::vector<std::string>& operands) const
/* override */ {
    return details::compress(0, {1});
}

constexpr /* static */ Instr::Operation Instr::operationFromName(const std::string& name) {
    if (name == "move")
        return Operation::move;
    if (name == "dup")
        return Operation::dup;
    if (name == "swap")
        return Operation::swap;
    if (name == "drop")
        return Operation::drop;
    if (name == "zero")
        return Operation::zero;
    if (name == "noop")
        return Operation::noop;

    throw std::invalid_argument(std::format("Operation {} is not supported in domain Instr", name));
}

} // namespace cthu::domains
