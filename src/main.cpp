#include <common/InterpreterException.hpp>
#include <interpreter/Interpreter.hpp>
#include <iostream>
#include <fstream>
#include <cassert>

int main(int argc, char** argv) {
    if (argc != 2) {
        std::cout << "./program source_code";
        return 1;
    }

    try {
        interpreter::Interpreter intr;
        std::ifstream codestream(argv[1]);
        intr.loadStateFromStream(codestream);
        if (!intr.hasWork())
            throw common::InterpreterException("No code to interpret");
        intr.run();
        std::cout << "Interpreter finished:\n";
        std::cout << intr.lastProcessedState()->toString(2) << '\n';

    } catch (const common::InterpreterException& exc) {
        std::cout << "Interpreter error:\n" << exc.what();
        return 1;
    } catch (const std::exception& exc) {
        std::cout << "Program error:\n" << exc.what();
        return 1;
    }
}