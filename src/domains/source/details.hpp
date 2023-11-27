#pragma once

#include <array>
#include <format>
#include <initializer_list>
#include <type_traits>

namespace cthu::domains::details {
inline constexpr uint32_t squash_operands() noexcept { return 0; }
template <typename first_t, typename... operands_t>
    requires std::conjunction_v<std::is_integral<first_t>, std::is_integral<operands_t>...>
constexpr uint32_t squash_operands(first_t first, operands_t... operands) noexcept {
    return static_cast<uint32_t>(first) << (3 * sizeof...(operands_t)) | squash_operands(operands...);
}

template <typename enum_t, typename... operands_t>
    requires std::conjunction_v<std::is_integral<operands_t>...>
constexpr uint32_t compress(enum_t operation, operands_t... operands) {
    return (static_cast<uint32_t>(operation) << 24) | squash_operands(operands...);
}

constexpr void checkThrowOperandsCount(std::size_t size, const std::vector<std::string>& vec) {
    if (vec.size() != size)
        throw std::invalid_argument(std::format("Invalid operands count, got: {}, expected: {}", size, vec.size()));
}
template <typename enum_t>
constexpr uint32_t compress(enum_t operation, const std::vector<std::string>& vec) {
    switch (vec.size()) {
    case 0:
        return compress(operation);
    case 1:
        return compress(operation, std::stoul(vec[0]));
    case 2:
        return compress(operation, std::stoul(vec[0]), std::stoul(vec[1]));
    case 3:
        return compress(operation, std::stoul(vec[0]), std::stoul(vec[1]), std::stoul(vec[2]));
    case 4:
        return compress(operation, std::stoul(vec[0]), std::stoul(vec[1]), std::stoul(vec[2]), std::stoul(vec[3]));
    case 5:
        return compress(operation,
                        std::stoul(vec[0]),
                        std::stoul(vec[1]),
                        std::stoul(vec[2]),
                        std::stoul(vec[3]),
                        std::stoul(vec[4]));
    case 6:
        return compress(operation,
                        std::stoul(vec[0]),
                        std::stoul(vec[1]),
                        std::stoul(vec[2]),
                        std::stoul(vec[3]),
                        std::stoul(vec[4]),
                        std::stoul(vec[5]));
    case 7:
        return compress(operation,
                        std::stoul(vec[0]),
                        std::stoul(vec[1]),
                        std::stoul(vec[2]),
                        std::stoul(vec[3]),
                        std::stoul(vec[4]),
                        std::stoul(vec[5]),
                        std::stoul(vec[6]));
    case 8:
        return compress(operation,
                        std::stoul(vec[0]),
                        std::stoul(vec[1]),
                        std::stoul(vec[2]),
                        std::stoul(vec[3]),
                        std::stoul(vec[4]),
                        std::stoul(vec[5]),
                        std::stoul(vec[6]),
                        std::stoul(vec[7]));
    default:
        throw std::runtime_error(std::format("Unsupported operand count: {}", vec.size()));
    }
}

template <std::size_t N>
constexpr std::array<uint8_t, N> extractOperands(uint32_t value) noexcept {
    std::array<uint8_t, N> operands;
    for (std::size_t i = 0; i < N; ++i) {
        operands[N - i - 1] = value & 0b111;
        value >>= 3;
    }

    return operands;
}

template <typename enum_t>
constexpr enum_t extractOperation(uint32_t value) noexcept {
    return static_cast<enum_t>(value >> 24);
}

namespace throwers {
inline void invalidOperation(const std::string& name, const std::string& domain) {
    throw std::invalid_argument(std::format("Operation {} is not supported in domain {}", name, domain));
}

template <typename enum_t>
void invalidOperationCode(enum_t operation, const std::string& name) {
    throw std::invalid_argument(
        std::format("Invalid operation code: {} for domain {}", static_cast<uint32_t>(operation), name));
}

template <typename enum_t>
void invalidOperationForArity(enum_t op, std::size_t arity) {
    throw std::invalid_argument(
        std::format("Invalid operation (code: {}) for arity {}", static_cast<uint32_t>(op), arity));
}

inline void unsupportedArity(std::size_t arity, const std::string& domain) {
    throw std::invalid_argument(std::format("Unsupported arity {} for domain {}", arity, domain));
}

} // namespace throwers

namespace ops_templ {
template <typename val_t, typename dict_t, typename fun_t>
constexpr void compute_unary(dict_t& state, std::array<uint8_t, 2> args, fun_t fun) {
    auto arg = state.at(args[0])->pop<val_t>();

    state.at(args[1])->push(fun(arg));
}

template <typename val_t, typename dict_t, typename fun_t>
constexpr void compute_binary(dict_t& state, std::array<uint8_t, 3> args, fun_t fun) {
    auto lhs = state.at(args[0])->pop<val_t>();
    auto rhs = state.at(args[1])->pop<val_t>();

    state.at(args[2])->push(fun(lhs, rhs));
}
} // namespace ops_templ

} // namespace cthu::domains::details
