#pragma once

#include <format>
#include <string>

namespace cthu::program {

class Word {
  public:
    Word() = default;
    Word(uint32_t value)
        : value(value) {}

    uint32_t value;
    std::string toShortString(bool is_on_top = true) const { return std::to_string(value); }
    std::string toJson(std::size_t indent = 0) const {
        std::string ws = std::string(indent, ' ');
        std::string out = ws + "{\"type\": \"word\", \"data\": {\n";
        out += ws + "  " + std::format("\"value\": \"{}\"\n", value);
        out += ws + "}";
        return out;
    }
};

static_assert(sizeof(Word) == 4, "Word class is broken");
} // namespace cthu::program

inline std::ostream& operator<<(std::ostream& stream, const cthu::program::Word& word) {
    stream << word.toShortString();
    return stream;
}
