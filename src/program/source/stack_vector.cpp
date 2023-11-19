#include "stack_vector.h"

namespace cthu::program::impl {
void StackVector::push(std::unique_ptr<IStackItem> item) /* override */ { m_vector.push_back(std::move(item)); }
IStackItem* StackVector::top() /* override */ { return m_vector.back().get(); }
void StackVector::pop() /* override */ { m_vector.pop_back(); }
std::unique_ptr<IStackItem> StackVector::poptop() /* override */ {
    auto rv = std::move(m_vector.back());
    pop();
    return rv;
}
std::size_t StackVector::size() const /* override */ { return m_vector.size(); }
bool StackVector::empty() const /* override */ { return m_vector.empty(); }
void StackVector::clear() /* override */ { return m_vector.clear(); }
std::unique_ptr<IStack> StackVector::clone() const /* override */ { return cloneRaw(); }
std::unique_ptr<IStackItem> StackVector::cloneItem() const /* override */ { return cloneRaw(); }
std::unique_ptr<IStack> StackVector::getEmpty() const /* override */ { return std::make_unique<StackVector>(); }

std::unique_ptr<StackVector> StackVector::cloneRaw() const {
    auto cpy = std::make_unique<StackVector>();
    cpy->m_vector.reserve(m_vector.size());
    for (const auto& item : m_vector)
        cpy->m_vector.push_back(item->cloneItem());

    return cpy;
}

std::string StackVector::toString(std::size_t indent /* = 0 */) const /* override */ {
    std::string out(indent, ' ');
    out += "------- TOP -------\n";
    for (auto it = m_vector.crbegin(); it != m_vector.crend(); ++it) {
        out += (*it)->toString(indent + 2);
        out += '\n';
    }
    out += std::string(indent, ' ');
    out += "----- BOTTOM ------";
    return out;
}
} // namespace cthu::program::impl

namespace cthu::program {
/* static */ std::unique_ptr<IStack> IStack::createStackVector() { return std::make_unique<impl::StackVector>(); }
} // namespace cthu::program
