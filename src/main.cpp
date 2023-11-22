#include "program/stack.hpp"
#include "program/word.hpp"
#include <iostream>

int main() {
    cthu::program::Stack s;

    s.push(cthu::program::Word(12));
    auto s2 = s;
    std::cout << s2.pop<cthu::program::Word>() << '\n';

    std::cout << "Hello word\n";
}
