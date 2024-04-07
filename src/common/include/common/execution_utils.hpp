#pragma once

#include <builtin_structs/FastInstruction.hpp>
#include <program/State.hpp>

namespace common::execution_utils {
template <typename arg0_t, typename func_t>
void unary_function(program::State& state, const builtin_structs::FastInstruction& finstr, func_t fun) {
    auto arg = state.dict->at(finstr.data.operand_0)->pop<arg0_t>();
    state.dict->at(finstr.data.operand_2)->push(fun(arg));
}

template <typename arg0_t, typename arg1_t, typename func_t>
void binary_function(program::State& state, const builtin_structs::FastInstruction& finstr, func_t fun) {
    auto lhs = state.dict->at(finstr.data.operand_0)->pop<arg0_t>();
    auto rhs = state.dict->at(finstr.data.operand_1)->pop<arg1_t>();
    state.dict->at(finstr.data.operand_2)->push(fun(lhs, rhs));
}

namespace generic {
template <typename arg_t>
void move(program::State& state, const builtin_structs::FastInstruction& finstr) {
    auto val = state.dict->at(finstr.data.operand_0)->pop<arg_t>();
    state.dict->at(finstr.data.operand_1)->push(std::move(val));
}
template <typename arg_t>
void drop(program::State& state, const builtin_structs::FastInstruction& finstr) {
    state.dict->at(finstr.data.operand_0)->pop<arg_t>();
}
template <typename arg_t>
void dup(program::State& state, const builtin_structs::FastInstruction& finstr) {
    auto val = state.dict->at(finstr.data.operand_0)->pop<arg_t>();
    if constexpr (common::traits::is_unique_ptr_v<arg_t>) {
        state.dict->at(finstr.data.operand_1)->push(std::make_unique<typename arg_t::element_type>(*val));
        state.dict->at(finstr.data.operand_2)->push(std::move(val));
    } else {
        state.dict->at(finstr.data.operand_2)->push(val);
        state.dict->at(finstr.data.operand_2)->push(val);
    }
}
} // namespace generic
} // namespace common::execution_utils