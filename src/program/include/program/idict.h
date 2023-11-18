#pragma once

#include "istack_item.h"

namespace cthu::program {
class IDict : public virtual IStackItem {
  public:
    virtual ~IDict() = default;
};
} // namespace cthu::program
