#pragma once

#include "program/istack.h"

#include <vector>

namespace cthu::program::impl {
class StackVector : public IStack {
  public:
    void push(std::shared_ptr<IStackItem> item) override;
    std::shared_ptr<IStackItem> top() override;
    void pop() override;
    std::shared_ptr<IStackItem> poptop() override;
    std::size_t size() const override;
    bool empty() const override;
    void clear() override;
    std::shared_ptr<IStack> clone() const override;
    std::shared_ptr<IStack> getEmpty() const override;

  private:
    std::vector<std::shared_ptr<IStackItem>> m_vector;
};
} // namespace cthu::program::impl
