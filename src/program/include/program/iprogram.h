#pragma once

#include "program/idict.h"

#include "utils/iset.h"

#include <string>

namespace cthu::program {
class IProgram {
  public:
    virtual std::unique_ptr<utils::ISet<std::string>> getRequiredBuiltins() const = 0;
    virtual void setRequriedBuiltins(std::unique_ptr<utils::ISet<std::string>> set) = 0;

    virtual std::unique_ptr<IDict> getInitDictionary() const = 0;
    virtual void setInitDictionary(std::unique_ptr<IDict> dict) = 0;

    virtual ~IProgram() = default;
};

} // namespace cthu::program
