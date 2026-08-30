#include <iostream>
#include "TypeDetectors.h"

int main() {
    std::cout << isInteger("123")    << " (1)\n";
    std::cout << isInteger("-123")   << " (1)\n";
    std::cout << isInteger("+123")   << " (1)\n";
    std::cout << isInteger("0")      << " (1)\n";
    std::cout << isInteger("12.3")   << " (0)\n";
    std::cout << isInteger("12a")    << " (0)\n";
    std::cout << isInteger("+")      << " (0)\n";
    std::cout << isInteger("")       << " (0)\n";
    std::cout << isInteger(" 123")   << " (0)\n";

    return 0;
}