#pragma once

#include "program/idict.h"
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
     * @return      line number
     */
    virtual int getLastLineNumber() const = 0;
    /**
     * @brief set the last line number
     *
     * @param[in]    line number
     */
    virtual void setLastLineNumber(int number) = 0;

    /**
     * @brief Get number of line that will be executed next.
     *
     * @return      line number
     */
    virtual int getNextLineNumber() const = 0;
    /**
     * @brief set the next line number
     *
     * NOTE: setting this is mostly job of the interpreter for your convenience.
     * If you want to change the line to be executed, please make sure that the interpreter
     * also reads and takes it into consideration, otherwise it will have no effect.
     *
     * @param[in]    line number
     */
    virtual void setNextLineNumber(int number) = 0;

    /**
     * @brief Get pointer to the state dict
     *
     * @return      state dictionary
     */
    virtual program::IDict* getStateDict() = 0;

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

    /**
     * @brief Set current program state as finished;
     *
     * @param[in] is_errornous      whether the program ended because of the end, or because of termination (e.g. guard)
     */
    virtual void setFinished(bool is_errornous = false) = 0;

    /**
     * @brief Set error message
     *
     * NOTE: this autmatically sets the program to finished error state (calls setFinished(true))
     *
     * @param[in] message            error message
     */
    virtual void setError(const std::string& message) = 0;

    virtual ~IProgramState() = default;
};
} // namespace cthu::interpreter
