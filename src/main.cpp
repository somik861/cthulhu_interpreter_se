#include "interpreter/iinterpreter.h"
#include "parser/iparser.h"
#include "utils/iset.h"

#include <filesystem>
#include <fstream>
#include <iostream>
#include <memory>
#include <string>

namespace fs = std::filesystem;

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

    auto parser = cthu::parser::IParser::createParserDummy();
    auto interpreter = cthu::interpreter::IInterpreter::createInterpreterDummy();

    interpreter->initExecution(parser->parse(source_code), &log_stream);
    while (interpreter->canContinue())
        interpreter->continueExecution();

    auto final_state = interpreter->getProgramState();
    if (final_state != nullptr) {
        auto stacks = final_state->getKnownStacks();
        auto iter = stacks->iterator();
        while (iter->hasNext())
            std::cout << iter->next() << '\n';
    }
}
