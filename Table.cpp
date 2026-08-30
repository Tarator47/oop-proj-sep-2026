#include "Table.h"

#include "RowParser.h"

#include <fstream>
#include <iostream>
#include <string>

void printTable(const Table& table, std::ostream& out) {
    table.print(out);
}

bool loadTable(Table& table, const char* filename, char delimiter) {
    return table.loadFromFile(filename, delimiter);
}

bool saveTable(const Table& table, const char* filename, char delimiter) {
    return table.saveToFile(filename, delimiter);
}

namespace {
    static char detectDelimiterFromFile(const char* filename) {
        std::ifstream input(filename);
        if (!input.is_open()) {
            return ',';
        }

        std::string line;
        if (!std::getline(input, line)) {
            return ',';
        }

        int comma = 0;
        int semicolon = 0;
        int tab = 0;
        bool inQuotes = false;

        for (size_t i = 0; i < line.length(); ++i) {
            char c = line[i];
            if (c == '"') {
                if (inQuotes && i + 1 < line.length() && line[i + 1] == '"') {
                    ++i;
                } else {
                    inQuotes = !inQuotes;
                }
                continue;
            }

            if (!inQuotes) {
                if (c == ',') {
                    ++comma;
                } else if (c == ';') {
                    ++semicolon;
                } else if (c == '\t') {
                    ++tab;
                }
            }
        }

        if (semicolon > comma && semicolon > tab) {
            return ';';
        }
        if (tab > comma && tab > semicolon) {
            return '\t';
        }
        return ',';
    }

    static void writeSerializedCell(std::ostream& out, const Cell* cell) {
        if (cell == nullptr) {
            return;
        }

        if (cell->getType() == Type::String) {
            const char* raw = cell->text();
            out << '"';
            for (size_t i = 0; raw[i] != '\0'; ++i) {
                if (raw[i] == '"') {
                    out << '\\';
                } else if (raw[i] == '\\') {
                    out << '\\';
                }
                out << raw[i];
            }
            out << '"';
            return;
        }

        cell->print(out);
    }
} // namespace

Table::Table()
    : rows(nullptr), count(0), capacity(0) {
}

Table::~Table() {
    free();
}

Table::Table(const Table& other)
    : rows(nullptr), count(0), capacity(0) {
    copyFrom(other);
}

Table& Table::operator=(const Table& other) {
    if (this != &other) {
        Table temp(other);
        Row** tmpRows = rows;
        rows = temp.rows;
        temp.rows = tmpRows;

        size_t tmpCount = count;
        count = temp.count;
        temp.count = tmpCount;

        size_t tmpCapacity = capacity;
        capacity = temp.capacity;
        temp.capacity = tmpCapacity;
    }

    return *this;
}

Table::Table(Table&& other) noexcept
    : rows(other.rows), count(other.count), capacity(other.capacity) {
    other.rows = nullptr;
    other.count = 0;
    other.capacity = 0;
}

Table& Table::operator=(Table&& other) noexcept {
    if (this != &other) {
        free();
        rows = other.rows;
        count = other.count;
        capacity = other.capacity;

        other.rows = nullptr;
        other.count = 0;
        other.capacity = 0;
    }

    return *this;
}

size_t Table::getRowCount() const {
    return count;
}

Row* Table::getRow(size_t r) {
    if (r >= count) {
        return nullptr;
    }
    return rows[r];
}

const Row* Table::getRow(size_t r) const {
    if (r >= count) {
        return nullptr;
    }
    return rows[r];
}

void Table::addRow(Row* row) {
    if (row == nullptr) {
        return;
    }

    if (count >= capacity) {
        size_t newCapacity = (capacity == 0) ? 4 : capacity * 2;
        resize(newCapacity);
    }

    rows[count] = row;
    ++count;
}

void Table::insertRow(size_t index, Row* row) {
    if (row == nullptr) {
        return;
    }

    if (index > count) {
        index = count;
    }

    if (count >= capacity) {
        size_t newCapacity = (capacity == 0) ? 4 : capacity * 2;
        while (newCapacity <= count) {
            newCapacity *= 2;
        }
        resize(newCapacity);
    }

    for (size_t i = count; i > index; --i) {
        rows[i] = rows[i - 1];
    }

    rows[index] = row;
    ++count;
}

