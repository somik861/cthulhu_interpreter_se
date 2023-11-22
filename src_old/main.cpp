#include "builtins/ibuiltin.h"
#include "interpreter/iinterpreter.h"
#include "parser/iparser.h"
#include "utils/iset.h"

#include <chrono>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <memory>
#include <string>

namespace fs = std::filesystem;
namespace chrn = std::chrono;

int main(int argc, char** argv) {
    bool STEP_DEBUG = false;
    if (argc != 2) {
        std::cerr << "./cthulhu source_code\n";
        return 1;
    }

    fs::path source_path(argv[1]);

    if (!fs::exists(source_path)) {
        std::cerr << "Source path does not exist\n";
        return 1;
    }

    std::ifstream source_code(source_path);

    // begin measurement
    auto start_timepoint = chrn::system_clock::now();

    auto parser = cthu::parser::IParser::createParser();
    auto interpreter = cthu::interpreter::IInterpreter::createInterpreter();
    auto program = std::make_unique<cthu::program::Program>();
    parser->parse(source_code, program.get());

    interpreter->addBuiltin("bv32", cthu::builtins::IBuiltin::createBuiltin_bv32());
    interpreter->addBuiltin("stck", cthu::builtins::IBuiltin::createBuiltin_stck());
    interpreter->initExecution(std::move(program));

    std::cout << "INIT\n";
    std::cout << interpreter->getProgramState()->state_dict->toShortString() << '\n';
    std::cout << "....\n";

    while (interpreter->canContinue()) {
        if (STEP_DEBUG) {
            std::cout << "BEFORE:\n";
            auto state = interpreter->getProgramState();
            std::cout << (state ? state->toString() : "nullptr") << '\n';
        }
        interpreter->continueExecution();

        if (STEP_DEBUG) {
            std::cout << "AFTER:\n";
            auto state = interpreter->getProgramState();
            std::cout << (state ? state->toString() : "nullptr") << '\n';
            std::cout << "Press enter to continue ...\n";
            std::getchar();
        }
    }
    std::size_t final_idx = 0;
    for (auto& final_state : interpreter->getFinishedStates()) {
        std::cout << "FINAL " << final_idx << ":\n";
        std::cout << final_state->state_dict->toShortString() << '\n';
        std::cout << "....\n";
        ++final_idx;
    }

    // end measurement
    auto delta = chrn::system_clock::now() - start_timepoint;
    auto time_wip = chrn::duration_cast<chrn::milliseconds>(delta).count();

    std::size_t mils = time_wip % 1000;
    time_wip /= 1000;

    std::size_t secs = time_wip % 60;
    time_wip /= 60;

    std::cout << std::format("Took: {}m {}s {}ms\n", time_wip, secs, mils);
}
