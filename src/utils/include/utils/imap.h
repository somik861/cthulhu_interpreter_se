#pragma once

#include "utils/iiterators.h"

namespace utils {
template <typename key_t, typename value_t>
class IMap {
  public:
    using key_type = key_t;
    using value_type = value_t;

    /**
     * @brief Get reference on object located at key
     *
     * It is UB when key does not exist.
     *
     * @param[in] key        key of the object
     * @return              reference to an object located at the key
     */
    virtual value_type& at(const key_type& key) = 0;
    /**
     * @brief Get reference on object located at key
     *
     * It is UB when key does not exist.
     *
     * @param[in] key        key of the object
     * @return              reference to an object located at the key
     */
    virtual const value_type& at(const key_type& key) const = 0;

    /**
     * @brief Get object located at given key and erase it from the map
     *
     * It is UB when key does not exist.
     *
     * @param[in] key        key of the object
     * @return              reference to an object located at the key
     */
    virtual value_type pop(const key_type& key) = 0;

    /**
     * @brief Insert key-value pair to the map
     *
     * @param[in] key        key part of the pair
     * @param[in] value      value part of the pair
     * @param[in] replace    whether to replace the value, if key is already in the map
     * @return              true if new key was created, else false
     */
    virtual bool insert(const key_type& key, value_type value, bool replace = false) = 0;

    /**
     * @brief Check for existence of key
     *
     * @param[in] key        key to search for
     * @return               true if such key is in the map, else false
     */
    virtual bool contains(const key_type& key) const = 0;

    /**
     * @brief Erase element with key
     *
     * @param[in] key        key to be erased
     * @return               true if such key existed (and was erased), else false
     */
    virtual bool erase(const key_type& key) = 0;

    /**
     * @brief Erase all elements in map
     */
    virtual void clear() = 0;

    /**
     * @brief Get number of keys located in the map
     *
     * @return               map size
     */
    virtual std::size_t size() const = 0;

    /**
     * @brief Check whether the map is empty
     *
     * @return               true if map is empty (size() == 0), false otherwise
     */
    virtual bool empty() const = 0;

    /**
     * @brief Get an iterator
     *
     * @return      iterator
     */
    virtual std::unique_ptr<IBidirectionalIterator<std::pair<const key_type, value_type>>> iterator() = 0;

    /**
     * @brief Get an iterator
     *
     * @return      iterator
     */
    virtual std::unique_ptr<IBidirectionalIterator<const std::pair<const key_type, value_type>>> iterator() const = 0;

    /**
     * @brief Get a copy of the map
     *
     * If an value_type is unique_ptr, method clone() is called.
     *
     * This function throws std::domain_error if value_type is not copy-able.
     *
     * @return       map copy
     */
    virtual std::unique_ptr<IMap> clone() const = 0;
    /**
     * @brief Get an empty map
     *
     * @return       empty map
     */
    virtual std::unique_ptr<IMap> getEmpty() const = 0;
    virtual ~IMap() = default;

    /* Map factories below */
    static std::unique_ptr<IMap> createStdMap();
};
} // namespace utils

// import templated definitions
#include "../source/std_map.h"
