
#include "program.h"

namespace cthu::program::impl {
std::unique_ptr<utils::ISet<std::string>> Program::getRequiredBuiltins() const /* override */ {
    return m_required_builtins->clone();
}
void Program::setRequriedBuiltins(std::unique_ptr<utils::ISet<std::string>> set) /* override */ {
    m_required_builtins = std::move(set);
}

std::unique_ptr<IDict> Program::getInitDictionary() const /* override */ { return m_init_dictionary->clone(); }
void Program::setInitDictionary(std::unique_ptr<IDict> dict) /* override */ { m_init_dictionary = std::move(dict); }
} // namespace cthu::program::impl

namespace cthu::program {
/* static */ std::unique_ptr<IProgram> IProgram::createProgram() { return std::make_unique<impl::Program>(); }
} // namespace cthu::program
