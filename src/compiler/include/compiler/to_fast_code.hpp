#pragma once

#include "domains/idomain.hpp"
#include "program/dict.hpp"
#include "program/safe_dict.hpp"

#include <map>
#include <string>
#include <vector>

namespace cthu::compiler {
program::Dict to_fast_code(const program::SafeDict& source,
                           const std::map<std::string, std::size_t>& domain_map,
                           const std::vector<std::unique_ptr<domains::IDomain>>& m_domains);
}
