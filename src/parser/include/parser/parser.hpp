#pragma once

#include "program/safe_dict.hpp"

#include <istream>
#include <map>
#include <set>
#include <string>
#include <utility>

namespace cthu::parser {
class Parser {
  public:
    /**
     * @brief Parse source code from stream into program
     *
     * Throws std::runtime_error on failiure.
     *
     * @param[in,out] source_code            stream with source code
     * @return                               pair{ initial state; required domains }
     */
    std::pair<program::SafeDict, std::set<std::string>> parse(std::istream& source_code);

  private:
    using lines_t = std::vector<std::pair<std::size_t, std::string>>;

    std::unique_ptr<program::SafeStack> parseStack(const lines_t& lines);
    std::unique_ptr<program::SafeDict> parseDict(const lines_t& lines);

    std::map<std::string, lines_t> m_stack_lines;
    std::map<std::string, lines_t> m_dict_lines;
};
} // namespace cthu::parser