#pragma once

#include <domains/BitVector.hpp>
#include <domains/Dictionary.hpp>
#include <domains/DomainManager.hpp>
#include <domains/IDomain.hpp>
#include <domains/Sequence.hpp>

#include <memory>

namespace domains {
DomainManager::DomainManager() {
    m_domains[0] = std::make_unique<domains::BitVector8>();
    m_domains[1] = std::make_unique<domains::BitVector16>();
    m_domains[2] = std::make_unique<domains::BitVector32>();
    m_domains[3] = std::make_unique<domains::Sequence>();
    m_domains[3] = std::make_unique<domains::Dictionary>();
}
std::size_t DomainManager::getDomainID(std::string_view domain) const {
    for (std::size_t i = 0; i < m_domains.size(); ++i)
        if (m_domains[i]->getName() == domain)
            return i;

    throw DomainManagerException(std::format("Domain '{}' does not exist", domain));
}
IDomain* DomainManager::getDomain(std::string_view domain) const { return getDomain(getDomainID(domain)); }
IDomain* DomainManager::getDomain(std::size_t index) const {
    if (index >= m_domains.size())
        throw DomainManagerException(std::format("Domain at index {} not found", index));
    return m_domains[index].get();
}

builtin_structs::InstructionResult DomainManager::execute(
    builtin_structs::FastInstruction instruction,
    program::State& state,
    std::vector<std::unique_ptr<builtin_structs::Dict>>& new_states) const {
    return m_domains[instruction.data.domain]->execute(instruction, state, new_states, this);
}

builtin_structs::FastInstruction DomainManager::compile(const builtin_structs::Instruction& instruction) const {
    return getDomain(instruction.domain())->compile(instruction, this);
}

DomainManager::~DomainManager() = default;

} // namespace domains