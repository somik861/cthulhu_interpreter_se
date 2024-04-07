#pragma once

#include <common/string_utils.hpp>
#include <string>
#include <vector>

namespace builtin_structs {
namespace details {} // namespace details

class Instruction {
  public:
    Instruction(std::string domain, std::string operation, std::vector<std::string> operands, std::size_t line_number)
        : m_domain(std::move(domain)),
          m_operation(std::move(operation)),
          m_operands(std::move(operands)),
          m_line_number(line_number) {}
    static Instruction fromString(std::string s, std::size_t line_number) {
        common::string_utils::trim(s);
        auto parts = common::string_utils::split(s, ' ');
        std::vector<std::string> args(parts.begin() + 2, parts.end());

        return Instruction(parts[0], parts[1], std::move(args), line_number);
    }

    const std::string& domain() const { return m_domain; }
    const std::string& operation() const { return m_operation; }
    const std::vector<std::string>& operands() const { return m_operands; }
    std::size_t line_number() const { return m_line_number; }
    std::string toString(int verbose_level) const {
        if (verbose_level <= 0)
            return "instr";
        std::string out = std::format("({}:{} {}", m_line_number, m_domain, m_operation);
        for (const auto& operand : m_operands)
            out += " " + operand;
        out += ")";
        return out;
    }

  private:
    std::string m_domain;
    std::string m_operation;
    std::vector<std::string> m_operands;
    std::size_t m_line_number;
};
} // namespace builtin_structs