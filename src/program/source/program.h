#pragma once

#include "program/iprogram.h"

namespace cthu::program::impl {
class Program : public virtual IProgram {
  public:
    std::unique_ptr<utils::ISet<std::string>> getRequiredBuiltins() const override;
    void setRequriedBuiltins(std::unique_ptr<utils::ISet<std::string>> set) override;

    std::unique_ptr<IDict> getInitDictionary() const override;
    void setInitDictionary(std::unique_ptr<IDict> dict) override;

  private:
    std::unique_ptr<IDict> m_init_dictionary;
    std::unique_ptr<utils::ISet<std::string>> m_required_builtins;
};
} // namespace cthu::program::impl
