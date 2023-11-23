#pragma once

#include "details/utils.hpp"

#include "dict.hpp"
#include "safe_stack.hpp"

namespace cthu::program {
class SafeDict : public Dict {
  public:
    SafeStack* at(Word key) const noexcept { return details::toSafe(m_storage[key.value].get()); }
    std::unique_ptr<SafeStack> pop(Word key) noexcept { return details::toSafe(std::move(m_storage[key.value])); }
    void set(Word key, std::unique_ptr<SafeStack>&& stack) {
        if (m_storage.size() <= key.value)
            m_storage.resize((std::size_t(key.value) + 1) * 2);
        m_storage[key.value] = fromSafe(std::move(stack));
    }
};

static_assert(sizeof(SafeDict) == sizeof(Dict), "The safe dict has something that it should not have.");
}; // namespace cthu::program
