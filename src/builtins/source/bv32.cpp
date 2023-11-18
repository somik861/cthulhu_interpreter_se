#include "bv32.h"

namespace cthu::builtins {
/* static */ std::unique_ptr<IBuiltin> createBuiltin_bv32() { return std::make_unique<impl::Bv32>(); }
} // namespace cthu::builtins
