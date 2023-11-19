#include "interpreter.h"

#include "program/stack_utils.h"

#include <cassert>
#include <format>

namespace cthu::interpreter::impl {
void Interpreter::addBuiltin(const std::string& name, std::unique_ptr<builtins::IBuiltin> builtin) /* override */ {
    assert(m_builtins->insert(name, std::move(builtin)));
}
void Interpreter::initExecution(std::unique_ptr<program::Program> program,
                                std::ostream* log_stream /* = nullptr */) /* override */ {
    m_program = std::move(program);
    m_log_stream = log_stream;

    auto iter = m_program->required_builtins->iterator();
    while (iter->hasNext()) {
        std::string builtin = iter->next();

        if (!m_builtins->contains(builtin))
            throw std::runtime_error(std::format("{} builtin is not supported", builtin));
    }

    // clear queue
    m_state_execution_queue = decltype(m_state_execution_queue){};

    // set init dict
    auto& state = m_thread_states.emplace_back(std::make_unique<ProgramState>());
    state->state_dict = m_program->init_dictionary->clone();

    // add thread to the queue
    m_state_execution_queue.push_back(state.get());
}
bool Interpreter::canContinue() const /* override */ { return !m_state_execution_queue.empty(); }
void Interpreter::continueExecution() /* override */ {
    ProgramState* state = m_state_execution_queue.front();
    m_state_execution_queue.pop_front();

    program::IStack* instruction_stack = state->state_dict->at(0);
    if (instruction_stack->empty()) {
        state->execution_state = ProgramState::State::Finished;
        return;
    }

    auto instruction = program::stack_utils::popInstruction(instruction_stack);
    m_builtins->at(instruction->getBuiltinName())
        ->executeOperation(instruction->getOperation(), instruction->getOperands(), state);

    if (state->execution_state == ProgramState::State::Running)
        m_state_execution_queue.push_back(state);
}
ProgramState* Interpreter::getProgramState() /* override */ { return m_state_execution_queue.front(); }
ProgramState* Interpreter::getProgramState(std::size_t id) /* override */ { return m_thread_states[id].get(); }
std::unique_ptr<utils::ISet<std::size_t>> Interpreter::getRunningThreads() const /* override */ {
    auto set = utils::ISet<std::size_t>::createStdSet();
    for (const auto& thread : m_state_execution_queue)
        set->insert(thread->thread_id);

    return set;
}

std::vector<ProgramState*> Interpreter::getFinishedStates(bool include_errors /* = false */) /* override */ {
    std::vector<ProgramState*> out;

    for (auto& thread : m_thread_states) {
        if (thread->execution_state == ProgramState::State::Running)
            continue;
        if (thread->execution_state == ProgramState::State::FinishedWithError && include_errors)
            out.push_back(thread.get());
        if (thread->execution_state == ProgramState::State::Finished)
            out.push_back(thread.get());
    }

    return out;
}
std::size_t Interpreter::getMaxThreadID() const /* override */ { return m_thread_states.size() - 1; }
} // namespace cthu::interpreter::impl

namespace cthu::interpreter {
/* static */ std::unique_ptr<IInterpreter> IInterpreter::createInterpreter() {
    return std::make_unique<impl::Interpreter>();
}
} // namespace cthu::interpreter
