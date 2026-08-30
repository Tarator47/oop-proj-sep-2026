#include <cstring>
#include <iostream>

#include "src/Cell.h"
#include "src/Table.h"

int main() {
    Table table;
    char currentFile[256] = "";
    char command[16];
    char arg[256];

    while (true) {
        std::cout << "> ";
        if (!(std::cin >> command)) {
            break;
        }

        if (strcmp(command, "open") == 0) {
            std::cin >> arg;
            if (table.loadFromFile(arg)) {
                strcpy(currentFile, arg);
                std::cout << "Loaded " << arg << "\n";
            } else {
                std::cout << "Error opening file\n";
            }
        } else if (strcmp(command, "print") == 0) {
            table.print(std::cout);
        } else if (strcmp(command, "edit") == 0) {
            size_t r = 0;
            size_t c = 0;
            std::cin >> r >> c;
            std::cin.ignore();
            std::cin.getline(arg, sizeof(arg));
            table.setCell(r, c, Cell::createFromText(arg));
            std::cout << "Edited R" << r << "C" << c << "\n";
        } else if (strcmp(command, "save") == 0) {
            if (strlen(currentFile) == 0) {
                std::cout << "No file opened\n";
            } else {
                table.saveToFile(currentFile);
                std::cout << "Saved\n";
            }
        } else if (strcmp(command, "saveas") == 0) {
            std::cin >> arg;
            table.saveToFile(arg);
            std::cout << "Saved as " << arg << "\n";
        } else if (strcmp(command, "close") == 0) {
            table.clear();
            currentFile[0] = '\0';
            std::cout << "Closed\n";
        } else if (strcmp(command, "exit") == 0) {
            break;
        } else {
            std::cout << "Unknown command\n";
        }
    }

    return 0;
}
