#pragma once

#include "utils/iiterators.h"

namespace utils::impl {
template <typename it_t>
class StdIteratorWrapper : public virtual IBidirectionalIterator<typename it_t::value_type> {
  public:
    StdIteratorWrapper(it_t begin_it, it_t end_it) : m_begin_it(begin_it), m_end_it(end_it), m_curr_it(begin_it) {}
    using value_type = typename it_t::value_type;

    bool hasNext() const override { return m_curr_it != m_end_it; }
    value_type& next() override {
        ++m_curr_it;
        return *m_curr_it;
    }
    value_type& value() override { return *m_curr_it; }
    value_type& prev() override {
        --m_curr_it;
        return *m_curr_it;
    }
    bool hasPrev() const override { return m_curr_it != m_begin_it; }

  private:
    it_t m_begin_it, m_end_it, m_curr_it;
};

template <typename it_t>
class ConstStdIteratorWrapper : public virtual IBidirectionalIterator<const typename it_t::value_type> {
  public:
    ConstStdIteratorWrapper(it_t begin_it, it_t end_it) : m_begin_it(begin_it), m_end_it(end_it), m_curr_it(begin_it) {}
    using value_type = typename it_t::value_type;

    bool hasNext() const override { return m_curr_it != m_end_it; }
    const value_type& next() override {
        ++m_curr_it;
        return *m_curr_it;
    }
    const value_type& value() override { return *m_curr_it; }
    const value_type& prev() override {
        --m_curr_it;
        return *m_curr_it;
    }
    bool hasPrev() const override { return m_curr_it != m_begin_it; }

  private:
    it_t m_begin_it, m_end_it, m_curr_it;
};

} // namespace utils::impl
