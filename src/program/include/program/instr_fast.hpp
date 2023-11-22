#pragma once

namespace cthu::program {
// The bottom-most 3 bits are ignored, so the Stack can do its fast optimisations;
class InstrFast {
  public:
    uint64_t value = 0;

    std::string toShortString(bool is_on_top = true) const { return ""; };
    std::string toJson(std::size_t indent = 0) const { return ""; }
};

// make sure that sizeof(InstrFast) is 8
static_assert(
    sizeof(InstrFast) == 8,
    "sizeof(InstrFast) is not 8, there is either something very wrong with the code, or your machine/compiler");
} // namespace cthu::program
