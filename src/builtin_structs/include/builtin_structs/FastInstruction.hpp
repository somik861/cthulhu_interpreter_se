#pragma once

#include "common/InterpreterException.hpp"
#include <cstdint>
#include <ostream>

namespace builtin_structs {
class FastInstructionException : public common::InterpreterException {
  public:
    using common::InterpreterException::InterpreterException;
};

enum class InstructionResult { OK, END_SUCCESS, END_ERROR };

class FastInstruction {
  public:
    FastInstruction() = default;
    FastInstruction(uint8_t domain) { data.domain = domain; }
    FastInstruction(uint8_t domain, uint8_t operation)
        : FastInstruction(domain) {
        data.operation = operation;
    }
    FastInstruction(uint8_t domain, uint8_t operation, uint8_t operand_0)
        : FastInstruction(domain, operation) {
        setOperand(0, operand_0);
    }
    FastInstruction(uint8_t domain, uint8_t operation, uint8_t operand_0, uint8_t operand_1)
        : FastInstruction(domain, operation, operand_0) {
        setOperand(1, operand_1);
    }
    FastInstruction(uint8_t domain, uint8_t operation, uint8_t operand_0, uint8_t operand_1, uint8_t operand_2)
        : FastInstruction(domain, operation, operand_0, operand_1) {
        setOperand(2, operand_2);
    }
    FastInstruction(uint8_t domain,
                    uint8_t operation,
                    uint8_t operand_0,
                    uint8_t operand_1,
                    uint8_t operand_2,
                    uint8_t operand_3)
        : FastInstruction(domain, operation, operand_0, operand_1, operand_2) {
        setOperand(3, operand_3);
    }
    FastInstruction(uint8_t domain,
                    uint8_t operation,
                    uint8_t operand_0,
                    uint8_t operand_1,
                    uint8_t operand_2,
                    uint8_t operand_3,
                    uint8_t operand_4)
        : FastInstruction(domain, operation, operand_0, operand_1, operand_2, operand_3) {
        setOperand(4, operand_4);
    }
    FastInstruction(uint8_t domain,
                    uint8_t operation,
                    uint8_t operand_0,
                    uint8_t operand_1,
                    uint8_t operand_2,
                    uint8_t operand_3,
                    uint8_t operand_4,
                    uint8_t operand_5)
        : FastInstruction(domain, operation, operand_0, operand_1, operand_2, operand_3, operand_4) {
        setOperand(5, operand_5);
    }

    void setCustom(uint32_t value) { data.custom = value; }
    void setOperand(std::size_t index, uint8_t value) {
        if (!fitsIn(value, 3))
            throw FastInstructionException("Operand does not fit into its number of bits");

        if (index > 5)
            throw FastInstructionException("Index of operand out of range");

        switch (index) {
        case 0:
            data.operand_0 = value;
            break;
        case 1:
            data.operand_1 = value;
            break;
        case 2:
            data.operand_2 = value;
            break;
        case 3:
            data.operand_3 = value;
            break;
        case 4:
            data.operand_4 = value;
            break;
        case 5:
            data.operand_5 = value;
            break;
        }
    }

    /**
     * Standard layout:
     * domain (6) | operation (8) | operands (6 * 3) | custom (32) -> 64
     */
    struct {
        uint32_t domain : 6;
        uint32_t operation : 8;
        uint32_t operand_0 : 3;
        uint32_t operand_1 : 3;
        uint32_t operand_2 : 3;
        uint32_t operand_3 : 3;
        uint32_t operand_4 : 3;
        uint32_t operand_5 : 3;
        uint32_t custom;
    } data{};

  private:
    template <typename T>
    bool fitsIn(T value, uint8_t bits) {
        return (value >> bits) == 0;
    }
};

static_assert(sizeof(FastInstruction) <= 8);

} // namespace builtin_structs
inline std::ostream& operator<<(std::ostream& os, const builtin_structs::FastInstruction& instr) {
    os << "finstr";
    return os;
}