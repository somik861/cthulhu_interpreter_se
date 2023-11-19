#pragma once

#include <format>
#include <stdexcept>

#define CHECK_OPERANDS_COUNT(structure, n)                                                                             \
    if (operands.size() != n)                                                                                          \
        throw std::invalid_argument(std::format("Unsupported operand count of {} operation {}, got {}, expected {}",   \
                                                structure, operation, operands.size(), n));
