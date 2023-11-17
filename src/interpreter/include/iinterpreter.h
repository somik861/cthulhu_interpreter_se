#pragma once

#include "ibuiltin.h"
#include "iprogram_code.h"
#include "iprogram_state.h"

#include <memory>
#include <string_view>

namespace cthu {

class IInterpreter {
  public:
	/**
	 * @brief Add builtin structure to be recognized
	 *
	 * @param[in] name		name of the builtin
	 * @param[in] builtin	builtin object
	 */
	virtual void addBuiltin(std::string_view name,
	                        std::unique_ptr<builtins::IBuiltin> builtin) = 0;

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
	virtual void initExecution(std::unique_ptr<parser::IProgramCode> code,
	                           std::ostream* log_stream = nullptr) = 0;

	/**
	 * @brief Return information whether the interpreter is ready to continue
	 *
	 * @returns	true if interpreter can continue, false if not initialized/at
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
	 * @brief Get state of the program
	 *
	 * Use this to obtain information about the current state.
	 * This can mean either at the end of the execution or when program is
	 * execution is stopped. You can also call this after initExecution(...) to
	 * check initial state.
	 *
	 * NOTE: This is not a copy, all changes will also affect the program.
	 *	To obtain a copy, use const-version of this method.
	 *
	 * @returns program state or nullptr (if no valid state that
	 * can be returned exists)
	 */
	virtual std::shared_ptr<IProgramState> getProgramState() = 0;

	/**
	 * @brief Get state of the program
	 *
	 * Use this to obtain information about the current state.
	 * This can mean either at the end of the execution or when program is
	 * execution is stopped. You can also call this after initExecution(...) to
	 * check initial state.
	 *
	 * @returns deep-copy of program state or nullptr (if no valid state that
	 * can be returned exists)
	 */
	virtual std::shared_ptr<IProgramState> getProgramState() const = 0;
};
} // namespace cthu
