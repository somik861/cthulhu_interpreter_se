#include "domains/builtins/bv32.hpp"
#include "../details.hpp"

namespace cthu::domains::builtins {
namespace {
template <typename dict_t>
constexpr interpreter::ThreadState execute(dict_t& state,
                                           Bv32::Operation operation,
                                           std::array<uint8_t, 1> args,
                                           std::vector<dict_t>& new_threads) {
    switch (operation) {
    case Bv32::Operation::drop:
        state.at(args[0])->template pop<program::Word>();
        return interpreter::ThreadState::Running;
    case Bv32::Operation::zero:
        state.at(args[0])->push(program::Word(0));
        return interpreter::ThreadState::Running;
    case Bv32::Operation::guard_zero:
        return (state.at(args[0])->template pop<program::Word>().value == 0) ? interpreter::ThreadState::Running
                                                                             : interpreter::ThreadState::Killed;
    case Bv32::Operation::guard_nonzero:
        return (state.at(args[0])->template pop<program::Word>().value != 0) ? interpreter::ThreadState::Running
                                                                             : interpreter::ThreadState::Killed;
    }

    details::throwers::invalidOperationForArity(operation, 1);
}

template <typename dict_t>
constexpr void execute(dict_t& state, Bv32::Operation operation, std::array<uint8_t, 2> args) {
    switch (operation) {
    case Bv32::Operation::move:
        state.at(args[1])->push(state.at(args[0])->template pop<program::Word>());
        return;
    case Bv32::Operation::swap: {
        auto fst = state.at(args[0])->template pop<program::Word>();
        auto snd = state.at(args[1])->template pop<program::Word>();
        state.at(args[0])->push(snd);
        state.at(args[1])->push(fst);
        return;
    }
    case Bv32::Operation::neg:
        details::ops_templ::compute_unary<program::Word>(state, args, [](auto arg) {
            return program::Word(~arg.value);
        });
        return;
    }
    details::throwers::invalidOperationForArity(operation, 2);
}

template <typename dict_t>
constexpr void execute(dict_t& state, Bv32::Operation operation, std::array<uint8_t, 3> args) {
    using word_t = program::Word;

    switch (operation) {
    case Bv32::Operation::dup: {
        auto elem = state.at(args[0])->template pop<word_t>();
        state.at(args[1])->push(elem);
        state.at(args[2])->push(elem);
        return;
    }
    case Bv32::Operation::add:
        details::ops_templ::compute_binary<word_t>(state, args, [](auto lhs, auto rhs) {
            return word_t(lhs.value + rhs.value);
        });
        return;
    case Bv32::Operation::sub:
        details::ops_templ::compute_binary<word_t>(state, args, [](auto lhs, auto rhs) {
            return word_t(lhs.value - rhs.value);
        });
        return;
    case Bv32::Operation::mul:
        details::ops_templ::compute_binary<word_t>(state, args, [](auto lhs, auto rhs) {
            return word_t(lhs.value * rhs.value);
        });
        return;
    case Bv32::Operation::div:
        details::ops_templ::compute_binary<word_t>(state, args, [](auto lhs, auto rhs) {
            return word_t(lhs.value / rhs.value);
        });
        return;
    case Bv32::Operation::mod:
        details::ops_templ::compute_binary<word_t>(state, args, [](auto lhs, auto rhs) {
            return word_t(lhs.value % rhs.value);
        });
        return;
    case Bv32::Operation::eq:
        details::ops_templ::compute_binary<word_t>(state, args, [](auto lhs, auto rhs) {
            return word_t(lhs.value == rhs.value);
        });
        return;
    case Bv32::Operation::neq:
        details::ops_templ::compute_binary<word_t>(state, args, [](auto lhs, auto rhs) {
            return word_t(lhs.value != rhs.value);
        });
        return;
    case Bv32::Operation::lt:
        details::ops_templ::compute_binary<word_t>(state, args, [](auto lhs, auto rhs) {
            return word_t(lhs.value < rhs.value);
        });
        return;
    case Bv32::Operation::le:
        details::ops_templ::compute_binary<word_t>(state, args, [](auto lhs, auto rhs) {
            return word_t(lhs.value <= rhs.value);
        });
        return;
    case Bv32::Operation::gt:
        details::ops_templ::compute_binary<word_t>(state, args, [](auto lhs, auto rhs) {
            return word_t(lhs.value > rhs.value);
        });
        return;
    case Bv32::Operation::ge:
        details::ops_templ::compute_binary<word_t>(state, args, [](auto lhs, auto rhs) {
            return word_t(lhs.value >= rhs.value);
        });
        return;
    case Bv32::Operation::and_:
        details::ops_templ::compute_binary<word_t>(state, args, [](auto lhs, auto rhs) {
            return word_t(lhs.value & rhs.value);
        });
        return;
    case Bv32::Operation::xor_:
        details::ops_templ::compute_binary<word_t>(state, args, [](auto lhs, auto rhs) {
            return word_t(lhs.value ^ rhs.value);
        });
        return;
    case Bv32::Operation::or_:
        details::ops_templ::compute_binary<word_t>(state, args, [](auto lhs, auto rhs) {
            return word_t(lhs.value | rhs.value);
        });
        return;
    case Bv32::Operation::land:
        details::ops_templ::compute_binary<word_t>(state, args, [](auto lhs, auto rhs) {
            return word_t(lhs.value && rhs.value);
        });
        return;
    case Bv32::Operation::lor:
        details::ops_templ::compute_binary<word_t>(state, args, [](auto lhs, auto rhs) {
            return word_t(lhs.value || rhs.value);
        });
        return;
    }

    details::throwers::invalidOperationForArity(operation, 3);
}

template <typename dict_t>
constexpr interpreter::ThreadState callCommon(dict_t& state, uint32_t opcode, std::vector<dict_t>& new_threads) {
    Bv32::Operation op = details::extractOperation<Bv32::Operation>(opcode);
    std::size_t arity = Bv32::getOperationArity(op);
    switch (arity) {
    case 1:
        return execute(state, op, details::extractOperands<1>(opcode), new_threads);
    case 2:
        if (op == Bv32::Operation::const_) {
            std::size_t stack_idx = opcode & 0xb111;
            program::Word value = (opcode >> 8) & 0xffff;
            state.at(stack_idx)->push(value);
            break;
        }
        execute(state, op, details::extractOperands<2>(opcode));
        break;
    case 3:
        execute(state, op, details::extractOperands<3>(opcode));
        break;
    default:
        details::throwers::unsupportedArity(arity, "Bv32");
        break;
    }
    return interpreter::ThreadState::Running;
}
} // namespace

interpreter::ThreadState Bv32::call(const std::string& operation,
                                    const std::vector<std::string>& operands,
                                    program::SafeDict& state,
                                    std::vector<program::SafeDict>& new_threads) const /* override */ {
    return callCommon(state, Bv32::compile(operation, operands), new_threads);
}

interpreter::ThreadState Bv32::call(uint32_t operation_code,
                                    program::Dict& state,
                                    std::vector<program::Dict>& new_threads) const /* override */ {
    return callCommon(state, operation_code, new_threads);
}

uint32_t Bv32::compile(const std::string& operation, const std::vector<std::string>& operands) const
/* override */ {
    Operation op = operationFromName(operation);
    std::size_t arity = getOperationArity(op);
    details::checkThrowOperandsCount(arity, operands);

    if (op == Operation::const_)
        return (static_cast<uint32_t>(op) << 24) | (uint32_t(uint16_t(std::stoul(operands[0]))) << 8) |
               std::stoul(operands[1]);

    return details::compress(op, operands);
}

/* static */ Bv32::Operation Bv32::operationFromName(const std::string& name) {
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
    if (name == "const")
        return Operation::const_;
    if (name == "add")
        return Operation::add;
    if (name == "sub")
        return Operation::sub;
    if (name == "mul")
        return Operation::mul;
    if (name == "div")
        return Operation::div;
    if (name == "mod")
        return Operation::mod;
    if (name == "eq")
        return Operation::eq;
    if (name == "neq")
        return Operation::neq;
    if (name == "lt")
        return Operation::lt;
    if (name == "le")
        return Operation::le;
    if (name == "gt")
        return Operation::gt;
    if (name == "ge")
        return Operation::ge;
    if (name == "and")
        return Operation::and_;
    if (name == "xor")
        return Operation::xor_;
    if (name == "or")
        return Operation::or_;
    if (name == "neg")
        return Operation::neg;
    if (name == "land")
        return Operation::land;
    if (name == "lor")
        return Operation::lor;
    if (name == "guard_zero")
        return Operation::guard_zero;
    if (name == "guard_nonzero")
        return Operation::guard_nonzero;

    details::throwers::invalidOperation(name, "Bv32");
}
/* static */ std::size_t Bv32::getOperationArity(Operation op) {
    switch (op) {
    case Operation::zero:
    case Operation::drop:
    case Operation::guard_zero:
    case Operation::guard_nonzero:
        return 1;
    case Operation::move:
    case Operation::swap:
    case Operation::neg:
    case Operation::const_:
        return 2;
    case Operation::dup:
    case Operation::add:
    case Operation::sub:
    case Operation::mul:
    case Operation::div:
    case Operation::mod:
    case Operation::eq:
    case Operation::neq:
    case Operation::lt:
    case Operation::le:
    case Operation::gt:
    case Operation::ge:
    case Operation::and_:
    case Operation::xor_:
    case Operation::or_:
    case Operation::land:
    case Operation::lor:
        return 3;
    }

    details::throwers::invalidOperationCode(op, "Bv32");
}
} // namespace cthu::domains::builtins
