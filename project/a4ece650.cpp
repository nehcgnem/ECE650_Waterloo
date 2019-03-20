#include <iostream>
#include <string>
#include <malloc.h>

#include "datatype.h"
#include "PlainInterpreter.h"



int main() {

    PlainInterpreter interpreter = PlainInterpreter();
    std::string input;

    while (std::cin) {
        std::getline(std::cin, input);
        try{
            interpreter.call(input);
        }
        catch (char const* e) {
            std::cout<<e<<'\n';
            continue;
        }

    }
    return 0;
}