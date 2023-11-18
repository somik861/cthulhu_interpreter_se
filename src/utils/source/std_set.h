#pragma once

#include "std_iterator_wrapper.h"

#include <set>

namespace utils::impl {
template <typename T>
class StdSet : public virtual ISet<T> {
  public:
    using value_type = T;

    bool insert(value_type value) override {
        auto [_, inserted] = m_set.insert(std::move(value));
        return inserted;
    }
    bool constains(const value_type& value) override { return m_set.contains(value); }
    bool erase(const value_type& value) override { return bool(m_set.erase(value)); }
    void clear() override { m_set.clear(); }
    std::size_t size() const override { return m_set.size(); }
    bool empty() const override { return m_set.empty(); }
    std::unique_ptr<IBidirectionalIterator<const value_type>> iterator() const override {
        return std::unique_ptr<IBidirectionalIterator<const value_type>>(
            new ConstStdIteratorWrapper(m_set.begin(), m_set.end()));
    }
    std::unique_ptr<ISet<value_type>> clone() const override {
        auto cpy = std::make_unique<StdSet>();
        cpy->m_set = m_set;
        return cpy;
    }
    std::unique_ptr<ISet<value_type>> getEmpty() const override { return std::make_unique<StdSet>(); }

  private:
    std::set<value_type> m_set;
};

} // namespace utils::impl

namespace utils {
template <typename T>
/* static */ std::unique_ptr<ISet<T>> ISet<T>::createStdSet() {
    return std::make_unique<impl::StdSet<T>>();
}
} // namespace utils
