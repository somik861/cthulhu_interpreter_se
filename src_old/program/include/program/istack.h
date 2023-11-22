#pragma once

#include "istack_item.h"

#include <memory>

namespace cthu::program {
class IStack : public virtual IStackItem {
  public:
    virtual ~IStack() = default;

    /**
     * @brief Push item on the top of the stack
     *
     * @param[in] item          item to push on top
     */
    virtual void push(std::unique_ptr<IStackItem> item) = 0;

    /**
     * @brief Get item on top of the stack
     *
     * @return           item on top of the stack
     */
    virtual IStackItem* top() = 0;

    /**
     * @brief Remove item on top of the stack
     *
     * Calling pop on empty stack is undefined behaviour.
     */
    virtual void pop() = 0;

    /**
     * @brief Combines pop() and top() for convenience
     *
     * @return       item that was popped
     */
    virtual std::unique_ptr<IStackItem> poptop() = 0;

    /**
     * @brief Get stack size
     *
     * @return       stack size
     */
    virtual std::size_t size() const = 0;

    /**
     * @brief Check if stack is empty
     *
     * @return        true if stack is empty, else false
     */
    virtual bool empty() const = 0;

    /**
     * @brief Remove all items from the stack
     */
    virtual void clear() = 0;

    /**
     * @brief get copy of the stack
     *
     * @return          copied stack
     */
    virtual std::unique_ptr<IStack> clone() const = 0;

    /**
     * @brief get empty stack of the same type
     *
     * @return          empty stack
     */
    virtual std::unique_ptr<IStack> getEmpty() const = 0;

    static std::unique_ptr<IStack> createStackVector(std::string name = "");
};
} // namespace cthu::program
