#pragma once

#include "program/istack_item.h"

#include <memory>

namespace cthu::program {
class IStack;
}

namespace cthu::program::stack_items {
class StackRef() {
  public:
    StackRef(std::shared_ptr<IStack> dict) : m_stack(std::move(stack)) {}
    std::shared_ptr<IStack> getStack() const { return m_stack; }

  private:
    std::shared_ptr<IStack> m_dict;
}
} // namespace cthu::program::stack_items
