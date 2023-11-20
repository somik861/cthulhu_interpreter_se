#pragma once

#include <memory>
#include <string>

namespace cthu::program {
class IStackItem {
  public:
    virtual ~IStackItem() = default;
    virtual std::unique_ptr<IStackItem> cloneItem() const = 0;
    virtual std::string toString(std::size_t indent = 0) const = 0;
    virtual std::string toShortString(bool is_on_top = true) const = 0;
};
} // namespace cthu::program
