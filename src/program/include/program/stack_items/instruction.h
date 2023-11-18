#pragma once

#include "program/istack_item.h"

#include <string>
#include <vector>

namespace cthu::program::stack_items {
class Instruction : public virtual IStackItem {
  public:
    Instruction(std::string builtin, std::string operation, std::vector<std::string> operands, std::size_t source_line)
        : m_builtin(std::move(builtin)), m_operation(std::move(operation)), m_operands(std::move(operands)),
          m_source_line(source_line) {}

    const std::string& getBuiltinName() const { return m_builtin; }
    const std::string& getOperation() const { return m_operation; }
    const std::vector<std::string>& getOperands() const { return m_operands; }
    std::size_t getSourceLine() const { return m_source_line; }

  private:
    std::string m_builtin;
    std::string m_operation;
    std::vector<std::string> m_operands;
    std::size_t m_source_line;
};
} // namespace cthu::program::stack_items
