#include "program/dict.hpp"
#include "program/safe_stack.hpp"
#include "program/stack.hpp"
#include "program/word.hpp"

#include <iostream>

int main() {
    cthu::program::SafeStack s;

    s.push(cthu::program::Word(12));
    auto s2 = s;

    std::cout << s2.peek<cthu::program::Word>() << '\n';
    std::cout << s2 << '\n';
    std::cout << s2.toJson() << '\n';

    std::cout << "Hello word\n";
}
