#pragma once

#include "program/details/utils.hpp"
#include "program/stack.hpp"
#include "program/word.hpp"

#include <algorithm>
#include <format>
#include <memory>
#include <ostream>
#include <string>
#include <vector>

namespace cthu::program {
class Dict {
  public:
    constexpr Dict() noexcept = default;
    constexpr Dict(const Dict& o) {
        m_storage.reserve(o.m_storage.size());
        for (const auto& elem : o.m_storage)
            if (elem == nullptr)
                m_storage.emplace_back();
            else
                m_storage.emplace_back(new Stack(*elem));
    };
    constexpr Dict(Dict&&) noexcept = default;
    constexpr Dict& operator=(const Dict& o) {
        if (m_storage.size() < o.m_storage.size())
            m_storage.resize(o.m_storage.size());

        std::transform(o.m_storage.begin(),
                       o.m_storage.end(),
                       m_storage.begin(),
                       [](const auto& elem) -> std::unique_ptr<Stack> {
                           if (elem == nullptr)
                               return nullptr;
                           return std::make_unique<Stack>(*elem);
                       });
        return *this;
    }
    constexpr Dict& operator=(Dict&&) noexcept = default;
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
