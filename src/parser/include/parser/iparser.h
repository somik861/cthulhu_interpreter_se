#pragma once

#include "program/iprogram.h"

#include <istream>

namespace cthu::parser {

class IParser {
  public:
    /**
     * @brief Parse source code from stream into program
     *
     * Throws std::runtime_error on failiure.
     *
     * @param[in,out] source_code            stream with source code
     * @oaran[out] program                   program to fill
     * @return                               program
     */
    virtual void parse(std::istream& source_code, program::IProgram* program) = 0;

    virtual ~IParser() = default;

    static std::unique_ptr<IParser> createParserDummy();
    static std::unique_ptr<IParser> createParser();
};
} // namespace cthu::parser
