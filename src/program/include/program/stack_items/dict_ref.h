#pragma once

#include "program/istack_item.h"

#include <memory>

namespace cthu::program {
class IDict;
}

namespace cthu::program::stack_items {
class DictRef() {
  public:
    DictRef(std::shared_ptr<IDict> dict) : m_dict(std::move(dict)) {}
    std::shared_ptr<IDict> getDict() const { return m_dict; }

  private:
    std::shared_ptr<IDict> m_dict;
}
} // namespace cthu::program::stack_items
