#pragma once

#include "program/istack.h"
#include "utils/iset.h"

#include <memory>
#include <string>
#include <string_view>

namespace cthu::interpreter {
class IProgramState {
  public:
    /**
     * @brief Get line number corresponding to the last processed line
     *
     * @returns line number
     */
    virtual int getLastLineNumber() = 0;

    /**
     * @brief Get number of line that will be executed next.
     *
     * @returns line number
     */
    virtual int getNextLineNumber() = 0;

    virtual std::unique_ptr<utils::ISet<std::string>> getKnownStacks() const = 0;

    /**
     * @brief Get stack with corresponding name
     *
     * NOTE: This is not a copy of the stack, all changes will also affect the
     *program. To obtain a deep-copy call const-version of this method.
     *
     * @brief stack corresponding to given name, or nullptr no such stack
     *exists.
     */
    virtual std::shared_ptr<program::IStack> getStack(std::string_view name) = 0;

    /**
     * @brief Get stack with corresponding name
     *
     * NOTE: This is not a copy of the stack, all changes will also affect the
     *program. To obtain a deep-copy call const-version of this method.
     *
     * @brief stack corresponding to given name, or nullptr no such stack
     *exists.
     */
    virtual std::shared_ptr<program::IStack> getStack(std::string_view name) const = 0;

    /**
     * @brief Check whether current program state corresponds to finished execution
     *
     * This does not mean that the program finished properly in this state.
     * If you want to see, whether program ended as it should (i.e. not in guard, etc.),
     * call isFinishedSuccessfully().
     *
     * @return           false if program can continue from this state, else true
     */
    virtual bool isFinished() const = 0;

    /**
     * @brief Check whether current program state corresponds to properly finished execution
     *
     * Most of the times, if this method returns true, the state holds result that you want.
     *
     * @return            false if program can continue from this state or ended with error, else true
     */
    virtual bool isFinishedSuccessfully() const = 0;

    virtual ~IProgramState() = default;
};
} // namespace cthu::interpreter
