#pragma once

#include "program/istack_item.h"

#include <format>

namespace cthu::program {
class Number : public virtual IStackItem {
  public:
    Number(uint32_t value) : m_value(value) {}
    uint32_t getValue() const { return m_value; }

    std::string toString(std::size_t indent = 0) const override {
        return std::string(" ", indent) + std::format("Number: {}", m_value);
    }

    std::unique_ptr<IStackItem> cloneItem() const override { return Number::createUnique(m_value); }

    static std::unique_ptr<Number> createUnique(uint32_t value) { return std::make_unique<Number>(value); }

  private:
    uint32_t m_value;
};
} // namespace cthu::program
