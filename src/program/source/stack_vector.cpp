#include "stack_vector.h"

namespace cthu::program::impl {
void StackVector::push(std::shared_ptr<IStackItem> item) /* override */ { m_vector.push_back(std::move(item)); }
std::shared_ptr<IStackItem> StackVector::top() /* override */ { return m_vector.back(); }
void StackVector::pop() /* override */ { m_vector.pop_back(); }
std::shared_ptr<IStackItem> StackVector::poptop() /* override */ {
    auto rv = top();
    pop();
    return rv;
}
std::size_t StackVector::size() const /* override */ { return m_vector.size(); }
bool StackVector::empty() const /* override */ { return m_vector.empty(); }
void StackVector::clear() /* override */ { return m_vector.clear(); }
std::shared_ptr<IStack> StackVector::clone() const /* override */ {
    auto cpy = std::make_shared<StackVector>();
    cpy->m_vector = m_vector;
    return cpy;
}
std::shared_ptr<IStack> StackVector::getEmpty() const /* override */ { return std::make_shared<StackVector>(); }
} // namespace cthu::program::impl

namespace cthu::program {
/* static */ std::shared_ptr<IStack> createStackVector() { return std::make_shared<impl::StackVector>(); }
} // namespace cthu::program
