#pragma once

#include "builtins/ibuiltin.h"
#include "interpreter/program_state.h"
#include "program/iprogram.h"
#include "utils/iset.h"

#include <memory>

namespace cthu::interpreter {

class IInterpreter {
  public:
    /**
     * @brief Add builtin structure to be recognized
     *
     * @param[in] name		name of the builtin
     * @param[in] builtin	builtin object
     */
    virtual void addBuiltin(const std::string& name, std::unique_ptr<builtins::IBuiltin> builtin) = 0;

    /**
     * @brief Initiate execution of program
     *
     * No computation will happen until you run continueExecution().
     *
     * You can optionally add stream that will be used to output log data.
     *
     * NOTE: the level and quality of loging fully depends on an interpreter
     *
     * @param[in] code				program code to execute
     * @param[in] log_stream		stream to use for logs, no logs will be
     * outputted if log_stream is nullptr
     */
    virtual void initExecution(std::unique_ptr<program::IProgram> code, std::ostream* log_stream = nullptr) = 0;

    /**
     * @brief Return information whether the interpreter is ready to continue
     *
     * @return	true if interpreter can continue, false if not initialized/at
     * the end/at error.
     */
    virtual bool canContinue() const = 0;

    /**
     * @brief Continue execution
     *
     * This functions blocks until the execution is stopped (for whatever
     * reason).
     *
     * This needs to be called after initExecution() in order to compute
     * something. This is also meant to be used to continue execution if it was
     * stopped by interpreter and is in valid state. Call canContinue() to
     * validate the state.
     *
     * Calling this function when canContinue() returns false does nothing.
     */
    virtual void continueExecution() = 0;

    /**
     * @brief Get state of the program of last run thread.
     *
     * Use this to obtain information about the current state.
     * This can mean either at the end of the execution or when program is
     * execution is stopped. You can also call this after initExecution(...) to
     * check initial state.
     *
     * NOTE: This is not a copy, all changes will also affect the program.
     *	To obtain a copy, use const-version of this method.
     *
     * @return program state or nullptr (if no valid state that
     * can be returned exists)
     */
    virtual ProgramState* getProgramState() = 0;

    /**
     * @brief Get state of the program of thread with given id
     *
     * Use this to obtain information about the current state.
     * This can mean either at the end of the execution or when program is
     * execution is stopped. You can also call this after initExecution(...) to
     * check initial state.
     *
     * NOTE: This is not a copy, all changes will also affect the program.
     *	To obtain a copy, use const-version of this method.
     *
     * @return program state or nullptr (if no valid state that
     * can be returned exists)
     */
    virtual ProgramState* getProgramState(std::size_t id) = 0;

    /**
     * @brief Get set of id's of running threads
     *
     * @return set of id's of running threads
     */
    virtual std::unique_ptr<utils::ISet<std::size_t>> getRunningThreads() const = 0;

    /**
     * @brief Get states that already finished
     *
     * @param[in] include_errors        include states that ended with errors
     * @return                          finished states
     */
    virtual std::vector<ProgramState*> getFinishedStates(bool include_errors = false) = 0;

    /**
     * @brief Get max thread id
     *
     * Thread ID's ranges from 0 to getMaxThreadID() inclusive.
     * Altough, some of them may not be running.
     *
     * @return       max valid thread ID
     */
    virtual std::size_t getMaxThreadID() const = 0;

    virtual ~IInterpreter() = default;

    static std::unique_ptr<IInterpreter> createInterpreterDummy();
    static std::unique_ptr<IInterpreter> createInterpreter();
};
} // namespace cthu::interpreter
