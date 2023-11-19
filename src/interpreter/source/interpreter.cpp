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
    m_state_execution_queue.clear();

    // set init dict
    auto& state = m_thread_states.emplace_back(std::make_unique<ProgramState>());
    state->state_dict = m_program->init_dictionary->clone();
    state->next_line_number = program::stack_utils::peekInstruction(state->state_dict->at(0))->getSourceLine();

    // add thread to the queue
    m_state_execution_queue.push_back(state.get());
}
bool Interpreter::canContinue() const /* override */ { return !m_state_execution_queue.empty(); }
void Interpreter::continueExecution() /* override */ {
    ProgramState* state = m_state_execution_queue.front();
    m_state_execution_queue.pop_front();

    program::IStack* instruction_stack = state->state_dict->at(0);
    if (instruction_stack->empty()) {
        state->execution_state = ExecutionState::Finished;
        return;
    }

    auto instruction = program::stack_utils::popInstruction(instruction_stack);
    state->last_line_number = instruction->getSourceLine();
    std::vector<std::unique_ptr<program::IDict>> new_threads;
    state->execution_state = m_builtins->at(instruction->getBuiltinName())
                                 ->executeOperation(instruction->getOperation(), instruction->getOperands(),
                                                    state->state_dict.get(), new_threads);

    for (auto& thread : new_threads) {
        auto& new_state = m_thread_states.emplace_back(std::make_unique<ProgramState>());
        new_state->thread_id = m_thread_states.size() - 1;
        new_state->state_dict = std::move(thread);
        new_state->execution_state = ExecutionState::Running;
        new_state->last_line_number = 0;
        new_state->next_line_number =
            program::stack_utils::peekInstruction(new_state->state_dict->at(0))->getSourceLine();
        m_state_execution_queue.push_back(new_state.get());
    }

    if (state->execution_state == ExecutionState::Running) {
        m_state_execution_queue.push_back(state);
        state->next_line_number = program::stack_utils::peekInstruction(state->state_dict->at(0))->getSourceLine();
    }
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
        if (thread->execution_state == ExecutionState::Running)
            continue;
        if (thread->execution_state == ExecutionState::FinishedWithError && include_errors)
            out.push_back(thread.get());
        if (thread->execution_state == ExecutionState::Finished)
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
