#pragma once

#include <initializer_list>
#include <type_traits>

namespace cthu::domains::details {
template <typename  typename... operands_t>
    requires std::conjunction_v<std::is_integral<operands_t>...>
constexpr uint32_t squash_operands(operands_t... operands) {

}


template <typename enum_t, typename ... operands_t>
requires std::conjunction_v<std::is_integral<operands_t>...>
constexpr uint32_t compress(enum_t operation, operands_t... operands) {
    uint32_t res = 0;
    for (const auto& op : operands) {
        res |= op;
        res <<= 3;
    }

    return res | (static_cast<uint32_t>(operation) << 24);
}

} // namespace cthu::domains::details