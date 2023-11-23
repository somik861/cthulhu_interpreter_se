#pragma once

#include "program/details/utils.hpp"
#include "program/stack.hpp"
#include "program/word.hpp"

#include <format>
#include <memory>
#include <ostream>
#include <string>
#include <vector>

namespace cthu::program {
class Dict {
  public:
    // TODO implement operators
    constexpr Dict() noexcept = default;
    constexpr Dict(const Dict&){};
    constexpr Dict(Dict&&) noexcept {};
    constexpr Dict& operator=(const Dict&) { return *this; }
    constexpr Dict& operator=(Dict&&) noexcept { return *this; };
    ~Dict() noexcept = default;

    constexpr bool contains(Word key) const noexcept {
        return key.value < m_storage.size() && m_storage[key.value] != nullptr;
    }
    Stack* at(Word key) const noexcept { return m_storage[key.value].get(); }
    std::unique_ptr<Stack> pop(Word key) noexcept { return std::move(m_storage[key.value]); }
    void set(Word key, std::unique_ptr<Stack>&& stack) {
        if (m_storage.size() <= key.value)
            m_storage.resize((std::size_t(key.value) + 1) * 2);
        m_storage[key.value] = std::move(stack);
    }

    std::string toShortString(bool is_on_top = true) const { return ""; };
    std::string toJson(std::size_t indent = 0) const { return ""; }

  protected:
    std::vector<std::unique_ptr<Stack>> m_storage;
};
} // namespace cthu::program

inline std::ostream& operator<<(std::ostream& stream, const cthu::program::Dict& dict) {
    stream << dict.toShortString();
    return stream;
}
