//
// Created by Denis on 16/09/2024.
//

#include "yaml-cpp/yaml.h"
#include <iostream>

int main()
{
    YAML::Emitter out;
    out << "Hello, World!";

    std::cout << "Here's the output YAML:\n" << out.c_str(); // prints "Hello, World!"
    return 0;
}