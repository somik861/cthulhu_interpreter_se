#pragma once

#include <format>
#include <string>

namespace cthu::program {
// The bottom-most 3 bits are ignored, so the Stack can do its fast optimisations;
class FastInstruction {
  public:
    uint8_t empty_space = 0; // << make sure this is always in the beginning
    uint8_t domain_code = 0;
    uint16_t unused_space = 0;
    uint32_t operation_code = 0;

    std::string toShortString(bool is_on_top = true) const {
        return std::format("finstr({:x}:{:x})", domain_code, operation_code);
    };
    std::string toJson(std::size_t indent = 0) const {
        std::string ws = std::string(indent, ' ');
        std::string out = ws + "{\"type\": \"fast instruction\", \"data\": {\n";
        out += ws + "  " + std::format("\"domain_code\": {},\n", domain_code);
        out += ws + "  " + std::format("\"instruction_code\": {}\n", operation_code);
        out += ws + "}";
        return out;
    }
};

// make sure that sizeof(FastInstruction) is 8
static_assert(
    sizeof(FastInstruction) == 8,
    "sizeof(FastInstruction) is not 8, there is either something very wrong with the code, or your machine/compiler");
} // namespace cthu::program

inline std::ostream& operator<<(std::ostream& stream, const cthu::program::FastInstruction& instr) {
    stream << instr.toShortString();
    return stream;
}
