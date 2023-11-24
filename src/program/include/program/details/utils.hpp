#pragma once

#include <memory>

namespace cthu::program {
class Stack;
class Dict;
class SafeStack;
class SafeDict;
} // namespace cthu::program

namespace cthu::program::details {

template <typename item_t>
constexpr auto toSafe(item_t item) {
    if constexpr (std::is_same_v<item_t, Stack*>)
        return reinterpret_cast<SafeStack*>(item);
    else if constexpr (std::is_same_v<item_t, std::unique_ptr<Stack>>)
        return std::unique_ptr<SafeStack>(reinterpret_cast<SafeStack*>(item.release()));
    else if constexpr (std::is_same_v<item_t, Dict*>)
        return reinterpret_cast<SafeDict*>(item);
    else if constexpr (std::is_same_v<item_t, std::unique_ptr<Dict>>)
        return std::unique_ptr<SafeDict>(reinterpret_cast<SafeDict*>(item.release()));
    else
        return item;
}
template <typename item_t>
constexpr auto fromSafe(item_t item) {
    if constexpr (std::is_same_v<item_t, SafeStack*>)
        return reinterpret_cast<Stack*>(item);
    else if constexpr (std::is_same_v<item_t, std::unique_ptr<SafeStack>>)
        return std::unique_ptr<Stack>(reinterpret_cast<Stack*>(item.release()));
    else if constexpr (std::is_same_v<item_t, SafeDict*>)
        return reinterpret_cast<Dict*>(item);
    else if constexpr (std::is_same_v<item_t, std::unique_ptr<SafeDict>>)
        return std::unique_ptr<Dict>(reinterpret_cast<Dict*>(item.release()));
    else
        return item;
}

} // namespace cthu::program::details
