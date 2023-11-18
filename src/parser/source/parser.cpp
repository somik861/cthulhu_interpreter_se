#include "parser.h"

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
void Parser::parse(std::istream& source_code, program::IProgram* program) /* override */ {
    auto required_builtins = utils::ISet<std::string>::createStdSet();

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
            required_builtins->insert(std::string(view));
            continue;
        }

        // stack start
        if (view.ends_with(":") && !view.starts_with("%")) {
            view.remove_suffix(1);
            current_lines = &m_stack_lines[std::string(view)];
        }

        // dict start
        if (view.ends_with(":") && view.starts_with("%")) {
            view.remove_prefix(1);
            view.remove_suffix(1);
            current_lines = &m_dict_lines[std::string(view)];
        }

        // important line
        if (current_lines == nullptr)
            throw std::runtime_error(std::format("Line {} does not belong to the stack/dict\n", line_number));

        current_lines->emplace_back(line_number, std::string(view));
    }

    program->setRequriedBuiltins(std::move(required_builtins));
    program->setInitDictionary(parseDict(m_dict_lines.at("init")));
}

std::unique_ptr<program::IStack> Parser::parseStack(const lines_t& lines) {
    auto stack = program::IStack::createStackVector();

    return stack;
}
std::unique_ptr<program::IDict> Parser::parseDict(const lines_t& lines) {
    auto dict = program::IDict::createDict();

    for (const auto& [_, line] : lines) {
        std::size_t pos;
        auto key = std::stoll(line, &pos);

        std::string_view view(line);
        view.remove_prefix(pos);
        trim_view(view);
        view.remove_prefix(2); // remove "=>"
        trim_view(view);
        dict->set(std::size_t(key), parseStack(m_stack_lines.at(std::string(view))));
    }

    return dict;
}

} // namespace cthu::parser::impl

namespace cthu::parser {
/* static */ std::unique_ptr<IParser> IParser::createParser() { return std::make_unique<impl::Parser>(); }
} // namespace cthu::parser
