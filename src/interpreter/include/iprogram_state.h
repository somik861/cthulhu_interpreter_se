#pragma once

#include "istack.h"

#include <memory>
#include <set>
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

	virtual std::set<std::string> getKnownStacks() const = 0;

	/**
	 * @brief Get stack with corresponding name
	 *
	 * NOTE: This is not a copy of the stack, all changes will also affect the
	 *program. To obtain a deep-copy call const-version of this method.
	 *
	 * @brief stack corresponding to given name, or nullptr no such stack
	 *exists.
	 */
	virtual std::shared_ptr<IStack> getStack(std::string_view name) = 0;

	/**
	 * @brief Get stack with corresponding name
	 *
	 * NOTE: This is not a copy of the stack, all changes will also affect the
	 *program. To obtain a deep-copy call const-version of this method.
	 *
	 * @brief stack corresponding to given name, or nullptr no such stack
	 *exists.
	 */
	virtual std::shared_ptr<IStack> getStack(std::string_view name) const = 0;
};
} // namespace cthu::interpreter
