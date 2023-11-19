#include "parser.h"

#include "program/instruction.h"
#include "program/number.h"

#include <format>
#include <map>
#include <string>
#include <string_view>
#include <vector>

namespace {
void trim_view(std::string_view& view) {
    while (!view.empty() && std::isspace(view.front()))
        view.remove_prefix(1);
    while (!view.empty() && std::isspace(view.back()))
        view.remove_suffix(1);
}
} // namespace

namespace cthu::parser::impl {
void Parser::parse(std::istream& source_code, program::Program* program) /* override */ {
    program->required_builtins = utils::ISet<std::string>::createStdSet();

    std::size_t line_number = 0;
    lines_t* current_lines = nullptr;

    std::string line;
    while (std::getline(source_code, line)) {
        ++line_number;
        std::string_view view(line);

        // trim
        trim_view(view);

        // check if empty or comment
        if (view.empty() || view.starts_with("#"))
            continue;

        // builtin definition
        if (view.starts_with(".struct ")) {
            view.remove_prefix(8); // remove ".struct "
            view.remove_suffix(8); // remove " builtin"
            trim_view(view);
            program->required_builtins->insert(std::string(view));
            continue;
        }

        // stack start
        if (view.ends_with(":") && !view.starts_with("%")) {
            view.remove_suffix(1);
            current_lines = &m_stack_lines[std::string(view)];
            continue;
        }

        // dict start
        if (view.ends_with(":") && view.starts_with("%")) {
            view.remove_prefix(1);
            view.remove_suffix(1);
            current_lines = &m_dict_lines[std::string(view)];
            continue;
        }

        // important line
        if (current_lines == nullptr)
            throw std::runtime_error(std::format("Line {} does not belong to the stack/dict\n", line_number));

        current_lines->emplace_back(line_number, std::string(view));
    }

    program->init_dictionary = parseDict(m_dict_lines.at("init"));
}

std::unique_ptr<program::IStack> Parser::parseStack(const lines_t& lines) {
    auto stack = program::IStack::createStackVector();

    for (auto it = lines.crbegin(); it != lines.crend(); ++it) {
        const auto& [num, line] = *it;
        std::string_view view(line);

        // number
        if (view.starts_with("$")) {
            view.remove_prefix(1);
            uint32_t value = uint32_t(std::stoll(std::string(view)));
            stack->push(program::Number::createUnique(value));
            continue;
        }

        // stack
        if (view.starts_with("@")) {
            view.remove_prefix(1);
            stack->push(parseStack(m_stack_lines[std::string(view)]));
            continue;
        }

        // dictionary
        if (view.starts_with("%")) {
            view.remove_prefix(1);
            stack->push(parseStack(m_dict_lines[std::string(view)]));
            continue;
        }

        auto extract_word = [&]() -> std::string {
            std::size_t idx = view.find_first_of(" ");
            std::string out = std::string(view.substr(0, idx));
            view.remove_prefix(out.size());
            trim_view(view);
            return out;
        };

        std::string builtin = extract_word();
        std::string operation = extract_word();

        std::vector<std::string> operands;
        while (!view.empty())
            operands.push_back(extract_word());
        stack->push(
            program::Instruction::createUnique(std::move(builtin), std::move(operation), std::move(operands), num));
    }

    return stack;
}
std::unique_ptr<program::IDict> Parser::parseDict(const lines_t& lines) {
    auto dict = program::IDict::createDict();

    for (const auto& [_, line] : lines) {
        std::size_t pos = 0;
        auto key = std::stoll(line, &pos);

        std::string_view view(line);
        view.remove_prefix(pos);
        trim_view(view);
        view.remove_prefix(2); // remove "=>"
        trim_view(view);
        view.remove_prefix(1); // remove "@"
        dict->set(std::size_t(key), parseStack(m_stack_lines.at(std::string(view))));
    }

    return dict;
}

} // namespace cthu::parser::impl

namespace cthu::parser {
/* static */ std::unique_ptr<IParser> IParser::createParser() { return std::make_unique<impl::Parser>(); }
} // namespace cthu::parser
