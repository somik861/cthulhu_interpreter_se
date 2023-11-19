#pragma once

#include "interpreter/iinterpreter.h"

namespace cthu::interpreter::impl {
class InterpreterDummy : public virtual IInterpreter {
  public:
    void addBuiltin(const std::string& name, std::unique_ptr<builtins::IBuiltin> builtin) override {}
    void initExecution(std::unique_ptr<program::IProgram> program, std::ostream* log_stream = nullptr) override {}
    bool canContinue() const override { return false; }
    void continueExecution() override {}
    ProgramState* getProgramState() override { return nullptr; }
    ProgramState* getProgramState(std::size_t id) override { return nullptr; }
    std::vector<ProgramState*> getFinishedStates(bool include_errors = false) override { return {}; };
    std::unique_ptr<utils::ISet<std::size_t>> getRunningThreads() const override { return nullptr; }
    std::size_t getMaxThreadID() const override { return 0; }
};
} // namespace cthu::interpreter::impl
