#include "Row.h"

#include <stddef.h>

Row::Row()
    : cells(nullptr), count(0), capacity(0) {
}

Row::~Row() {
    free();
}

Row::Row(const Row& other)
    : cells(nullptr), count(0), capacity(0) {
    copyFrom(other);
}

Row& Row::operator=(const Row& other) {
    if (this != &other) {
        Row temp(other);
        Cell** tmpCells = cells;
        cells = temp.cells;
        temp.cells = tmpCells;

        size_t tmpCount = count;
        count = temp.count;
        temp.count = tmpCount;

        size_t tmpCapacity = capacity;
        capacity = temp.capacity;
        temp.capacity = tmpCapacity;
    }

    return *this;
}

Row::Row(Row&& other) noexcept
    : cells(other.cells), count(other.count), capacity(other.capacity) {
    other.cells = nullptr;
    other.count = 0;
    other.capacity = 0;
}

Row& Row::operator=(Row&& other) noexcept {
    if (this != &other) {
        free();
        cells = other.cells;
        count = other.count;
        capacity = other.capacity;

        other.cells = nullptr;
        other.count = 0;
        other.capacity = 0;
    }

    return *this;
}

void Row::addCell(Cell* cell) {
    if (cell == nullptr) {
        return;
    }

    if (count >= capacity) {
        size_t newCapacity = (capacity == 0) ? 4 : capacity * 2;
        resize(newCapacity);
    }

    cells[count] = cell;
    ++count;
}

void Row::setCell(size_t index, Cell* cell) {
    if (cell == nullptr) {
        return;
    }

    if (index >= capacity) {
        size_t newCapacity = (capacity == 0) ? 4 : capacity * 2;
        while (newCapacity <= index) {
            newCapacity *= 2;
        }
        resize(newCapacity);
    }

    for (size_t i = count; i < index; ++i) {
        cells[i] = new EmptyCell();
    }

    if (index < count) {
        delete cells[index];
    }

    cells[index] = cell;

    if (index >= count) {
        count = index + 1;
    }
}

Cell* Row::getCell(size_t index) {
    if (index >= count) {
        return nullptr;
    }

    return cells[index];
}

const Cell* Row::getCell(size_t index) const {
    if (index >= count) {
        return nullptr;
    }

    return cells[index];
}

size_t Row::getCellCount() const {
    return count;
}

void Row::clear() {
    free();
}

void Row::copyFrom(const Row& other) {
    if (this == &other) {
        return;
    }

    count = other.count;
    capacity = (other.capacity == 0) ? 4 : other.capacity;
    cells = new Cell*[capacity];

    for (size_t i = 0; i < count; ++i) {
        cells[i] = other.cells[i]->clone();
    }
}

void Row::free() {
    if (cells == nullptr) {
        return;
    }

    for (size_t i = 0; i < count; ++i) {
        delete cells[i];
    }

    delete[] cells;
    cells = nullptr;
    count = 0;
    capacity = 0;
}

void Row::resize(size_t newCapacity) {
    Cell** newCells = new Cell*[newCapacity];
    for (size_t i = 0; i < count; ++i) {
        newCells[i] = cells[i];
    }

    delete[] cells;
    cells = newCells;
    capacity = newCapacity;
}
