#pragma once

#include "interpreter/iinterpreter.h"

#include "utils/imap.h"
#include <deque>

namespace cthu::interpreter::impl {
class Interpreter : public virtual IInterpreter {
  public:
    void addBuiltin(const std::string& name, std::unique_ptr<builtins::IBuiltin> builtin) override;
    void initExecution(std::unique_ptr<program::IProgram> program, std::ostream* log_stream = nullptr) override;
    bool canContinue() const override;
    void continueExecution() override;
    ProgramState* getProgramState() override;
    ProgramState* getProgramState(std::size_t id) override;
    std::vector<ProgramState*> getFinishedStates(bool include_errors = false) override;
    std::unique_ptr<utils::ISet<std::size_t>> getRunningThreads() const override;
    std::size_t getMaxThreadID() const override;

  private:
    std::unique_ptr<utils::IMap<std::string, std::unique_ptr<builtins::IBuiltin>>> m_builtins =
        utils::IMap<std::string, std::unique_ptr<builtins::IBuiltin>>::createStdMap();
    std::unique_ptr<program::IProgram> m_program;
    std::ostream* m_log_stream = nullptr;

    // all threads in the queue has to be in Running state
    std::deque<ProgramState*> m_state_execution_queue;

    std::vector<std::unique_ptr<ProgramState>> m_thread_states;
};
} // namespace cthu::interpreter::impl
