#pragma once

#include "builtin_structs/BitVector.hpp"
#include "builtin_structs/Stack.hpp"
#include "common/InterpreterException.hpp"
#include <format>
#include <map>
#include <memory>

namespace builtin_structs {

class DictException : public common::InterpreterException {
  public:
	using common::InterpreterException::InterpreterException;
};

class Dict {
  public:
	using key_type = BitVector32;
	using backend_type = std::map<key_type, std::unique_ptr<Stack>>;

	Dict();
	Dict(const Dict& d);
	Dict(Dict&& d);
	Dict& operator=(const Dict& d);
	Dict& operator=(Dict&& d);
	~Dict();

	bool contains(key_type key) const;
	Stack* at(key_type key) const;
	std::unique_ptr<Stack> pop(key_type key);
	void set(key_type key, std::unique_ptr<Stack> stack);
	std::size_t size() const noexcept;
	void clear();
    std::string toString(int verbose_level) const;

  private:
	backend_type m_storage;
};

} // namespace builtin_structs
