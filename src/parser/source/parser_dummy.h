#pragma once

#include "parser/iparser.h"

namespace cthu::parser::impl {
class ParserDummy : public virtual IParser {
  public:
    std::unique_ptr<program::IProgram> parse(std::istream& source_code) override { return nullptr; }
};
} // namespace cthu::parser::impl
