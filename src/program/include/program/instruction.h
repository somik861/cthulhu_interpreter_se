#pragma once

#include "program/istack_item.h"

#include <format>
#include <string>
#include <vector>

namespace cthu::program {
class Instruction : public virtual IStackItem {
  public:
    Instruction(std::string builtin, std::string operation, std::vector<std::string> operands, std::size_t source_line)
        : m_builtin(std::move(builtin)), m_operation(std::move(operation)), m_operands(std::move(operands)),
          m_source_line(source_line) {}

    const std::string& getBuiltinName() const { return m_builtin; }
    const std::string& getOperation() const { return m_operation; }
    const std::vector<std::string>& getOperands() const { return m_operands; }
    std::size_t getSourceLine() const { return m_source_line; }

    std::string toString(std::size_t indent = 0) const override {
        auto out =
            std::string(" ", indent) + std::format("{}:{} instruction: {}", m_source_line, m_builtin, m_operation);
        for (const auto& op : m_operands) {
            out += " ";
            out += op;
        }

        return out;
    }

    std::unique_ptr<IStackItem> cloneItem() const override {
        return createUnique(m_builtin, m_operation, m_operands, m_source_line);
    }

    static std::unique_ptr<Instruction> createUnique(std::string builtin,
                                                     std::string operation,
                                                     std::vector<std::string> operands,
                                                     std::size_t source_line) {
        return std::make_unique<Instruction>(std::move(builtin), std::move(operation), std::move(operands),
                                             source_line);
    }

  private:
    std::string m_builtin;
    std::string m_operation;
    std::vector<std::string> m_operands;
    std::size_t m_source_line;
};
} // namespace cthu::program
