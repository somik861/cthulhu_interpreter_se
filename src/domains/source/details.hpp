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
        operands[N - i] = value & 0b111;
        value >>= 3;
    }

    return operands;
}

template <typename enum_t>
constexpr enum_t extractOperation(uint32_t value) noexcept {
    return static_cast<enum_t>(value >> 24);
}

template <typename enum_t>
void throwInvalidOperation(enum_t op, std::size_t arity) {
    throw std::invalid_argument(
        std::format("Invalid operation (code: {}) for arity {}", static_cast<uint32_t>(op), arity));
}
} // namespace cthu::domains::details
