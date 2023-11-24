#pragma once

#include "compiler/to_fast_code.hpp"
#include "domains/idomain.hpp"
#include "interpreter/mode.hpp"
#include "interpreter/program_state.hpp"
#include "program/dict.hpp"
#include "program/fast_instruction.hpp"
#include "program/instruction.hpp"
#include "program/safe_dict.hpp"

#include <cassert>
#include <deque>
#include <format>
#include <map>
#include <memory>
#include <set>
#include <type_traits>

namespace cthu::interpreter {
namespace details {
template <Mode mode>
struct InterpreterData;

template <>
struct InterpreterData<Mode::Debug> {
    std::size_t max_thread_id;
};

template <>
struct InterpreterData<Mode::Normal> : public InterpreterData<Mode::Debug> {};

template <>
struct InterpreterData<Mode::Fast> {
    std::size_t max_thread_id;
};

template <>
struct InterpreterData<Mode::Parallel> {
    std::atomic_size_t max_thread_id;
};
} // namespace details

template <Mode mode>
class Interpreter {
  public:
    constexpr static bool fast_mode = (mode == Mode::Fast || mode == Mode::Parallel);
    using dict_t = std::conditional_t<fast_mode, program::SafeDict, program::Dict>;
    using instruction_t = std::conditional_t<fast_mode, program::Instruction, program::FastInstruction>;
    using program_state_t = ProgramState<dict_t>;
    using state_container_t = std::set<std::unique_ptr<program_state_t>>;
    using state_iterator_t = state_container_t::iterator;

    constexpr void registerDomain(std::string name, std::unique_ptr<domains::IDomain> domain) {
        assert(!m_domain_map.contains(name));
        m_domain_map[name] = m_domains.size();
        m_domains.push_back(std::move(domain));
    }
    constexpr void initExecution(const program::SafeDict& state, const std::set<std::string>& required_domains) {
        for (const auto& domain : required_domains)
            if (!m_domain_map.contains(domain))
                throw std::runtime_error(std::format("Domain {} is not supported by the interpreter"));

        auto [iterator, inserted] = m_running_states.emplace(new program_state_t());
        assert(inserted);

        if constexpr (!fast_mode)
            (*iterator)->state = state;
        else
            (*iterator)->state = compiler::to_fast_code(state, m_domain_map, m_domains);

        m_process_queue.push_back(iterator);
    }
    constexpr bool canContinue() const noexcept { return !m_process_queue.empty(); }
    constexpr void continueExecution() {
        state_iterator_t state_it = m_process_queue.front();
        m_process_queue.pop_front();

        auto program_state = state_it->get();
        while (true) {
            auto instruction_stack = program_state->state.at(0);
            if (instruction_stack->empty()) {
                program_state->thread_state = ThreadState::Finished;
                break;
            }
            if constexpr (!fast_mode) {
                // slow execution
            } else {
                // fast execution
            }

            if constexpr (mode == Mode::Debug)
                break;
        }

        switch (program_state->thread_state) {
        case ThreadState::Running:
            m_stop_requester = program_state;
            m_process_queue.push_front(state_it); // prefer the same thread next time
            break;
        case ThreadState::Finished:
            m_stop_requester = program_state;
            m_finished_states.push_back(std::move(m_running_states.extract(state_it).value()));
            break;
        case ThreadState::Killed:
            m_stop_requester = nullptr;
            m_running_states.erase(state_it);
            break;
        }
    }
    constexpr std::set<std::size_t> getRunningThreads() const {
        std::set<std::size_t> out;
        for (const auto& state : m_running_states)
            out.insert(state->thread_id);
        return out;
    }

    program_state_t* getProgramState() { return m_stop_requester; }
    program_state_t* getProgramState(std::size_t idx) { return nullptr; }
    std::vector<program_state_t*> getFinishedStates() {
        std::vector<program_state_t*> out;
        for (const auto& state : m_finished_states)
            out.push_back(state.get());
        return out;
    }

  private:
    std::vector<std::unique_ptr<domains::IDomain>> m_domains;
    std::map<std::string, std::size_t> m_domain_map;
    state_container_t m_running_states;
    std::vector<std::unique_ptr<program_state_t>> m_finished_states;
    std::deque<state_iterator_t> m_process_queue;
    program_state_t* m_stop_requester = nullptr;
};

} // namespace cthu::interpreter
