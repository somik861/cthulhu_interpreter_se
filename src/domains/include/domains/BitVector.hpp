#pragma once

#include <builtin_structs/BitVector.hpp>
#include <common/compile_utils.hpp>
#include <common/execution_utils.hpp>
#include <domains/DomainManager.hpp>
#include <domains/IDomain.hpp>

#include <unordered_map>

namespace domains {
namespace details {

using namespace common::execution_utils;

template <typename bv_t>
class BitVector : public IDomain {
  public:
    BitVector(std::string name)
        : m_name(std::move(name)) {}
    std::string getName() const override { return m_name; }
    builtin_structs::FastInstruction compile(const builtin_structs::Instruction& instr,
                                             const DomainManager* domain_manager) const override {
        builtin_structs::FastInstruction finstr(uint8_t(domain_manager->getDomainID(m_name)));
        if (false) {
        } else if (m_argument_map.contains(instr.operation())) {
            auto [op, arg_count] = m_argument_map.at(instr.operation());
            finstr.data.operation = static_cast<uint8_t>(op);
            common::compile_utils::compileIntArgs(finstr, instr.operands(), arg_count);
        } else
            throw common::compile_utils::CompileException(std::format("Unknown operation: {}", instr.operation()));

        return finstr;
    }
    builtin_structs::InstructionResult execute(builtin_structs::FastInstruction finstr,
                                               program::State& state,
                                               std::vector<std::unique_ptr<builtin_structs::Dict>>& new_states,
                                               const DomainManager* domain_manager) const override {
        using sbv_t = std::make_signed_t<bv_t>;

        switch (static_cast<Operation>(finstr.data.operation)) {
        case Operation::zero:
            state.dict->at(finstr.data.operand_0)->push(bv_t(0));
            break;
        case Operation::one:
            state.dict->at(finstr.data.operand_0)->push(bv_t(1));
            break;
        case Operation::two:
            state.dict->at(finstr.data.operand_0)->push(bv_t(2));
            break;
        case Operation::add:
            binary_function<bv_t, bv_t>(state, finstr, [](bv_t lhs, bv_t rhs) { return bv_t(lhs + rhs); });
            break;
        case Operation::sub:
            binary_function<bv_t, bv_t>(state, finstr, [](bv_t lhs, bv_t rhs) { return bv_t(lhs - rhs); });
            break;
        case Operation::udiv:
            binary_function<bv_t, bv_t>(state, finstr, [](bv_t lhs, bv_t rhs) { return bv_t(lhs / rhs); });
            break;
        case Operation::sdiv:
            binary_function<bv_t, bv_t>(state, finstr, [](bv_t lhs, bv_t rhs) {
                return bv_t(sbv_t(lhs) / sbv_t(rhs));
            });
            break;
        case Operation::urem:
            binary_function<bv_t, bv_t>(state, finstr, [](bv_t lhs, bv_t rhs) { return bv_t(lhs % rhs); });
            break;
        case Operation::srem:
            binary_function<bv_t, bv_t>(state, finstr, [](bv_t lhs, bv_t rhs) {
                return bv_t(sbv_t(lhs) % sbv_t(rhs));
            });
            break;
        case Operation::mul:
            binary_function<bv_t, bv_t>(state, finstr, [](bv_t lhs, bv_t rhs) { return bv_t(lhs * rhs); });
            break;
        case Operation::and_:
            binary_function<bv_t, bv_t>(state, finstr, [](bv_t lhs, bv_t rhs) { return bv_t(lhs & rhs); });
            break;
        case Operation::xor_:
            binary_function<bv_t, bv_t>(state, finstr, [](bv_t lhs, bv_t rhs) { return bv_t(lhs ^ rhs); });
            break;
        case Operation::or_:
            binary_function<bv_t, bv_t>(state, finstr, [](bv_t lhs, bv_t rhs) { return bv_t(lhs | rhs); });
            break;
        case Operation::lshr:
            binary_function<bv_t, bv_t>(state, finstr, [](bv_t lhs, bv_t rhs) { return bv_t(lhs >> rhs); });
            break;
        case Operation::ashr:
            binary_function<bv_t, bv_t>(state, finstr, [](bv_t lhs, bv_t rhs) { return bv_t(sbv_t(lhs) >> rhs); });
            break;
        case Operation::shl:
            binary_function<bv_t, bv_t>(state, finstr, [](bv_t lhs, bv_t rhs) { return bv_t(lhs << rhs); });
            break;
        case Operation::neg:
            unary_function<bv_t>(state, finstr, [](bv_t arg) { return bv_t(~arg); });
            break;
        case Operation::drop:
            generic::drop<bv_t>(state, finstr);
            break;
        case Operation::move:
            generic::move<bv_t>(state, finstr);
            break;
        case Operation::dup:
            generic::dup<bv_t>(state, finstr);
            break;
        }

        return builtin_structs::InstructionResult::OK;
    }

  private:
    enum class Operation {
        zero = 0,
        one,
        two,
        add,
        sub,
        sdiv,
        udiv,
        srem,
        urem,
        mul,
        and_,
        xor_,
        or_,
        lshr,
        ashr,
        shl,
        neg,
        move,
        drop,
        dup
    };

    std::unordered_map<std::string, std::pair<Operation, uint8_t>> m_argument_map{{"zero", {Operation::zero, 1}},
                                                                                  {"one", {Operation::one, 1}},
                                                                                  {"two", {Operation::two, 1}},
                                                                                  {"add", {Operation::add, 3}},
                                                                                  {"sub", {Operation::sub, 3}},
                                                                                  {"sdiv", {Operation::sdiv, 3}},
                                                                                  {"udiv", {Operation::udiv, 3}},
                                                                                  {"srem", {Operation::srem, 3}},
                                                                                  {"urem", {Operation::urem, 3}},
                                                                                  {"mul", {Operation::mul, 3}},
                                                                                  {"and", {Operation::and_, 3}},
                                                                                  {"xor", {Operation::xor_, 3}},
                                                                                  {"lshr", {Operation::lshr, 3}},
                                                                                  {"ashr", {Operation::ashr, 3}},
                                                                                  {"shl", {Operation::shl, 3}},
                                                                                  {"neg", {Operation::neg, 2}},
                                                                                  {"drop", {Operation::drop, 1}},
                                                                                  {"dup", {Operation::dup, 3}},
                                                                                  {"move", {Operation::move, 2}}};

    std::string m_name;
};
} // namespace details

class BitVector8 : public details::BitVector<builtin_structs::BitVector8> {
  public:
    BitVector8()
        : details::BitVector<builtin_structs::BitVector8>("bv8") {}
};
class BitVector16 : public details::BitVector<builtin_structs::BitVector16> {
  public:
    BitVector16()
        : details::BitVector<builtin_structs::BitVector16>("bv16") {}
};
class BitVector32 : public details::BitVector<builtin_structs::BitVector32> {
  public:
    BitVector32()
        : details::BitVector<builtin_structs::BitVector32>("bv32") {}
};

} // namespace domains