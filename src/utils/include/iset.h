#pragma once

#include "iiterators.h"

#include <memory>

namespace utils {
template <typename T>
class ISet {
  public:
    using value_type = T;

    /**
     * @brief Insert value to set
     *
     * @param[in] value      value to insert
     * @return               true if new value was inserted, false otherwise (value already existed)
     */
    virtual bool insert(value_type value) = 0;

    /**
     * @brief Check presence of value
     *
     * @return               true if value is in the set, else false
     */
    virtual bool constains(const value_type& value) = 0;
    /**
     * @brief Erase value from the set
     *
     * @param[in] value      value to be erased
     * @return               true if value was erased, false if no such value existed
     */
    virtual bool erase(const value_type& value) = 0;

    /**
     * @brief Remove all values from the set
     */
    virtual void clear() = 0;

    /**
     * @brief Get number of elements in the set
     *
     * @return               number of elements in the set
     */
    virtual std::size_t size() const = 0;

    /**
     * @brief Check if set is empty (size == 0)
     *
     * @return       true if set is empty, else false
     */
    virtual bool empty() const = 0;

    /**
     * @brief Get iterator over values
     *
     * @return       iterator
     */
    virtual std::unique_ptr<IBidirectionalIterator<const value_type>> iterator() const = 0;

    /**
     * @brief Get copy of current set
     *
     * std::domain_error is raised if value_type is not copy-able.
     *
     * @return       copoy of set
     */
    virtual std::unique_ptr<ISet> clone() const = 0;

    /**
     * @brief Get empty set
     *
     * @return       empty set
     */
    virtual std::unique_ptr<ISet> getEmpty() const = 0;

    virtual ~ISet() = default;
};
} // namespace utils
