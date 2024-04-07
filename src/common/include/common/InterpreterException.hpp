#pragma once

#include <stdexcept>

namespace common {
class InterpreterException : public std::runtime_error {
  public:
	InterpreterException(const std::string& what) : std::runtime_error(what) {}
	InterpreterException(const char* what) : std::runtime_error(what) {}
};

} // namespace common