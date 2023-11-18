#pragma once

#include "program/istack_item.h"

namespace cthu::program::stack_items {
class Number : public virtual IStackItem {
  public:
    Number(uint32_t value) : m_value(value) {}
    uin32_t getValue() const { return m_value }

  private:
    uint32_t m_value;
};
} // namespace cthu::program::stack_items