void Table::removeRow(size_t index) {
    if (index >= count) {
        return;
    }

    delete rows[index];
    for (size_t i = index; i + 1 < count; ++i) {
        rows[i] = rows[i + 1];
    }
    rows[count - 1] = nullptr;
    --count;
}

void Table::clear() {
    free();
}

bool Table::loadFromFile(const char* filename, char delimiter) {
    if (filename == nullptr) {
        return false;
    }

    std::ifstream input(filename);
    if (!input.is_open()) {
        return false;
    }

    if (delimiter == '\0') {
        delimiter = detectDelimiterFromFile(filename);
    }

    clear();

    std::string line;
    while (std::getline(input, line)) {
        Row row = parseRow(line.c_str(), delimiter);
        addRow(new Row(std::move(row)));
    }

    return true;
}

bool Table::saveToFile(const char* filename, char delimiter) const {
    if (filename == nullptr) {
        return false;
    }

    std::ofstream output(filename, std::ios::out | std::ios::trunc);
    if (!output.is_open()) {
        return false;
    }

    for (size_t r = 0; r < count; ++r) {
        const Row* row = rows[r];
        if (row == nullptr) {
            continue;
        }

        for (size_t c = 0; c < row->getCellCount(); ++c) {
            const Cell* cell = row->getCell(c);
            if (cell != nullptr) {
                writeSerializedCell(output, cell);
            }
            if (c + 1 < row->getCellCount()) {
                output << delimiter;
            }
        }

        output << '\n';
    }

    return true;
}

void Table::print(std::ostream& out) const {
    for (size_t r = 0; r < count; ++r) {
        const Row* row = rows[r];
        if (row == nullptr) {
            out << "[empty row]";
            if (r + 1 < count) {
                out << '\n';
            }
            continue;
        }

        for (size_t c = 0; c < row->getCellCount(); ++c) {
            const Cell* cell = row->getCell(c);
            if (cell != nullptr) {
                cell->print(out);
            }
            if (c + 1 < row->getCellCount()) {
                out << " | ";
            }
        }
        out << '\n';
    }
}

Cell* Table::getCell(size_t r, size_t c) {
    Row* row = getRow(r);
    if (row == nullptr) {
        return nullptr;
    }
    return row->getCell(c);
}

const Cell* Table::getCell(size_t r, size_t c) const {
    const Row* row = getRow(r);
    if (row == nullptr) {
        return nullptr;
    }
    return row->getCell(c);
}

bool Table::setCell(size_t r, size_t c, Cell* cell) {
    if (cell == nullptr) {
        return false;
    }

    if (r >= count) {
        size_t newRowCount = r + 1;
        if (capacity < newRowCount) {
            size_t newCapacity = (capacity == 0) ? 4 : capacity * 2;
            while (newCapacity < newRowCount) {
                newCapacity *= 2;
            }
            resize(newCapacity);
        }

        while (count < newRowCount) {
            rows[count] = new Row();
            ++count;
        }
    }

    Row* row = rows[r];
    if (row == nullptr) {
        row = new Row();
        rows[r] = row;
    }

    row->setCell(c, cell);
    return true;
}

bool Table::editCell(size_t r, size_t c, const char* text) {
    if (text == nullptr) {
        return false;
    }

    Cell* cell = Cell::createFromText(text);
    bool success = setCell(r, c, cell);
    if (!success) {
        delete cell;
    }

    return success;
}

void Table::copyFrom(const Table& other) {
    if (this == &other) {
        return;
    }

    count = other.count;
    capacity = other.capacity == 0 ? 1 : other.capacity;
    rows = new Row*[capacity];

    for (size_t i = 0; i < capacity; ++i) {
        rows[i] = nullptr;
    }

    for (size_t i = 0; i < count; ++i) {
        rows[i] = new Row(*other.rows[i]);
    }
}

void Table::free() {
    if (rows == nullptr) {
        return;
    }

    for (size_t i = 0; i < count; ++i) {
        delete rows[i];
    }

    delete[] rows;
    rows = nullptr;
    count = 0;
    capacity = 0;
}

void Table::resize(size_t newCapacity) {
    Row** newRows = new Row*[newCapacity];
    for (size_t i = 0; i < newCapacity; ++i) {
        newRows[i] = nullptr;
    }

    for (size_t i = 0; i < count; ++i) {
        newRows[i] = rows[i];
    }

    delete[] rows;
    rows = newRows;
    capacity = newCapacity;
}
