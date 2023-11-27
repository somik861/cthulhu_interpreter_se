#include "domains/stck.hpp"

#include "details.hpp"

namespace cthu::domains {
namespace {
template <typename dict_t>
constexpr void execute(dict_t& state,
                       Stck::Operation operation,
                       std::array<uint8_t, 1> args,
                       std::vector<dict_t>& new_threads) {
    using stack_t = std::remove_reference_t<decltype(*state.at(0))>;
    switch (operation) {
    case Stck::Operation::drop:
        state.at(args[0])->template pop<program::Stack>();
        return;
    case Stck::Operation::zero:
        state.at(args[0])->push(std::make_unique<stack_t>());
        return;
    case Stck::Operation::exec: {
        auto instr_stack = state.at(args[0])->template pop<program::Stack>();
        auto temp_stack = state.pop(0);
        auto cpy = state;

        state.set(0, std::move(temp_stack));
        cpy.set(0, std::move(instr_stack));
        new_threads.push_back(std::move(cpy));
        return;
    }
    }
    details::throwers::invalidOperationForArity(operation, 1);
}

template <typename dict_t>
constexpr void execute(dict_t& state, Stck::Operation operation, std::array<uint8_t, 2> args) {
    switch (operation) {
    case Stck::Operation::move:
        state.at(args[1])->push(state.at(args[0])->template pop<program::Stack>());
        return;
    case Stck::Operation::swap:
        auto fst = state.at(args[0])->template pop<program::Stack>();
        auto snd = state.at(args[1])->template pop<program::Stack>();
        state.at(args[0])->push(std::move(snd));
        state.at(args[1])->push(std::move(fst));
        return;
    }
    details::throwers::invalidOperationForArity(operation, 2);
}

template <typename dict_t>
constexpr void execute(dict_t& state, Stck::Operation operation, std::array<uint8_t, 3> args) {
    using stack_t = std::remove_reference_t<decltype(*state.at(0))>;
    switch (operation) {
    case Stck::Operation::dup: {
        auto elem = state.at(args[0])->template pop<program::Stack>();
        state.at(args[1])->push(std::unique_ptr<stack_t>(new stack_t(*elem)));
        state.at(args[2])->push(std::move(elem));
    }
        return;
    case Stck::Operation::pivot: {
        auto stack = state.pop(args[0]);
        auto new_stack = state.at(args[1])->template pop<program::Stack>();
        state.set(args[0], std::move(new_stack));
        state.at(args[2])->push(std::move(stack));
        return;
    }
    }
    details::throwers::invalidOperationForArity(operation, 3);
}

template <typename dict_t>
constexpr interpreter::ThreadState callCommon(dict_t& state, uint32_t opcode, std::vector<dict_t>& new_threads) {
    Stck::Operation op = details::extractOperation<Stck::Operation>(opcode);
    std::size_t arity = Stck::getOperationArity(op);
    switch (arity) {
    case 1:
        execute(state, op, details::extractOperands<1>(opcode), new_threads);
        break;
    case 2:
        execute(state, op, details::extractOperands<2>(opcode));
        break;
    case 3:
        execute(state, op, details::extractOperands<3>(opcode));
        break;
    default:
        details::throwers::unsupportedArity(arity, "Stck");
        break;
    }
    return interpreter::ThreadState::Running;
}
} // namespace

interpreter::ThreadState Stck::call(const std::string& operation,
                                    const std::vector<std::string>& operands,
                                    program::SafeDict& state,
                                    std::vector<program::SafeDict>& new_threads) const /* override */ {
    return callCommon(state, Stck::compile(operation, operands), new_threads);
}

interpreter::ThreadState Stck::call(uint32_t operation_code,
                                    program::Dict& state,
                                    std::vector<program::Dict>& new_threads) const /* override */ {
    return callCommon(state, operation_code, new_threads);
}

uint32_t Stck::compile(const std::string& operation, const std::vector<std::string>& operands) const
/* override */ {
    Operation op = operationFromName(operation);
    std::size_t arity = getOperationArity(op);
    details::checkThrowOperandsCount(arity, operands);
    return details::compress(op, operands);
}

/* static */ Stck::Operation Stck::operationFromName(const std::string& name) {
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
    if (name == "exec")
        return Operation::exec;
    if (name == "pivot")
        return Operation::pivot;

    details::throwers::invalidOperation(name, "Stck");
    return Operation::pivot; // unreachable
}
/* static */ std::size_t Stck::getOperationArity(Operation op) {
    switch (op) {
    case Operation::zero:
    case Operation::drop:
        return 1;
    case Operation::exec:
    case Operation::move:
    case Operation::swap:
        return 2;
    case Operation::dup:
    case Operation::pivot:
        return 3;
    }

    details::throwers::invalidOperationCode(op, "Stck");
    return 0; // unreachable
}
} // namespace cthu::domains
