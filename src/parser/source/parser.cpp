#include "parser/parser.hpp"

#include <stdexcept>
#include <string_view>

namespace {
void trim_view(std::string_view& view) {
    while (!view.empty() && std::isspace(view.front()))
        view.remove_prefix(1);
    while (!view.empty() && std::isspace(view.back()))
        view.remove_suffix(1);
}
} // namespace

namespace cthu::parser {
std::pair<program::SafeDict, std::set<std::string>> Parser::parse(std::istream& stream) {
    m_stack_lines.clear();
    m_dict_lines.clear();

    std::set<std::string> required_domains;

    std::size_t line_number = 0;
    lines_t* current_lines = nullptr;

    std::string line;
    while (std::getline(stream, line)) {
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
            required_domains.insert(std::string(view));
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

    if (!m_dict_lines.contains("init"))
        throw std::runtime_error("Source code does not contain required dictionary: 'init'");

    return {*parseDict(m_dict_lines.at("init")), required_domains};
}

std::unique_ptr<program::SafeStack> Parser::parseStack(const lines_t& lines) {
    auto stack = std::make_unique<program::SafeStack>();

    for (auto it = lines.crbegin(); it != lines.crend(); ++it) {
        const auto& [num, line] = *it;
        std::string_view view(line);

        // number
        if (view.starts_with("$")) {
            view.remove_prefix(1);
            uint32_t value = uint32_t(std::stoul(std::string(view)));
            stack->push(program::Word(value));
            continue;
        }

        // stack
        if (view.starts_with("@")) {
            view.remove_prefix(1);
            stack->push(parseStack(m_stack_lines.at(std::string(view))));
            continue;
        }

        // dictionary
        if (view.starts_with("%")) {
            view.remove_prefix(1);
            stack->push(parseStack(m_dict_lines.at(std::string(view))));
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
            std::make_unique<program::Instruction>(std::move(builtin), std::move(operation), std::move(operands), num));
    }

    return stack;
}
std::unique_ptr<program::SafeDict> Parser::parseDict(const lines_t& lines) {
    auto dict = std::make_unique<program::SafeDict>();

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

} // namespace cthu::parser
