#pragma once

#include <memory>

namespace cthu::program {
class IStackItem {
  public:
    virtual ~IStackItem() = default;
    virtual std::unique_ptr<IStackItem> cloneItem() const = 0;
};
} // namespace cthu::program
