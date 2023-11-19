#include "dict.h"

#include <format>

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

std::unique_ptr<IDict> Dict::clone() const /* override */ {
    auto rv = IDict::createDict(m_map->clone());
    return rv;
}
std::unique_ptr<IDict> Dict::getEmpty() const /* override */ { return IDict::createDict(m_map->getEmpty()); }

std::string Dict::toString(std::size_t indent /* = 0 */) const /* override */ {
    std::string out(" ", indent);
    out += "==== DICT START ====\n";

    auto it = m_map->iterator();
    while (it->hasNext()) {
        const auto& [key, value] = it->next();
        out += std::string(" ", indent + 2) + std::format("{} =>\n", key);
        out += value->toString(indent + 2);
        out += '\n';
    }
    out += "==== DICT END ====";

    return out;
}

std::unique_ptr<IStackItem> Dict::cloneItem() const /* override */ { return clone(); }
} // namespace cthu::program::impl

namespace cthu::program {
/* static */ std::unique_ptr<IDict>
IDict::createDict(std::unique_ptr<mapping_type> backend /* = mapping_type::createStdMap() */) {
    return std::make_unique<impl::Dict>(std::move(backend));
}
} // namespace cthu::program
