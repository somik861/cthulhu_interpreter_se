#include "domains/instr.hpp"

#include "details.hpp"
#include "program/fast_instruction.hpp"
#include "program/instruction.hpp"
#include "program/safe_dict.hpp"

namespace {
using Operation = cthu::domains::Instr::Operation;

template <typename dict_t>
constexpr void execute(dict_t& state, Operation op) {
    switch (op) {
    case Operation::noop:
        break;
    default:
        cthu::domains::details::throwInvalidOperation(op, 0);
    }
}
template <typename dict_t>
constexpr void execute(dict_t& state, Operation op, std::array<uint8_t, 1> args) {
    using instruction_t = std::conditional_t<std::is_same_v<dict_t, cthu::program::SafeDict>,
                                             cthu::program::Instruction,
                                             cthu::program::FastInstruction>;
    switch (op) {
    case Operation::drop:
        state.at(args[0])->pop<instruction_t>();
        break;
    case Operation::zero:
        if constexpr (std::is_same_v<instruction_t, cthu::program::Instruction>)
            state.at(args[0])->push(std::make_unique<instruction_t>("instr", "noop", std::vector<std::string>{}, 0));
        else
            state.at(args[0])->push(cthu::domains::Instr::noop_instruction);
        break;
    default:
        cthu::domains::details::throwInvalidOperation(op, 1);
    }
}
template <typename dict_t>
constexpr void execute(dict_t& state, Operation op, std::array<uint8_t, 2> args) {
    using instruction_t = std::conditional_t<std::is_same_v<dict_t, cthu::program::SafeDict>,
                                             cthu::program::Instruction,
                                             cthu::program::FastInstruction>;
    switch (op) {
    case Operation::move:
        state.at(args[1])->push(state.at(args[0])->pop<instruction_t>());
    case Operation::swap: {
        auto fst = state.at(args[0])->pop<instruction_t>();
        auto snd = state.at(args[1])->pop<instruction_t>();
        state.at(args[0])->push(std::move(snd));
        state.at(args[1])->push(std::move(fst));
        break;
    }
    default:
        cthu::domains::details::throwInvalidOperation(op, 2);
    }
}
template <typename dict_t>
constexpr void execute(dict_t& state, Operation op, std::array<uint8_t, 3> args) {
    using instruction_t = std::conditional_t<std::is_same_v<dict_t, cthu::program::SafeDict>,
                                             cthu::program::Instruction,
                                             cthu::program::FastInstruction>;
    switch (op) {
    case Operation::dup: {
        auto item = state.at(args[0])->pop<instruction_t>();
        if constexpr (std::is_same_v<instruction_t, cthu::program::Instruction>) {
            state.at(args[1])->push(std::unique_ptr<instruction_t>(new instruction_t(*item)));
            state.at(args[2])->push(std::unique_ptr<instruction_t>(new instruction_t(*item)));
        } else {
            state.at(args[1])->push(item);
            state.at(args[2])->push(item);
        }
        break;
    }
    default:
        cthu::domains::details::throwInvalidOperation(op, 3);
    }
}

template <typename dict_t>
constexpr cthu::interpreter::ThreadState callCommon(dict_t& state, uint32_t opcode) {
    Operation op = cthu::domains::details::extractOperation<Operation>(opcode);
    std::size_t arity = cthu::domains::Instr::getOperationArity(op);
    switch (arity) {
    case 0:
        execute(state, op);
        break;
    case 1:
        execute(state, op, cthu::domains::details::extractOperands<1>(opcode));
    case 2:
        execute(state, op, cthu::domains::details::extractOperands<2>(opcode));
        break;
    case 3:
        execute(state, op, cthu::domains::details::extractOperands<3>(opcode));
        break;
    default:
        throw std::runtime_error(std::format("Unsupported arity {} for domain 'instr'", arity));
    }

    return cthu::interpreter::ThreadState::Running;
}
} // namespace

namespace cthu::domains {
constexpr interpreter::ThreadState Instr::call(const std::string& operation,
                                               const std::vector<std::string>& operands,
                                               program::SafeDict& state,
                                               std::vector<program::SafeDict>& new_threads) const /* override */ {
    return callCommon(state, cthu::domains::Instr::compile(operation, operands));
}

constexpr interpreter::ThreadState Instr::call(uint32_t operation_code,
                                               program::Dict& state,
                                               std::vector<program::Dict>& new_threads) const /* override */ {
    return callCommon(state, operation_code);
}

constexpr Instr::Instr(std::size_t domain_code) noexcept {
    noop_instruction.domain_code = domain_code;
    noop_instruction.operation_code = details::compress(Operation::noop);
}

constexpr uint32_t Instr::compile(const std::string& operation, const std::vector<std::string>& operands) const
/* override */ {
    Operation op = operationFromName(operation);
    std::size_t arity = getOperationArity(op);
    details::checkThrowOperandsCount(arity, operands);
    return details::compress(op, operands);
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

constexpr /* static */ std::size_t Instr::getOperationArity(Operation op) {
    switch (op) {
    case cthu::domains::Instr::Operation::dup:
        return 3;
    case cthu::domains::Instr::Operation::move:
    case cthu::domains::Instr::Operation::swap:
        return 2;
    case cthu::domains::Instr::Operation::drop:
    case cthu::domains::Instr::Operation::zero:
        return 1;
    case cthu::domains::Instr::Operation::noop:
        return 0;
    default:
        throw std::invalid_argument(std::format("Invalid operation code: {}", static_cast<uint32_t>(op)));
    }
}

} // namespace cthu::domains
