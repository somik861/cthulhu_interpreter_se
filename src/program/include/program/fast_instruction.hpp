#pragma once

#include <format>
#include <string>

namespace cthu::program {
// The bottom-most 3 bits are ignored, so the Stack can do its fast optimisations;
class FastInstruction {
  public:
    uint64_t value = 0;

    std::string toShortString(bool is_on_top = true) const { return std::format("instr_fast({})", value); };
    std::string toJson(std::size_t indent = 0) const {
        std::string ws = std::string(indent, ' ');
        std::string out = ws + "{\"type\": \"fast instruction\", \"data\": {\n";
        out += ws + "  " + std::format("\"value\": \"{:x}\",\n", value);
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
