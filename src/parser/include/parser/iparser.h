#pragma once

#include "program/iprogram.h"

#include <istream>

namespace cthu::parser {

class IParser {
  public:
    virtual std::unique_ptr<program::IProgram> parse(std::istream& source_code) = 0;

    virtual ~IParser() = default;

    static std::unique_ptr<IParser> createParserDummy();
};
} // namespace cthu::parser
