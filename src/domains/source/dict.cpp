#include "domains/dict.hpp"

#include "details.hpp"
#include "program/dict.hpp"

#include <array>

namespace cthu::domains {
namespace {
template <typename dict_t>
constexpr void execute(dict_t& state, Dict::Operation operation, std::array<uint8_t, 1> args) {
    switch (operation) {
    case Dict::Operation::drop:
        state.at(args[0])->pop<program::Dict>();
        return;
    case Dict::Operation::zero:
        state.at(args[0])->push(std::make_unique<dict_t>());
        return;
    }
    details::throwers::invalidOperationForArity(operation, 1);
}
template <typename dict_t>
constexpr void execute(dict_t& state, Dict::Operation operation, std::array<uint8_t, 2> args) {
    switch (operation) {
    case Dict::Operation::move:
        state.at(args[1])->push(state.at(args[0])->pop<program::Dict>());
        return;
    case Dict::Operation::swap: {
        auto fst = state.at(args[0])->pop<program::Dict>();
        auto snd = state.at(args[1])->pop<program::Dict>();
        state.at(args[0])->push(std::move(snd));
        state.at(args[1])->push(std::move(fst));
        return;
    }
    }
    details::throwers::invalidOperationForArity(operation, 2);
}
template <typename dict_t>
constexpr void execute(dict_t& state, Dict::Operation operation, std::array<uint8_t, 3> args) {
    switch (operation) {
    case Dict::Operation::dup:
        auto elem = state.at(args[0])->pop<program::Dict>();
        state.at(args[1])->push(std::unique_ptr<dict_t>(new dict_t(*elem)));
        state.at(args[2])->push(std::move(elem));
        return;
    }
    details::throwers::invalidOperationForArity(operation, 3);
}
template <typename dict_t>
constexpr void execute(dict_t& state, Dict::Operation operation, std::array<uint8_t, 5> args) {
    switch (operation) {
    case Dict::Operation::fetch: {
        auto stack = state.pop(args[0]);
        auto dict = state.at(args[1])->pop<program::Dict>();
        auto key = state.at(args[2])->pop<program::Word>();
        state.set(args[0], dict->pop(key));
        state.at(args[3])->push(std::move(dict));
        state.at(args[4])->push(std::move(stack));
        return;
    }
    case Dict::Operation::store: {
        auto stack = state.pop(args[0]);
        auto dict = state.at(args[1])->pop<program::Dict>();
        auto key = state.at(args[2])->pop<program::Word>();
        auto new_stack = state.pop(args[3]);
        dict->set(key, std::move(stack));
        state.set(args[0], std::move(new_stack));
        state.at(args[4])->push(std::move(dict));
    }
    }
    details::throwers::invalidOperationForArity(operation, 5);
}

template <typename dict_t>
constexpr interpreter::ThreadState callCommon(dict_t& state, uint32_t opcode) {
    Dict::Operation op = details::extractOperation<Dict::Operation>(opcode);
    std::size_t arity = Dict::getOperationArity(op);
    switch (arity) {
    case 1:
        execute(state, op, details::extractOperands<1>(opcode));
        break;
    case 2:
        execute(state, op, details::extractOperands<2>(opcode));
        break;
    case 3:
        execute(state, op, details::extractOperands<3>(opcode));
        break;
    case 5:
        execute(state, op, details::extractOperands<5>(opcode));
        break;
    default:
        details::throwers::unsupportedArity(arity, "Dict");
        break;
    }
    return interpreter::ThreadState::Running;
}
} // namespace

constexpr interpreter::ThreadState Dict::call(const std::string& operation,
                                              const std::vector<std::string>& operands,
                                              program::SafeDict& state,
                                              std::vector<program::SafeDict>& new_threads) const /* override */ {
    return callCommon(state, Dict::compile(operation, operands));
}

constexpr interpreter::ThreadState Dict::call(uint32_t operation_code,
                                              program::Dict& state,
                                              std::vector<program::Dict>& new_threads) const /* override */ {
    return callCommon(state, operation_code);
}

constexpr uint32_t Dict::compile(const std::string& operation, const std::vector<std::string>& operands) const
/* override */ {
    Operation op = operationFromName(operation);
    std::size_t arity = getOperationArity(op);
    details::checkThrowOperandsCount(arity, operands);
    return details::compress(op, operands);
}

constexpr /* static */ Dict::Operation Dict::operationFromName(const std::string& name) {
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
    if (name == "fetch")
        return Operation::fetch;
    if (name == "store")
        return Operation::store;

    details::throwers::invalidOperation(name, "Dict");
}
constexpr /* static */ std::size_t Dict::getOperationArity(Operation op) {
    switch (op) {
    case Operation::zero:
    case Operation::drop:
        return 1;
    case Operation::move:
    case Operation::swap:
        return 2;
    case Operation::dup:
        return 3;
    case Operation::fetch:
    case Operation::store:
        return 5;
    }

    details::throwers::invalidOperationCode(op, "Dict");
}

} // namespace cthu::domains
