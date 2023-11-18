#pragma once

#include "imap.h"

#include "std_iterator_wrap.h"

#include <map>
#include <memory>

namespace utils::impl {
template <typename key_t, typename value_t>
class StdMap : public virtual IMap<key_t, value_t> {
  public:
    using key_type = key_t;
    using value_type = value_t;

    value_type& at(const key_type& key) override { return m_map.at(key); }
    const value_type& at(const key_type& key) const override { return m_map.at(key); }
    bool insert(const key_type& key, value_type value, bool replace = false) override {
        bool existed = m_map.contains(key);

        if (!existed || replace)
            m_map[key] = std::move(value);

        return existed;
    }
    bool contains(const key_type& key) const override { return m_map.contains(key); }
    bool erase(const key_type& key) override { return bool(m_map.erase(key)); }
    void clear() override { m_map.clear(); }
    std::size_t size() const override { return m_map.size(); }
    bool empty() const override { return m_map.empty(); }
    std::unique_ptr<IBidirectionalIterator<std::pair<const key_type&, value_type&>>> iterator() override {
        return std::make_unique<StdIteratorWrapper>(m_map.begin(), m_map.end());
    }
    std::unique_ptr<IBidirectionalIterator<std::pair<const key_type&, const value_type&>>> iterator() const override {
        return std::make_unique<StdIteratorWrapper>(m_map.begin(), m_map.end());
    }
    std::unique_ptr<IMap> clone() const override {
        auto cpy = std::make_unique<StdMap>();
        cpy.m_map = m_map;
        return cpy;
    }
    std::unique_ptr<IMap> getEmpty() const override { return std::make_unique<StdMap>(); }

  private:
    std::map<key_type, value_type> m_map;
};
} // namespace utils::impl

namespace utils {
/* static */ std::unique_ptr<IMap> IMap::createStdMap() { return std::make_unique<impl::StdMap>(); }
} // namespace utils
