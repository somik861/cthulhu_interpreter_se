#include "domains/builtins/bv32.hpp"
#include "domains/dict.hpp"
#include "domains/instr.hpp"
#include "domains/stck.hpp"
#include "domains/word.hpp"

#include "interpreter/interpreter.hpp"
#include "parser/parser.hpp"
#include "program/dict.hpp"
#include "program/safe_stack.hpp"
#include "program/stack.hpp"
#include "program/word.hpp"

#include <filesystem>
#include <format>
#include <fstream>
#include <iostream>
#include <optional>
#include <string>
#include <string_view>
#include <thread>

namespace fs = std::filesystem;
namespace doms = cthu::domains;

namespace {
struct CmdLine {
    fs::path source_code;
    cthu::interpreter::Mode mode = cthu::interpreter::Mode::Normal;
    std::size_t max_threads = std::thread::hardware_concurrency();
    std::optional<std::size_t> read_result = {};
    bool json = false;
};

std::string_view mode_to_sv(cthu::interpreter::Mode mode) {
    switch (mode) {
    case cthu::interpreter::Mode::Debug:
        return "debug";
    case cthu::interpreter::Mode::Normal:
        return "normal";
    case cthu::interpreter::Mode::Fast:
        return "fast";
    case cthu::interpreter::Mode::Parallel:
        return "parallel";
    }
    return "";
}

std::optional<CmdLine> parse_cmdline(int argc, char** argv) {
    CmdLine out;

    if (argc < 2) {
        std::cout << std::format("./program source_code [--mode={}] [--max_threads={}] [--read_result=N] [--json]",
                                 mode_to_sv(out.mode),
                                 out.max_threads);
        return {};
    }

    out.source_code = argv[1];

    auto parse_option = [&out](std::string_view sv) -> bool {
        if (sv == "json") {
            out.json = true;
            return true;
        }

        if (sv.find('=') == std::string_view::npos)
            return false;
        std::size_t idx = sv.find('=');

        std::string_view option = sv.substr(0, idx);
        std::string_view value = sv.substr(idx + 1);

        if (option == "mode") {
            if (value == "debug")
                out.mode = cthu::interpreter::Mode::Debug;
            else if (value == "normal")
                out.mode = cthu::interpreter::Mode::Normal;
            else if (value == "fast")
                out.mode = cthu::interpreter::Mode::Fast;
            else if (value == "parallel")
                out.mode = cthu::interpreter::Mode::Parallel;
            else {
                std::cout << std::format("Unknown interpreter mode '{}'\n", value);
                return false;
            }
            return true;
        }

        if (option == "max_threads") {
            std::size_t processed;
            out.max_threads = std::stoul(std::string(value), &processed);
            bool rv = processed == value.size();
            if (!rv)
                std::cout << std::format("Invalid max_threads value '{}'\n", value);
            return rv;
        }

        if (option == "read_result") {
            std::size_t processed;
            out.read_result = std::stoul(std::string(value), &processed);
            bool rv = processed == value.size() && out.read_result.value() < 6;
            if (!rv)
                std::cout << std::format("Invalid read_result value '{}'\n", value);
            return rv;
        }

        std::cout << std::format("Unknown option '{}'\n", option);
        return false;
    };

    for (int i = 2; i < argc; ++i)
        if (!parse_option(argv[i]))
            return {};

    return out;
}
} // namespace

int main(int argc, char** argv) {
    auto init_domains = [](auto& intr) {
        // it is important to load Instruction first for all interpreters
        intr.registerDomain("instr", std::make_unique<doms::Instr>(0));

        // register other interpreter
        intr.registerDomain("word", std::make_unique<doms::Word>());
        intr.registerDomain("bv32", std::make_unique<doms::builtins::Bv32>());
        intr.registerDomain("dict", std::make_unique<doms::Dict>());
        intr.registerDomain("stck", std::make_unique<doms::Stck>());
    };

    cthu::interpreter::Interpreter<cthu::interpreter::Mode::Debug> idebug;
    cthu::interpreter::Interpreter<cthu::interpreter::Mode::Normal> inormal;
    cthu::interpreter::Interpreter<cthu::interpreter::Mode::Fast> ifast;
    cthu::interpreter::Interpreter<cthu::interpreter::Mode::Parallel> iparallel;

    init_domains(idebug);
    init_domains(inormal);
    init_domains(ifast);
    init_domains(iparallel);

    cthu::parser::Parser parser;
    CmdLine args;
    if (auto cmd_args = parse_cmdline(argc, argv); cmd_args.has_value())
        args = cmd_args.value();
    else
        return 1;

    if (!fs::exists(args.source_code))
        std::cout << "File: '" << args.source_code << "' does not exist\n";

    auto execute_program = [&](auto& interpreter) {
        try {
            std::ifstream source_stream(args.source_code);
            auto [state, domains] = parser.parse(source_stream);

            interpreter.initExecution(state, domains);
            do {
                interpreter.continueExecution();
                if (!interpreter.canContinue())
                    break;
                auto state = interpreter.getProgramState();
                std::cout << "State ID: " << state->thread_id << '\n';
                if (args.json)
                    std::cout << state->toJson() << "\n\n";
                else
                    std::cout << *state << "\n\n";
                std::cout << "Press enter to continue ... \n";
                while (std::getchar() != '\n')
                    ;
            } while (true);

            std::cout << "===== program ended =====\n";
            auto fin_states = interpreter.getFinishedStates();
            for (std::size_t i = 0; i < fin_states.size(); ++i) {
                std::cout << "--------- Fin state: " << i << ":\n";
                if (args.json)
                    std::cout << fin_states[i]->toJson() << '\n';
                else
                    std::cout << (*fin_states[i]) << '\n';
            }

        } catch (const std::exception& e) {
            std::cout << "An exception was thrown:\n" << e.what() << std::endl;
            return 1;
        }
        return 0;
    };

    switch (args.mode) {
    case cthu::interpreter::Mode::Debug:
        return execute_program(idebug);
    case cthu::interpreter::Mode::Normal:
        return execute_program(inormal);
    case cthu::interpreter::Mode::Fast:
        return execute_program(ifast);
    case cthu::interpreter::Mode::Parallel:
        return execute_program(iparallel);
    }
    return 1;
}
