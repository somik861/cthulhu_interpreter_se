#pragma once

#include "program/idict.h"

namespace cthu::program::impl {
class Dict : public virtual IDict {
  public:
    Dict(std::string name = "", std::unique_ptr<IDict::mapping_type> backend = mapping_type::createStdMap());

    IStack* at(std::size_t idx) override;
    const IStack* at(std::size_t idx) const override;
    void set(std::size_t idx, std::unique_ptr<IStack> stack) override;
    std::unique_ptr<utils::ISet<std::size_t>> getKeys() const override;
    std::unique_ptr<IDict> clone() const override;
    std::unique_ptr<IDict> getEmpty() const override;
    std::unique_ptr<IStackItem> cloneItem() const override;
    bool contains(std::size_t idx) const override;
    std::unique_ptr<IStack> pop(std::size_t idx) override;

    std::string toString(std::size_t indent = 0) const override;
    std::string toShortString(bool is_on_top = true) const override;

  private:
    std::unique_ptr<IDict::mapping_type> m_map = nullptr;
    std::string m_name;
};
} // namespace cthu::program::impl