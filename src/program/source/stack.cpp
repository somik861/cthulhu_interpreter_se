#include "program/stack.hpp"

#include "program/dict.hpp"

namespace cthu::program {
std::string Stack::toShortString(bool is_on_top /* = true */) const {
    if (!is_on_top)
        return "stck";

    std::string out = "| ";
    for (const auto elem : m_storage) {
        out += Apply<>::run(elem, [](auto item) {
            if constexpr (std::is_pointer_v<decltype(item)>)
                return item->toShortString(false);
            else
                return item.toShortString(false);
        });
        out += " ";
    }

    out += ">t";
    return out;
}
std::string Stack::toJson(std::size_t indent /* = 0 */) const {
    std::string ws = std::string(indent, ' ');
    std::string out = ws + "{\"type\": \"stack\", \"data\": {\n";
    out += ws + "  \"items\": [\n";

    std::string delim = "";
    for (const auto& elem : m_storage) {
        out += delim;
        out += Apply<>::run(elem, [=](auto item) {
            if constexpr (std::is_pointer_v<decltype(item)>)
                return item->toJson(indent + 4);
            else
                return item.toJson(indent + 4);
        });

        delim = ",\n";
    }
    out += "\n" + ws + "  ]\n";
    out += ws + "}";
    return out;
}
} // namespace cthu::program
