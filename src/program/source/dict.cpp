#include "dict.h"

#include <format>

namespace cthu::program::impl {
Dict::Dict(std::string name /* = "" */,
           std::unique_ptr<IDict::mapping_type> backend /* = mapping_type::createStdMap() */)
    : m_map(std::move(backend)),
      m_name(std::move(name)) {}

IStack* Dict::at(std::size_t idx) /* override */ {
    if (!m_map->contains(idx))
        throw std::out_of_range(std::format("dictionary does not contain index {}\n", idx));

    return m_map->at(idx).get();
}
const IStack* Dict::at(std::size_t idx) const /* override */ {
    if (!m_map->contains(idx))
        throw std::out_of_range(std::format("dictionary does not contain index {}\n", idx));

    return m_map->at(idx).get();
}
void Dict::set(std::size_t idx, std::unique_ptr<IStack> stack) /* override */ {
    m_map->insert(idx, std::move(stack), true);
}

bool Dict::contains(std::size_t idx) const /* override */ { return m_map->contains(idx); }
std::unique_ptr<IStack> Dict::pop(std::size_t idx) /* override */ {
    if (!m_map->contains(idx))
        throw std::out_of_range(std::format("dictionary does not contain index {}\n", idx));

    return m_map->pop(idx);
}

std::unique_ptr<utils::ISet<std::size_t>> Dict::getKeys() const /* override */ {
    auto set = utils::ISet<std::size_t>::createStdSet();

    auto iterator = m_map->iterator();
    while (iterator->hasNext())
        set->insert(iterator->next().first);
    return set;
}

std::unique_ptr<IDict> Dict::clone() const /* override */ {
    auto rv = IDict::createDict(m_name, m_map->clone());
    return rv;
}
std::unique_ptr<IDict> Dict::getEmpty() const /* override */ { return IDict::createDict("", m_map->getEmpty()); }

std::string Dict::toString(std::size_t indent /* = 0 */) const /* override */ {
    std::string out(indent, ' ');
    out += "==== DICT START ====\n";

    auto it = m_map->iterator();
    while (it->hasNext()) {
        const auto& [key, value] = it->next();
        out += std::string(indent + 2, ' ') + std::format("{} =>\n", key);
        out += value->toString(indent + 2);
        out += '\n';
    }
    out += std::string(indent, ' ') + "==== DICT END ====";

    return out;
}

std::string Dict::toShortString(bool is_on_top /* = true */) const /* override */ {
    if (!is_on_top)
        return std::string("dict") + (m_name.empty() ? "" : ":") + m_name;
    std::string out = m_name + "{\n";

    auto it = m_map->iterator();
    while (it->hasNext()) {
        const auto& [key, value] = it->next();
        out += std::format("  {} => ", key);
        out += value->toShortString(true);
        out += '\n';
    }
    out += "}";

    return out;
}

std::unique_ptr<IStackItem> Dict::cloneItem() const /* override */ { return clone(); }
} // namespace cthu::program::impl

namespace cthu::program {
/* static */ std::unique_ptr<IDict> IDict::createDict(
    std::string name /* = "" */,
    std::unique_ptr<mapping_type> backend /* = mapping_type::createStdMap() */) {
    return std::make_unique<impl::Dict>(std::move(name), std::move(backend));
}
} // namespace cthu::program
