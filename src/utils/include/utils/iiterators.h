#pragma once

namespace utils {

template <typename T>
class IIterator {
  public:
    using value_type = T;

    /**
     * @brief Check if iterator can proceed to the next value
     *
     * @return		false if at the end, else true
     */
    virtual bool hasNext() const = 0;

    /**
     * @brief Advance iterator and return value
     *
     * @return		value after advancing the iterator
     */
    virtual value_type& next() = 0;
    /**
     * @brief Get current iterator value
     *
     * @return		current iterator value
     */
    virtual value_type& value() = 0;

    virtual ~IIterator() = default;
};

template <typename T>
using IForwardIterator = IIterator<T>;

template <typename T>
class IBidirectionalIterator : public virtual IForwardIterator<T> {
  public:
    using value_type = T;

    /**
     * @brief Return iterator to the previous value
     *
     * @return		value after moving the iterator
     */
    virtual value_type& prev() = 0;

    /**
     * @brief Check if iterator can be moved to the previous value
     *
     * @return			false if not previous value exists, else true
     */
    virtual bool hasPrev() const = 0;
};

} // namespace utils
