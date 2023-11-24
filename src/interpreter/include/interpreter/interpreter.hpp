#pragma once

#include "domains/idomain.hpp"
#include "interpreter/mode.hpp"
#include "program/dict.hpp"
#include "program/fast_instruction.hpp"
#include "program/instruction.hpp"
#include "program/safe_dict.hpp"

#include <map>
#include <memory>
#include <set>
#include <type_traits>

namespace cthu::interpreter {
template <Mode mode>
class Interpreter {
  public:
    using dict_t = std::conditional_t<mode == Mode::Debug || mode == Mode::Normal, program::SafeDict, program::Dict>;
    using instruction_t =
        std::conditional_t<mode == Mode::Debug || mode == Mode::Normal, program::Instruction, program::FastInstruction>;

    constexpr void registerBuiltin(std::string name, std::unique_ptr<domains::IDomain> domain) {}
    constexpr void initExecution(std::unique_ptr<dict_t> state) {}
    constexpr bool canContinue() const noexcept { return false; }
    constexpr void continueExecution() {}
    constexpr std::set<std::size_t> getRunningThreads() const { return {}; }

    // getProgramState
    // getProgramState(idx)
    // getFinishedStates()

  private:
    std::vector<std::unique_ptr<domains::IDomain>> m_domains;
    std::map<std::string, std::size_t> m_domain_map;
};

} // namespace cthu::interpreter
