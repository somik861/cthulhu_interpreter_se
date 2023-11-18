#pragma once

#include "parser/iparser.h"

#include "program/istack.h"

#include <vector>

namespace cthu::parser::impl {
class Parser : public virtual IParser {
  public:
    void parse(std::istream& source_code, program::IProgram* program) override;

  private:
    using lines_t = std::vector<std::pair<std::size_t, std::string>>;

    std::unique_ptr<program::IStack> parseStack(const lines_t& lines);
    std::unique_ptr<program::IDict> parseDict(const lines_t& lines);

    std::map<std::string, lines_t> m_stack_lines;
    std::map<std::string, lines_t> m_dict_lines;
};
} // namespace cthu::parser::impl
