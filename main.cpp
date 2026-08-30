#include <iostream>

#include "Cell.h"
#include "TypeDetectors.h"

int main() {
    std::cout << "Detector tests:\n";
    std::cout << isInteger("123") << " (1)\n";
    std::cout << isInteger("-123") << " (1)\n";
    std::cout << isInteger("+123") << " (1)\n";
    std::cout << isInteger("0") << " (1)\n";
    std::cout << isInteger("12.3") << " (0)\n";
    std::cout << isInteger("12a") << " (0)\n";
    std::cout << isInteger("+") << " (0)\n";
    std::cout << isInteger("") << " (0)\n";
    std::cout << isInteger(" 123") << " (1)\n";

    std::cout << "\nCell factory tests:\n";
    Cell* intCell = Cell::createFromText("123");
    Cell* doubleCell = Cell::createFromText("123.45");
    Cell* stringCell = Cell::createFromText("\"Hello\"");
    Cell* dateCell = Cell::createFromText("29-02-2024");
    Cell* formulaCell = Cell::createFromText("=R1C1+10");
    Cell* emptyCell = Cell::createFromText("   ");

    std::cout << static_cast<int>(intCell->getType()) << " "; intCell->print(std::cout); std::cout << "\n";
    std::cout << static_cast<int>(doubleCell->getType()) << " "; doubleCell->print(std::cout); std::cout << "\n";
    std::cout << static_cast<int>(stringCell->getType()) << " "; stringCell->print(std::cout); std::cout << "\n";
    std::cout << static_cast<int>(dateCell->getType()) << " "; dateCell->print(std::cout); std::cout << "\n";
    std::cout << static_cast<int>(formulaCell->getType()) << " "; formulaCell->print(std::cout); std::cout << "\n";
    std::cout << emptyCell->isEmpty() << "\n";

    delete intCell;
    delete doubleCell;
    delete stringCell;
    delete dateCell;
    delete formulaCell;
    delete emptyCell;

    return 0;
}