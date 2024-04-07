#include "builtin_structs/Dict.hpp"

namespace builtin_structs {
Dict::Dict() = default;
Dict::Dict(const Dict& d) {
    for (const auto& [k, v] : d.m_storage)
        set(k, std::make_unique<Stack>(*v));
}
Dict::Dict(Dict&& d) = default;
Dict& Dict::operator=(const Dict& d) {
    Dict cpy = d;
    std::swap(*this, cpy);
    return *this;
}
Dict& Dict::operator=(Dict&& d) = default;
Dict::~Dict() = default;

bool Dict::contains(key_type key) const { return m_storage.contains(key); }

Stack* Dict::at(key_type key) const {
    if (!contains(key))
        throw DictException(std::format("Key '{}' not in dictionary", key));

    auto it = m_storage.find(key);
    return it->second.get();
}
std::unique_ptr<Stack> Dict::pop(key_type key) {
    if (!contains(key))
        throw DictException(std::format("Key '{}' not in dictionary", key));

    auto it = m_storage.find(key);
    auto rv = std::move(it->second);
    m_storage.erase(it);
    return rv;
}
void Dict::set(key_type key, std::unique_ptr<Stack> stack) { m_storage.insert_or_assign(key, std::move(stack)); }

std::size_t Dict::size() const noexcept { return m_storage.size(); }
void Dict::clear() { m_storage.clear(); }
std::string Dict::toString(int verbose_level) const {
    if (verbose_level <= 0)
        return "dict";
    std::string out = "{\n";
    for (const auto& [k, v] : m_storage)
        out += "  " + std::to_string(k) + " => " + v->toString(verbose_level <= 1 ? 0 : verbose_level - 1) + "\n";
    out += "}";
    return out;
}
} // namespace builtin_structs