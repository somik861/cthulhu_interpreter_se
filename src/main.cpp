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
#include <fstream>
#include <iostream>

namespace fs = std::filesystem;
namespace doms = cthu::domains;

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

    cthu::interpreter::Interpreter<cthu::interpreter::Mode::Debug> i1;
    cthu::interpreter::Interpreter<cthu::interpreter::Mode::Normal> i2;
    cthu::interpreter::Interpreter<cthu::interpreter::Mode::Fast> i3;
    cthu::interpreter::Interpreter<cthu::interpreter::Mode::Parallel> i4;

    init_domains(i1);
    init_domains(i2);
    init_domains(i3);
    init_domains(i4);

    cthu::parser::Parser parser;

    if (argc != 2) {
        std::cerr << "./program source_file" << '\n';
        return 1;
    }

    try {
        std::ifstream source_stream(argv[1]);
        auto [state, domains] = parser.parse(source_stream);

        i2.initExecution(state, domains);

        while (i2.canContinue())
            i2.continueExecution();

        std::cout << "===== program ended =====\n";
        auto fin_states = i2.getFinishedStates();
        for (std::size_t i = 0; i < fin_states.size(); ++i) {
            std::cout << "--------- Fin state: " << i << ":\n";
            std::cout << (*fin_states[i]) << '\n';
        }
    } catch (const std::exception& e) {
        std::cout << "An exception was thrown:\n" << e.what() << std::endl;
        return 1;
    }
}
