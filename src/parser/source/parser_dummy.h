#pragma once

#include "parser/iparser.h"

namespace cthu::parser::impl {
class ParserDummy : public virtual IParser {
  public:
    void parse(std::istream& source_code, program::IProgram* program) override { return; }
};
} // namespace cthu::parser::impl
