#include "builtins/ibuiltin.h"

#include <stdexcept>

namespace cthu::builtins::impl {
class Bv32 : public virtual IBuiltin {
  public:
    void executeOperation(const std::string& operation,
                          const std::vector<std::string>& operands,
                          interpreter::ProgramState* state) override;
};
} // namespace cthu::builtins::impl
