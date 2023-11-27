#include "program/dict.hpp"

namespace cthu::program {
std::string Dict::toShortString(bool is_on_top /* = true */) const {
    if (!is_on_top)
        return "dict";
    std::string out = "{\n";
    for (std::size_t i = 0; i < m_storage.size(); ++i) {
        if (m_storage[i] == nullptr)
            continue;
        out += std::format("  {} => {}\n", i, m_storage[i]->toShortString(true));
    }
    out += "}";
    return out;
};

std::string Dict::toJson(std::size_t indent /* = 0 */) const {
    std::string ws = std::string(indent, ' ');
    std::string out = ws + "{\"type\": \"dict\", \"data\": {\n";
    out += ws + "  \"items\": {\n";

    std::string delim = "";
    for (std::size_t i = 0; i < m_storage.size(); ++i) {
        if (m_storage[i] == nullptr)
            continue;
        out += std::format("{}{}  \"{}\": {}", delim, ws, i, m_storage[i]->toJson(indent + 4));
        delim = ",\n";
    }
    out += "\n" + ws + "  }\n";
    out += ws + "}";
    return out;
}
} // namespace cthu::program
