#include "builtins/ibuiltin.h"

#include <stdexcept>

namespace cthu::builtins::impl {
class Bv32 : public virtual IBuiltin {
  public:
    interpreter::ExecutionState executeOperation(const std::string& operation,
                                                 const std::vector<std::string>& operands,
                                                 program::IDict* state_dict,
                                                 std::vector<std::unique_ptr<program::IDict>>& new_threads) override;
};
} // namespace cthu::builtins::impl
