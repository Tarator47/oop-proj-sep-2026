#pragma once

#include "Cell.h"

#include <stddef.h>

class Row {
  public:
    Row();
    ~Row();

    Row(const Row& other);
    Row& operator=(const Row& other);
    Row(Row&& other) noexcept;
    Row& operator=(Row&& other) noexcept;

    void addCell(Cell* cell);
    void setCell(size_t index, Cell* cell);
    Cell* getCell(size_t index);
    const Cell* getCell(size_t index) const;
    size_t getCellCount() const;
    void clear();

  private:
    void copyFrom(const Row& other);
    void free();
    void resize(size_t newCapacity);

    Cell** cells;
    size_t count;
    size_t capacity;
};
