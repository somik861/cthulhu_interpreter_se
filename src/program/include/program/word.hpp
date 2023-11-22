#pragma once

namespace cthu::program {

class Word {
  public:
    uint32_t value;
    std::string toShortString(bool is_on_top = true) const { return ""; }
    std::string toJson(std::size_t indent = 0) const { return ""; }
};

static_assert(sizeof(Word) == 4, "Word class is broken");
} // namespace cthu::program
