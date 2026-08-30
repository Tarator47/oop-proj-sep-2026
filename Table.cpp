#include "Table.h"

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

void Table::addRow(const Row& row) {
    if (count >= capacity) {
        size_t newCapacity = (capacity == 0) ? 4 : capacity * 2;
        resize(newCapacity);
    }

    rows[count] = new Row(row);
    ++count;
}

void Table::setRow(size_t index, const Row& row) {
    if (index >= capacity) {
        size_t newCapacity = (capacity == 0) ? 4 : capacity * 2;
        while (newCapacity <= index) {
            newCapacity *= 2;
        }
        resize(newCapacity);
    }

    if (index < count) {
        delete rows[index];
    }

    rows[index] = new Row(row);

    if (index >= count) {
        count = index + 1;
    }
}

Row* Table::getRow(size_t index) {
    if (index >= count) {
        return nullptr;
    }

    return rows[index];
}

const Row* Table::getRow(size_t index) const {
    if (index >= count) {
        return nullptr;
    }

    return rows[index];
}

size_t Table::getRowCount() const {
    return count;
}

void Table::clear() {
    free();
}

void Table::copyFrom(const Table& other) {
    if (this == &other) {
        return;
    }

    count = other.count;
    capacity = other.capacity;
    rows = new Row*[capacity == 0 ? 1 : capacity];

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
