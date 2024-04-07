#pragma once

#include "builtin_structs/Dict.hpp"
#include "common/InterpreterException.hpp"
#include <memory>
#include <ostream>

namespace program {
class StateException : public common::InterpreterException {
  public:
    using common::InterpreterException::InterpreterException;
};

class State {
  public:
    std::unique_ptr<builtin_structs::Dict> dict;

    static std::unique_ptr<State> createEmptyUnique() {
        auto ret = std::make_unique<State>();
        ret->dict = std::make_unique<builtin_structs::Dict>();
        for (int i = 0; i < 6; ++i)
            ret->dict->set(i, std::make_unique<builtin_structs::Stack>());
        return ret;
    }

    std::string toString(int verbose_level = 0) const { return dict->toString(verbose_level); }
};

} // namespace program