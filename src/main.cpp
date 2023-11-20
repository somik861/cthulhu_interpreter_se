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
    if (argc != 3) {
        std::cerr << "./cthulhu source_code log_file\n";
        return 1;
    }

    fs::path source_path(argv[1]);
    fs::path log_path(argv[2]);

    if (!fs::exists(source_path)) {
        std::cerr << "Source path does not exist\n";
        return 1;
    }

    std::ifstream source_code(source_path);
    std::ofstream log_stream(log_path);

    // begin measurement
    auto start_timepoint = chrn::system_clock::now();

    auto parser = cthu::parser::IParser::createParser();
    auto interpreter = cthu::interpreter::IInterpreter::createInterpreter();
    auto program = std::make_unique<cthu::program::Program>();
    parser->parse(source_code, program.get());

    interpreter->addBuiltin("bv32", cthu::builtins::IBuiltin::createBuiltin_bv32());
    interpreter->addBuiltin("stck", cthu::builtins::IBuiltin::createBuiltin_stck());
    interpreter->initExecution(std::move(program), &log_stream);

    std::cout << "INIT\n";
    std::cout << interpreter->getProgramState()->state_dict->toString() << '\n';
    std::cout << "....\n";

    while (interpreter->canContinue())
        interpreter->continueExecution();

    auto final_state = interpreter->getFinishedStates()[0];

    std::cout << "FINAL\n";
    std::cout << final_state->state_dict->toString() << '\n';
    std::cout << "....\n";

    // end measurement
    auto delta = chrn::system_clock::now() - start_timepoint;
    auto time_wip = chrn::duration_cast<chrn::milliseconds>(delta).count();

    std::size_t mils = time_wip % 100;
    time_wip /= 100;

    std::size_t secs = time_wip % 60;
    time_wip /= 60;

    std::cout << std::format("Took: {}m {}s {}ms\n", time_wip, secs, mils);
}
