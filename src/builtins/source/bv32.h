#include "builtins/ibuiltin.h"

#include <stdexcept>

namespace cthu::builtins::impl {
class Bv32 : public virtual IBuiltin {
  public:
    void executeOperation(const std::string& operation,
                          const std::vector<std::string>& operands,
                          interpreter::IProgramState* state) override {
        throw std::invalid_argument("Operation not implemented");
    }
};
} // namespace cthu::builtins::impl
