#include "interpreter_dummy.h"

namespace cthu::interpreter {
/* static */ std::unique_ptr<IInterpreter> IInterpreter::createInterpreterDummy() {
    return std::make_unique<impl::InterpreterDummy>();
}
} // namespace cthu::interpreter
