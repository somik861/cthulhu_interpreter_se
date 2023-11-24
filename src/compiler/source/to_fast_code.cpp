#include "compiler/to_fast_code.hpp"

namespace cthu::compiler {
program::Dict to_fast_code(const program::SafeDict& source,
                           const std::map<std::string, std::size_t>& builtin_map,
                           const std::vector<std::unique_ptr<domains::IDomain>> m_domains) {
    return {};
}
} // namespace cthu::compiler
