#pragma once

#include "details.h"
#include "std_iterator_wrapper.h"

#include <map>
#include <memory>
#include <type_traits>

namespace utils::impl {

template <typename key_t, typename value_t>
class StdMap : public virtual IMap<key_t, value_t> {
  public:
    using key_type = key_t;
    using value_type = value_t;
    using iterator_value = std::pair<const key_type, value_type>;

    value_type& at(const key_type& key) override { return m_map.at(key); }
    const value_type& at(const key_type& key) const override { return m_map.at(key); }
    bool insert(const key_type& key, value_type value, bool replace = false) override {
        bool existed = m_map.contains(key);

        if (!existed || replace)
            m_map[key] = std::move(value);

        return !existed;
    }
    bool contains(const key_type& key) const override { return m_map.contains(key); }
    bool erase(const key_type& key) override { return bool(m_map.erase(key)); }
    void clear() override { m_map.clear(); }
    std::size_t size() const override { return m_map.size(); }
    bool empty() const override { return m_map.empty(); }
    std::unique_ptr<IBidirectionalIterator<iterator_value>> iterator() override {
        return std::unique_ptr<IBidirectionalIterator<iterator_value>>(
            new StdIteratorWrapper(m_map.begin(), m_map.end()));
    }
    std::unique_ptr<IBidirectionalIterator<const iterator_value>> iterator() const override {
        return std::unique_ptr<IBidirectionalIterator<const iterator_value>>(
            new ConstStdIteratorWrapper(m_map.begin(), m_map.end()));
    }
    std::unique_ptr<IMap<key_type, value_type>> clone() const override {
        auto cpy = std::make_unique<StdMap>();
        if constexpr (details::is_unique_ptr_v<value_type>) {
            for (const auto& [key, val] : m_map)
                cpy->m_map[key] = val->clone();
        } else {
            cpy->m_map = m_map;
        }
        return cpy;
    }
    std::unique_ptr<IMap<key_type, value_type>> getEmpty() const override { return std::make_unique<StdMap>(); }

  private:
    std::map<key_type, value_type> m_map;
};
} // namespace utils::impl

namespace utils {
template <typename key_t, typename value_t>
/* static */ std::unique_ptr<IMap<key_t, value_t>> IMap<key_t, value_t>::createStdMap() {
    return std::make_unique<impl::StdMap<key_t, value_t>>();
}
} // namespace utils
