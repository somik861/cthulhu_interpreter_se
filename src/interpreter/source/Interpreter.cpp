#include "interpreter/Interpreter.hpp"
#include <builtin_structs/Dict.hpp>
#include <builtin_structs/Instruction.hpp>
#include <string>

namespace interpreter {
void Interpreter::loadStateFromStream(std::istream& stream) {
    std::vector<std::unique_ptr<builtin_structs::Instruction>> instructions;
    std::string line;
    for (std::size_t line_num = 1; std::getline(stream, line); ++line_num)
        instructions.emplace_back(
            std::make_unique<builtin_structs::Instruction>(builtin_structs::Instruction::fromString(line, line_num)));

    auto state = program::State::createEmptyUnique();
    for (auto it = instructions.rbegin(); it != instructions.rend(); ++it)
        state->dict->at(0)->push(std::move(*it));

    m_state_queue.push_back(std::move(state));
}
bool Interpreter::hasWork() const { return !m_state_queue.empty(); }
void Interpreter::run() {
    std::vector<std::unique_ptr<builtin_structs::Dict>> new_states;
    while (!m_state_queue.empty()) {
        // get current thread
        m_last_state_cache = std::move(m_state_queue.front());
        m_state_queue.pop_front();
        m_last_state = m_last_state_cache.get();

        // execute
        auto res = executeStateInstr(*m_last_state);

        // process result
        switch (res) {
        case builtin_structs::InstructionResult::OK:
            m_state_queue.push_back(std::move(m_last_state_cache));
        case builtin_structs::InstructionResult::END_ERROR:
        case builtin_structs::InstructionResult::END_SUCCESS:
            break;
        }
    }
}
const program::State* Interpreter::lastProcessedState() const { return m_last_state; }
builtin_structs::InstructionResult Interpreter::executeStateInstr(program::State& state) {
    auto* instr_stack = state.dict->at(0);
    if (instr_stack->size() == 0)
        return builtin_structs::InstructionResult::END_SUCCESS;

    std::vector<std::unique_ptr<builtin_structs::Dict>> new_states;
    auto instruction = instr_stack->pop<std::unique_ptr<builtin_structs::Instruction>>();
    auto finstruction = m_domains_manager.compile(*instruction);
    auto res = m_domains_manager.execute(finstruction, state, new_states);

    for (auto& new_state : new_states) {
        auto s = std::make_unique<program::State>();
        s->dict = std::move(new_state);
        m_state_queue.push_back(std::move(s));
    }
    return res;
}

} // namespace interpreter