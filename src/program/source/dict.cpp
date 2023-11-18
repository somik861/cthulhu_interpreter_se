#include "dict.h"

namespace cthu::program::impl {
Dict::Dict(std::unique_ptr<IDict::mapping_type> backend) : m_map(std::move(backend)) {}

IStack* Dict::at(std::size_t idx) /* override */ {
    if (m_map->contains(idx))
        return m_map->at(idx).get();
    return nullptr;
}
const IStack* Dict::at(std::size_t idx) const /* override */ {
    if (m_map->contains(idx))
        return m_map->at(idx).get();
    return nullptr;
}
void Dict::set(std::size_t idx, std::unique_ptr<IStack> stack) /* override */ {
    m_map->insert(idx, std::move(stack), true);
}
std::unique_ptr<utils::ISet<std::size_t>> Dict::getKeys() const /* override */ {
    auto set = utils::ISet<std::size_t>::createStdSet();

    auto iterator = m_map->iterator();
    while (iterator->hasNext())
        set->insert(iterator->next().first);
    return set;
}
std::unique_ptr<IDict> Dict::clone() const /* override */ { return IDict::createDict(m_map->clone()); }
std::unique_ptr<IDict> Dict::getEmpty() const /* override */ { return IDict::createDict(m_map->getEmpty()); }

std::unique_ptr<IStackItem> Dict::cloneItem() const /* override */ { return clone(); }
} // namespace cthu::program::impl

namespace cthu::program {
/* static */ std::unique_ptr<IDict>
IDict::createDict(std::unique_ptr<mapping_type> backend /* = mapping_type::createStdMap() */) {
    return std::make_unique<impl::Dict>(std::move(backend));
}
} // namespace cthu::program
