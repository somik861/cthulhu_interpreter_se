#include <domains/Dictionary.hpp>

#include <common/compile_utils.hpp>
#include <common/execution_utils.hpp>
#include <domains/DomainManager.hpp>
#include <memory>

namespace domains {
std::string Dictionary::getName() const { return "dict"; }
builtin_structs::FastInstruction Dictionary::compile(const builtin_structs::Instruction& instr,
                                                     const DomainManager* domain_manager) const {
    builtin_structs::FastInstruction finstr(uint8_t(domain_manager->getDomainID(getName())));
    if (false) {
    } else if (m_argument_map.contains(instr.operation())) {
        auto [op, arg_count] = m_argument_map.at(instr.operation());
        finstr.data.operation = static_cast<uint8_t>(op);
        common::compile_utils::compileIntArgs(finstr, instr.operands(), arg_count);
    } else
        throw common::compile_utils::CompileException(std::format("Unknown operation: {}", instr.operation()));

    return finstr;
}
builtin_structs::InstructionResult Dictionary::execute(builtin_structs::FastInstruction finstr,
                                                       program::State& state,
                                                       std::vector<std::unique_ptr<builtin_structs::Dict>>& new_states,
                                                       const DomainManager* domain_manager) const {
    using namespace common::execution_utils;
    using dict_t = std::unique_ptr<builtin_structs::Dict>;

    switch (static_cast<Operation>(finstr.data.operation)) {
    case Operation::drop:
        generic::drop<dict_t>(state, finstr);
        break;
    case Operation::dup:
        generic::dup<dict_t>(state, finstr);
        break;
    case Operation::move:
        generic::move<dict_t>(state, finstr);
        break;
    }
    return builtin_structs::InstructionResult::OK;
}
} // namespace domains