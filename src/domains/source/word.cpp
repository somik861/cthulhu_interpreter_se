#include "domains/word.hpp"

#include "details.hpp"

namespace cthu::domains {

namespace {
template <typename dict_t>
constexpr interpreter::ThreadState execute(dict_t& state,
                                           Word::Operation operation,
                                           std::array<uint8_t, 1> args,
                                           std::vector<dict_t>& new_threads) {
    switch (operation) {
    case Word::Operation::drop:
        state.at(args[0])->pop<program::Word>();
        return interpreter::ThreadState::Running;
    case Word::Operation::zero:
        state.at(args[0])->push(program::Word(0));
        return interpreter::ThreadState::Running;
    case Word::Operation::guard_zero:
        return (state.at(args[0])->pop<program::Word>().value == 0) ? interpreter::ThreadState::Running
                                                                    : interpreter::ThreadState::Killed;
    case Word::Operation::guard_nonzero:
        return (state.at(args[0])->pop<program::Word>().value != 0) ? interpreter::ThreadState::Running
                                                                    : interpreter::ThreadState::Killed;
    }

    details::throwers::invalidOperationForArity(operation, 1);
}

template <typename dict_t>
constexpr void execute(dict_t& state, Word::Operation operation, std::array<uint8_t, 2> args) {
    switch (operation) {
    case Word::Operation::move:
        state.at(args[1])->push(state.at(args[0])->pop<program::Word>());
        return;
    case Word::Operation::swap: {
        auto fst = state.at(args[0])->pop<program::Word>();
        auto snd = state.at(args[1])->pop<program::Word>();
        state.at(args[0])->push(snd);
        state.at(args[1])->push(fst);
        return;
    }
    }
    details::throwers::invalidOperationForArity(operation, 2);
}

template <typename dict_t>
constexpr void execute(dict_t& state, Word::Operation operation, std::array<uint8_t, 3> args) {
    using word_t = program::Word;

    switch (operation) {
    case Word::Operation::dup: {
        auto elem = state.at(args[0])->pop<word_t>();
        state.at(args[1])->push(elem);
        state.at(args[2])->push(elem);
        return;
    }
    case Word::Operation::add:
        details::ops_templ::compute_binary<word_t>(state, args, [](auto lhs, auto rhs) {
            return word_t(lhs.value + rhs.value);
        });
        return;
    case Word::Operation::sub:
        details::ops_templ::compute_binary<word_t>(state, args, [](auto lhs, auto rhs) {
            return word_t(lhs.value - rhs.value);
        });
        return;
    case Word::Operation::mul:
        details::ops_templ::compute_binary<word_t>(state, args, [](auto lhs, auto rhs) {
            return word_t(lhs.value * rhs.value);
        });
        return;
    case Word::Operation::div:
        details::ops_templ::compute_binary<word_t>(state, args, [](auto lhs, auto rhs) {
            return word_t(lhs.value / rhs.value);
        });
        return;
    case Word::Operation::mod:
        details::ops_templ::compute_binary<word_t>(state, args, [](auto lhs, auto rhs) {
            return word_t(lhs.value % rhs.value);
        });
        return;
    case Word::Operation::eq:
        details::ops_templ::compute_binary<word_t>(state, args, [](auto lhs, auto rhs) {
            return word_t(lhs.value == rhs.value);
        });
        return;
    case Word::Operation::neq:
        details::ops_templ::compute_binary<word_t>(state, args, [](auto lhs, auto rhs) {
            return word_t(lhs.value != rhs.value);
        });
        return;
    case Word::Operation::lt:
        details::ops_templ::compute_binary<word_t>(state, args, [](auto lhs, auto rhs) {
            return word_t(lhs.value < rhs.value);
        });
        return;
    case Word::Operation::le:
        details::ops_templ::compute_binary<word_t>(state, args, [](auto lhs, auto rhs) {
            return word_t(lhs.value <= rhs.value);
        });
        return;
    case Word::Operation::gt:
        details::ops_templ::compute_binary<word_t>(state, args, [](auto lhs, auto rhs) {
            return word_t(lhs.value > rhs.value);
        });
        return;
    case Word::Operation::ge:
        details::ops_templ::compute_binary<word_t>(state, args, [](auto lhs, auto rhs) {
            return word_t(lhs.value >= rhs.value);
        });
        return;
    case Word::Operation::land:
        details::ops_templ::compute_binary<word_t>(state, args, [](auto lhs, auto rhs) {
            return word_t(lhs.value && rhs.value);
        });
        return;
    case Word::Operation::lor:
        details::ops_templ::compute_binary<word_t>(state, args, [](auto lhs, auto rhs) {
            return word_t(lhs.value || rhs.value);
        });
        return;
    }

    details::throwers::invalidOperationForArity(operation, 3);
}

template <typename dict_t>
constexpr interpreter::ThreadState callCommon(dict_t& state, uint32_t opcode, std::vector<dict_t>& new_threads) {
    Word::Operation op = details::extractOperation<Word::Operation>(opcode);
    std::size_t arity = Word::getOperationArity(op);
    switch (arity) {
    case 1:
        return execute(state, op, details::extractOperands<1>(opcode), new_threads);
    case 2:
        if (op == Word::Operation::const_) {
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
        details::throwers::unsupportedArity(arity, "Word");
        break;
    }
    return interpreter::ThreadState::Running;
}
} // namespace

constexpr interpreter::ThreadState Word::call(const std::string& operation,
                                              const std::vector<std::string>& operands,
                                              program::SafeDict& state,
                                              std::vector<program::SafeDict>& new_threads) const /* override */ {
    return callCommon(state, Word::compile(operation, operands), new_threads);
}

constexpr interpreter::ThreadState Word::call(uint32_t operation_code,
                                              program::Dict& state,
                                              std::vector<program::Dict>& new_threads) const /* override */ {
    return callCommon(state, operation_code, new_threads);
}

constexpr uint32_t Word::compile(const std::string& operation, const std::vector<std::string>& operands) const
/* override */ {
    Operation op = operationFromName(operation);
    std::size_t arity = getOperationArity(op);
    details::checkThrowOperandsCount(arity, operands);

    if (op == Operation::const_)
        return (static_cast<uint32_t>(op) << 24) | (uint32_t(uint16_t(std::stoul(operands[0]))) << 8) |
               std::stoul(operands[1]);

    return details::compress(op, operands);
}

constexpr /* static */ Word::Operation Word::operationFromName(const std::string& name) {
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
    if (name == "land")
        return Operation::land;
    if (name == "lor")
        return Operation::lor;
    if (name == "guard_zero")
        return Operation::guard_zero;
    if (name == "guard_nonzero")
        return Operation::guard_nonzero;

    details::throwers::invalidOperation(name, "Word");
}
constexpr /* static */ std::size_t Word::getOperationArity(Operation op) {
    switch (op) {
    case Operation::zero:
    case Operation::drop:
    case Operation::guard_zero:
    case Operation::guard_nonzero:
        return 1;
    case Operation::move:
    case Operation::swap:
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
    case Operation::land:
    case Operation::lor:
        return 3;
    }

    details::throwers::invalidOperationCode(op, "Word");
}

} // namespace cthu::domains
