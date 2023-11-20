#include "stack_vector.h"

namespace cthu::program::impl {
StackVector::StackVector(std::string name /* = "" */)
    : m_name(std::move(name)) {}
void StackVector::push(std::unique_ptr<IStackItem> item) /* override */ {
    m_name = "", m_vector.push_back(std::move(item));
}
IStackItem* StackVector::top() /* override */ { return m_vector.back().get(); }
void StackVector::pop() /* override */ {
    m_name = "";
    m_vector.pop_back();
}
std::unique_ptr<IStackItem> StackVector::poptop() /* override */ {
    auto rv = std::move(m_vector.back());
    pop();
    return rv;
}
std::size_t StackVector::size() const /* override */ { return m_vector.size(); }
bool StackVector::empty() const /* override */ { return m_vector.empty(); }
void StackVector::clear() /* override */ {
    m_name = "";
    return m_vector.clear();
}
std::unique_ptr<IStack> StackVector::clone() const /* override */ { return cloneRaw(); }
std::unique_ptr<IStackItem> StackVector::cloneItem() const /* override */ { return cloneRaw(); }
std::unique_ptr<IStack> StackVector::getEmpty() const /* override */ { return std::make_unique<StackVector>(); }

std::unique_ptr<StackVector> StackVector::cloneRaw() const {
    auto cpy = std::make_unique<StackVector>();
    cpy->m_vector.reserve(m_vector.size());
    for (const auto& item : m_vector)
        cpy->m_vector.push_back(item->cloneItem());

    cpy->m_name = m_name;
    return cpy;
}

std::string StackVector::toString(std::size_t indent /* = 0 */) const /* override */ {
    std::string out(indent, ' ');
    out += "------- TOP ---";
    if (!m_name.empty())
        out += " " + m_name;
    else
        out += "----\n";

    for (auto it = m_vector.crbegin(); it != m_vector.crend(); ++it) {
        out += (*it)->toString(indent + 2);
        out += '\n';
    }
    out += std::string(indent, ' ');
    out += "----- BOTTOM ---";
    if (!m_name.empty())
        out += " " + m_name;
    else
        out += "---";
    return out;
}

std::string StackVector::toShortString(bool is_on_top /* = true */) const /* override */ {
    if (!is_on_top)
        return std::string("stck") + (m_name.empty() ? "" : ":") + m_name;
    std::string out = m_name + "| ";

    for (auto& item : m_vector) {
        out += item->toShortString(false) + " ";
    }

    out += ">t";
    return out;
}
} // namespace cthu::program::impl

namespace cthu::program {
/* static */ std::unique_ptr<IStack> IStack::createStackVector(std::string name /* = "" */) {
    return std::make_unique<impl::StackVector>(std::move(name));
}
} // namespace cthu::program
