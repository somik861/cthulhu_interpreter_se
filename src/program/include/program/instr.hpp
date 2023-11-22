#pragma once

#include <format>
#include <string>
#include <vector>

namespace cthu::program {
class Instr {
  public:
    constexpr Instr(std::string domain, std::string operation, std::vector<std::string> operands) noexcept
        : domain(std::move(domain)),
          operation(std::move(operation)),
          operands(std::move(operands)),
          source_line(source_line) {}

    std::string toShortString(bool is_on_top = true) const {
        if (!is_on_top)
            return std::format("inst:{}", source_line);
        return ""; // TODO
    };

    std::string toJson(std::size_t indent = 0) const { return ""; }

    const std::string domain;
    const std::string operation;
    const std::vector<std::string> operands;
    const std::size_t source_line;
};
} // namespace cthu::program
