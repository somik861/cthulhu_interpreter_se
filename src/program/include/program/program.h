#pragma once

#include "program/idict.h"

#include "utils/iset.h"

#include <string>

namespace cthu::program {
class Program {
  public:
    std::unique_ptr<IDict> init_dictionary = nullptr;
    std::unique_ptr<utils::ISet<std::string>> required_builtins = nullptr;
};

} // namespace cthu::program
