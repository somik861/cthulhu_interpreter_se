#pragma once

#include "program/istack.h"

#include <vector>

namespace cthu::program::impl {
class StackVector : public IStack {
  public:
    void push(std::unique_ptr<IStackItem> item) override;
    IStackItem* top() override;
    void pop() override;
    std::unique_ptr<IStackItem> poptop() override;
    std::size_t size() const override;
    bool empty() const override;
    void clear() override;
    std::unique_ptr<IStack> clone() const override;
    std::unique_ptr<IStackItem> cloneItem() const override;
    std::unique_ptr<IStack> getEmpty() const override;
    std::string toString(std::size_t indent = 0) const override;

  private:
    std::unique_ptr<StackVector> cloneRaw() const;

    std::vector<std::unique_ptr<IStackItem>> m_vector;
};
} // namespace cthu::program::impl
