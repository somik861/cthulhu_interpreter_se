#pragma once

#include <builtin_structs/FastInstruction.hpp>
#include <common/InterpreterException.hpp>
#include <format>
#include <string>
#include <string_view>
#include <vector>

namespace common::compile_utils {
class CompileException : public InterpreterException {
  public:
    using InterpreterException::InterpreterException;
};

inline void assertArgCount(const std::vector<std::string>& arg, std::size_t arg_count) {
    if (arg.size() != arg_count)
        throw CompileException(std::format("Invalid arg count, got: {}, expected: {}", arg.size(), arg_count));
}

inline int intFromString(std::string_view sv, bool* ok = nullptr) {
    std::size_t processed;
    int rv = std::stoi(std::string(sv), &processed);
    if (ok != nullptr)
        *ok = (processed == sv.size());
    return rv;
}

inline void compileIntArgs(builtin_structs::FastInstruction& instr,
                           const std::vector<std::string>& args,
                           std::size_t arg_count) {
    assertArgCount(args, arg_count);
    for (std::size_t i = 0; i < arg_count; ++i) {
        bool ok;
        instr.setOperand(i, uint8_t(intFromString(args[i], &ok)));
        if (!ok)
            throw CompileException(std::format("Can not convert '{}' to int", args[i]));
    }
}
} // namespace common::compile_utils