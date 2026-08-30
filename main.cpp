#include <iostream>

#include "Cell.h"
#include "Row.h"
#include "RowParser.h"
#include "Table.h"
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

    std::cout << static_cast<int>(intCell->getType()) << " ";
    intCell->print(std::cout);
    std::cout << "\n";
    std::cout << static_cast<int>(doubleCell->getType()) << " ";
    doubleCell->print(std::cout);
    std::cout << "\n";
    std::cout << static_cast<int>(stringCell->getType()) << " ";
    stringCell->print(std::cout);
    std::cout << "\n";
    std::cout << static_cast<int>(dateCell->getType()) << " ";
    dateCell->print(std::cout);
    std::cout << "\n";
    std::cout << static_cast<int>(formulaCell->getType()) << " ";
    formulaCell->print(std::cout);
    std::cout << "\n";
    std::cout << emptyCell->isEmpty() << "\n";

    delete intCell;
    delete doubleCell;
    delete stringCell;
    delete dateCell;
    delete formulaCell;
    delete emptyCell;

    std::cout << "\nRow tests:\n";
    Row row;
    row.addCell(Cell::createFromText("123"));
    row.addCell(Cell::createFromText("\"Hello\""));
    row.addCell(Cell::createFromText("2024-02-29"));

    std::cout << row.getCellCount() << "\n";
    std::cout << row.getCell(0)->text() << "\n";
    std::cout << row.getCell(1)->text() << "\n";
    std::cout << row.getCell(2)->text() << "\n";

    Row copyRow(row);
    std::cout << copyRow.getCellCount() << "\n";
    std::cout << copyRow.getCell(1)->text() << "\n";

    Row movedRow(std::move(row));
    std::cout << movedRow.getCellCount() << "\n";
    std::cout << movedRow.getCell(0)->text() << "\n";

    std::cout << "\nParser tests:\n";
    Row parsed = parseRow("10, \"Hello, world!\", 123.56", ',');
    std::cout << parsed.getCellCount() << "\n";
    std::cout << parsed.getCell(0)->text() << "\n";
    std::cout << parsed.getCell(1)->text() << "\n";
    std::cout << parsed.getCell(2)->text() << "\n";

    Row parsedEmpty = parseRow("10,,1000,", ',');
    std::cout << parsedEmpty.getCellCount() << "\n";
    for (size_t i = 0; i < parsedEmpty.getCellCount(); ++i) {
        std::cout << i << ": '";
        parsedEmpty.getCell(i)->print(std::cout);
        std::cout << "'\n";
    }

    Row parsedEscaped = parseRow("\"C:\\\\temp\\\\\", \"\\\"Quoted\\\"\"", ',');
    std::cout << parsedEscaped.getCellCount() << "\n";
    std::cout << parsedEscaped.getCell(0)->text() << "\n";
    std::cout << parsedEscaped.getCell(1)->text() << "\n";

    std::cout << "\nTable tests:\n";
    Table table;
    table.addRow(new Row(parseRow("10, 20, 30", ',')));
    table.addRow(new Row(parseRow("\"Hello\", 42", ',')));
    table.setCell(5, 10, Cell::createFromText("99"));
    std::cout << table.getRowCount() << "\n";
    std::cout << table.getRow(0)->getCellCount() << "\n";
    std::cout << table.getRow(1)->getCell(0)->text() << "\n";
    std::cout << table.getCell(5, 10)->text() << "\n";

    table.editCell(1, 1, "777");
    std::cout << table.getRow(1)->getCell(1)->text() << "\n";

    table.saveToFile("table_test.csv", ',');

    Table loaded;
    loaded.loadFromFile("table_test.csv", ',');
    std::cout << loaded.getRowCount() << "\n";
    std::cout << loaded.getRow(5)->getCell(10)->text() << "\n";

    return 0;
}
