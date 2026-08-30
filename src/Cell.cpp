#include "Cell.h"

#include "TypeDetectors.h"

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <ctype.h>
#include <iostream>

namespace {
    char* cloneCString(const char* source) {
        if (source == nullptr) {
            return new char[1]{'\0'};
        }

        size_t length = strlen(source) + 1;
        char* copy = new char[length];
        strcpy(copy, source);
        return copy;
    }

    void trimInPlace(char* text) {
        if (text == nullptr) {
            return;
        }

        size_t start = 0;
        while (text[start] != '\0' && isspace((unsigned char)text[start])) {
            ++start;
        }

        if (start > 0) {
            size_t length = strlen(text + start) + 1;
            memmove(text, text + start, length);
        }

        size_t end = strlen(text);
        while (end > 0 && isspace((unsigned char)text[end - 1])) {
            text[--end] = '\0';
        }
    }

    char* trimClone(const char* source) {
        char* copy = cloneCString(source == nullptr ? "" : source);
        trimInPlace(copy);
        char* result = cloneCString(copy);
        delete[] copy;
        return result;
    }

    char* unescapeQuotedString(const char* token) {
        if (token == nullptr) {
            return cloneCString("");
        }

        size_t len = strlen(token);
        if (len < 2 || token[0] != '"' || token[len - 1] != '"') {
            return cloneCString(token);
        }

        char* result = new char[len];
        size_t out = 0;

        for (size_t i = 1; i + 1 < len; ++i) {
            if (token[i] == '\\' && i + 1 < len - 1) {
                ++i;
                result[out++] = token[i];
            } else {
                result[out++] = token[i];
            }
        }

        result[out] = '\0';
        return result;
    }
}

bool Cell::isEmpty() const {
    return getType() == Type::Empty;
}

bool Cell::isFormulaCell() const {
    return false;
}

long long Cell::asInteger() const {
    return 0;
}

double Cell::asDouble() const {
    return 0.0;
}

Cell* Cell::createFromText(const char* rawText) {
    char* trimmed = trimClone(rawText);

    if (trimmed[0] == '\0') {
        delete[] trimmed;
        return new EmptyCell();
    }

    Type type = detectCellType(trimmed);
    Cell* result = nullptr;

    switch (type) {
    case Type::Empty:
        result = new EmptyCell();
        break;
    case Type::Integer:
        result = new IntCell(atoll(trimmed));
        break;
    case Type::Double:
        result = new DoubleCell(atof(trimmed));
        break;
    case Type::String:
        result = new StringCell(trimmed);
        break;
    case Type::Date:
        result = new DateCell(trimmed);
        break;
    case Type::Formula:
        result = new FormulaCell(trimmed);
        break;
    default:
        result = new EmptyCell();
        break;
    }

    delete[] trimmed;
    return result;
}

EmptyCell::EmptyCell() {}

EmptyCell::EmptyCell(const EmptyCell& other) {
    (void)other;
}

Type EmptyCell::getType() const {
    return Type::Empty;
}

void EmptyCell::print(std::ostream& out) const {
    (void)out;
}

const char* EmptyCell::text() const {
    return "";
}

Cell* EmptyCell::clone() const {
    return new EmptyCell(*this);
}

IntCell::IntCell(long long value) : value(value) {}

IntCell::IntCell(const IntCell& other) : value(other.value) {}

Type IntCell::getType() const {
    return Type::Integer;
}

void IntCell::print(std::ostream& out) const {
    out << value;
}

const char* IntCell::text() const {
    static char buffer[64];
    snprintf(buffer, sizeof(buffer), "%lld", value);
    return buffer;
}

long long IntCell::asInteger() const {
    return value;
}

Cell* IntCell::clone() const {
    return new IntCell(*this);
}

long long IntCell::getValue() const {
    return value;
}

DoubleCell::DoubleCell(double value) : value(value) {}

DoubleCell::DoubleCell(const DoubleCell& other) : value(other.value) {}

Type DoubleCell::getType() const {
    return Type::Double;
}

void DoubleCell::print(std::ostream& out) const {
    out << value;
}

const char* DoubleCell::text() const {
    static char buffer[64];
    snprintf(buffer, sizeof(buffer), "%g", value);
    return buffer;
}

double DoubleCell::asDouble() const {
    return value;
}

Cell* DoubleCell::clone() const {
    return new DoubleCell(*this);
}

double DoubleCell::getValue() const {
    return value;
}

StringCell::StringCell(const char* value) {
    char* raw = cloneCString(value == nullptr ? "" : value);
    char* prepared = raw;

    if (raw[0] == '"' && raw[strlen(raw) - 1] == '"') {
        char* unescaped = unescapeQuotedString(raw);
        delete[] raw;
        prepared = unescaped;
    }

    this->value = prepared;
}

StringCell::StringCell(const StringCell& other) : value(cloneCString(other.value == nullptr ? "" : other.value)) {}

StringCell::~StringCell() {
    delete[] value;
}

Type StringCell::getType() const {
    return Type::String;
}

void StringCell::print(std::ostream& out) const {
    out << value;
}

const char* StringCell::text() const {
    return value;
}

Cell* StringCell::clone() const {
    return new StringCell(*this);
}

DateCell::DateCell(const char* value) : value(cloneCString(value == nullptr ? "" : value)) {}

DateCell::DateCell(const DateCell& other) : value(cloneCString(other.value == nullptr ? "" : other.value)) {}

DateCell::~DateCell() {
    delete[] value;
}

Type DateCell::getType() const {
    return Type::Date;
}

void DateCell::print(std::ostream& out) const {
    out << value;
}

const char* DateCell::text() const {
    return value;
}

Cell* DateCell::clone() const {
    return new DateCell(*this);
}

FormulaCell::FormulaCell(const char* expression) : expression(cloneCString(expression == nullptr ? "" : expression)) {}

FormulaCell::FormulaCell(const FormulaCell& other) : expression(cloneCString(other.expression == nullptr ? "" : other.expression)) {}

FormulaCell::~FormulaCell() {
    delete[] expression;
}

Type FormulaCell::getType() const {
    return Type::Formula;
}

bool FormulaCell::isFormulaCell() const {
    return true;
}

void FormulaCell::print(std::ostream& out) const {
    out << expression;
}

const char* FormulaCell::text() const {
    return expression;
}

Cell* FormulaCell::clone() const {
    return new FormulaCell(*this);
}
