#pragma once

#include "Row.h"

#include <stddef.h>

class Table {
  public:
    Table();
    ~Table();

    Table(const Table& other);
    Table& operator=(const Table& other);
    Table(Table&& other) noexcept;
    Table& operator=(Table&& other) noexcept;

    void addRow(const Row& row);
    void setRow(size_t index, const Row& row);
    Row* getRow(size_t index);
    const Row* getRow(size_t index) const;
    size_t getRowCount() const;
    void clear();

  private:
    void copyFrom(const Table& other);
    void free();
    void resize(size_t newCapacity);

    Row** rows;
    size_t count;
    size_t capacity;
};
