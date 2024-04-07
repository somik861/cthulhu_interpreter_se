#pragma once

#include <sstream>
#include <string>
#include <string_view>
#include <vector>

namespace common::string_utils {
inline void trim_front(std::string& str) {
    while (!str.empty() && std::isspace(str.front()))
        str.erase(str.begin());
}
inline void trim_back(std::string& str) {
    while (!str.empty() && std::isspace(str.back()))
        str.erase(str.end());
}

inline void trim(std::string& str) {
    trim_back(str);
    trim_front(str);
}

inline std::vector<std::string> split(std::string_view sv, char delim) {
    std::vector<std::string> out;
    std::string buffer;

    for (char ch : sv) {
        if (ch == delim) {
            if (!buffer.empty())
                out.push_back(std::move(buffer));
            buffer.clear();
            continue;
        }
        buffer.push_back(ch);
    }
    if (!buffer.empty())
        out.push_back(std::move(buffer));

    return out;
}

std::string stringFromStreamable(const auto& el) {
    std::stringstream ss;

    if constexpr (std::is_same_v<decltype(el), const uint8_t&>)
        ss << uint32_t(el);
    else
        ss << el;

    return ss.str();
}
} // namespace common::string_utils