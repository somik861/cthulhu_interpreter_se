#pragma once

#include <format>
#include <string>
#include <vector>

namespace cthu::program {
class Instr {
  public:
    constexpr Instr(std::string domain,
                    std::string operation,
                    std::vector<std::string> operands,
                    std::size_t source_line) noexcept
        : domain(std::move(domain)),
          operation(std::move(operation)),
          operands(std::move(operands)),
          source_line(source_line) {}

    std::string toShortString(bool is_on_top = true) const {
        if (!is_on_top)
            return std::format("instr:{}", source_line);
        std::string out = std::format("insturction:{} ({} {}", source_line, domain, operation);
        for (const auto& operand : operands)
            out += " " + operand;
        out += ")";
        return out;
    };

    std::string toJson(std::size_t indent = 0) const {
        std::string ws = std::string(indent, ' ');
        std::string out = ws + "{\"type\": \"instruction\", \"data\": {\n";
        out += ws + "  " + std::format("\"domain\": \"{}\",\n", domain);
        out += ws + "  " + std::format("\"operation\": \"{}\",\n", operation);
        out += ws + "  \"operands\": [";

        std::string delim = " ";
        for (const auto& operand : operands) {
            out += delim + "\"" + operand + "\"";
            delim = ", ";
        }
        out += "]\n";

        out += ws + "  " + std::format("\"source_line\": {}\n", source_line);
        out += ws + "}";
        return out;
    }

    const std::string domain;
    const std::string operation;
    const std::vector<std::string> operands;
    const std::size_t source_line;
};
} // namespace cthu::program

inline std::ostream& operator<<(std::ostream& stream, const cthu::program::Instr& instr) {
    stream << instr.toShortString();
    return stream;
}
