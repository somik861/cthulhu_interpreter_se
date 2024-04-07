#pragma once

#include "domains/DomainManager.hpp"
#include "program/State.hpp"
#include "builtin_structs/FastInstruction.hpp"

#include <deque>
#include <istream>

namespace interpreter {
class Interpreter {
  public:
    void loadStateFromStream(std::istream& stream);
    bool hasWork() const;
    void run();
    const program::State* lastProcessedState() const;

  private:
    builtin_structs::InstructionResult executeStateInstr(program::State& state);

    std::deque<std::unique_ptr<program::State>> m_state_queue;
    std::unique_ptr<program::State> m_last_state_cache;
    domains::DomainManager m_domains_manager;
    program::State* m_last_state = nullptr;
};

} // namespace interpreter