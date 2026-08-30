#pragma once

#include "Row.h"

#include <iosfwd>
#include <stddef.h>

class Table {
  public:
    Table();
    ~Table();

    Table(const Table& other);
    Table& operator=(const Table& other);
    Table(Table&& other) noexcept;
    Table& operator=(Table&& other) noexcept;

    size_t getRowCount() const;
    Row* getRow(size_t r);
    const Row* getRow(size_t r) const;

    void addRow(Row* row);
    void insertRow(size_t index, Row* row);
    void removeRow(size_t index);
    void clear();

    bool loadFromFile(const char* filename, char delimiter = ',');
    bool saveToFile(const char* filename, char delimiter = ',') const;
    void print(std::ostream& out) const;

    Cell* getCell(size_t r, size_t c);
    const Cell* getCell(size_t r, size_t c) const;
    bool setCell(size_t r, size_t c, Cell* cell);
    bool editCell(size_t r, size_t c, const char* text);

  private:
    void copyFrom(const Table& other);
    void free();
    void resize(size_t newCapacity);

    Row** rows;
    size_t count;
    size_t capacity;
};
