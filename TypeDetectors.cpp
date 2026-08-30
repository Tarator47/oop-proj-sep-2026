#include "TypeDetectors.h"

#include <stddef.h>
#include <stdlib.h>
#include <string.h>

namespace {
    bool isWhitespaceChar(char c) {
        return c == ' ' || c == '\t' || c == '\n' || c == '\r';
    }

    const char* trimLeadingWhitespace(const char* s) {
        if (s == nullptr) {
            return "";
        }

        while (*s != '\0' && isWhitespaceChar(*s)) {
            ++s;
        }

        return s;
    }

    bool isDigitSequence(const char* start, size_t length) {
        if (start == nullptr || length == 0) {
            return false;
        }

        for (size_t i = 0; i < length; ++i) {
            if (!(start[i] >= '0' && start[i] <= '9')) {
                return false;
            }
        }

        return true;
    }

    bool isLeapYear(int year) {
        return (year % 400 == 0) || (year % 4 == 0 && year % 100 != 0);
    }

    bool isValidDate(int day, int month, int year) {
        if (year < 1 || month < 1 || month > 12 || day < 1) {
            return false;
        }

        const int daysInMonth[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
        int maxDay = daysInMonth[month - 1];
        if (month == 2 && isLeapYear(year)) {
            maxDay = 29;
        }

        return day <= maxDay;
    }

    bool isValidDateString(const char* s, bool yearFirst) {
        if (s == nullptr || *s == '\0') {
            return false;
        }

        const char* firstDash = strchr(s, '-');
        if (firstDash == nullptr || firstDash == s) {
            return false;
        }

        const char* secondDash = strchr(firstDash + 1, '-');
        if (secondDash == nullptr || secondDash == firstDash + 1 || secondDash[1] == '\0') {
            return false;
        }

        if (strchr(secondDash + 1, '-') != nullptr) {
            return false;
        }

        size_t len1 = (size_t)(firstDash - s);
        size_t len2 = (size_t)(secondDash - (firstDash + 1));
        size_t len3 = strlen(secondDash + 1);

        if ((len1 == 2 && len2 == 2 && len3 == 4) || (len1 == 4 && len2 == 2 && len3 == 2)) {
            char part1[5] = {'\0'};
            char part2[3] = {'\0'};
            char part3[5] = {'\0'};

            memcpy(part1, s, len1);
            memcpy(part2, firstDash + 1, len2);
            memcpy(part3, secondDash + 1, len3);

            if (!isDigitSequence(part1, len1) || !isDigitSequence(part2, len2) || !isDigitSequence(part3, len3)) {
                return false;
            }

            int a = atoi(part1);
            int b = atoi(part2);
            int c = atoi(part3);

            if (yearFirst) {
                return isValidDate(c, b, a);
            }

            return isValidDate(a, b, c);
        }

        return false;
    }
} // namespace

bool isDigit(char c) {
    return c >= '0' && c <= '9';
}

bool isInteger(const char* s) {
    if (s == nullptr) {
        return false;
    }

    const char* begin = trimLeadingWhitespace(s);
    if (*begin == '\0') {
        return false;
    }

    const char* end = begin + strlen(begin);
    while (end > begin && isWhitespaceChar(end[-1])) {
        --end;
    }

    size_t length = (size_t)(end - begin);
    if (length == 0) {
        return false;
    }

    size_t i = 0;
    if (begin[i] == '+' || begin[i] == '-') {
        ++i;
    }

    if (i == length) {
        return false;
    }

    while (i < length) {
        if (!isDigit(begin[i])) {
            return false;
        }
        ++i;
    }

    return true;
}

bool isDouble(const char* s) {
    if (s == nullptr) {
        return false;
    }

    const char* begin = trimLeadingWhitespace(s);
    if (*begin == '\0') {
        return false;
    }

    const char* end = begin + strlen(begin);
    while (end > begin && isWhitespaceChar(end[-1])) {
        --end;
    }

    size_t length = (size_t)(end - begin);
    if (length == 0) {
        return false;
    }

    size_t i = 0;
    if (begin[i] == '+' || begin[i] == '-') {
        ++i;
    }

    bool hasDigitBeforeDot = false;
    bool hasDigitAfterDot = false;
    bool seenDot = false;

    while (i < length) {
        char c = begin[i];

        if (isDigit(c)) {
            if (seenDot) {
                hasDigitAfterDot = true;
            } else {
                hasDigitBeforeDot = true;
            }
            ++i;
            continue;
        }

        if (c == '.' && !seenDot) {
            seenDot = true;
            ++i;
            continue;
        }

        return false;
    }

    return seenDot && hasDigitBeforeDot && hasDigitAfterDot;
}

bool isQuotedString(const char* s) {
    if (s == nullptr) {
        return false;
    }

    const char* begin = trimLeadingWhitespace(s);
    if (*begin == '\0') {
        return false;
    }

    const char* end = begin + strlen(begin);
    while (end > begin && isWhitespaceChar(end[-1])) {
        --end;
    }

    size_t length = (size_t)(end - begin);
    if (length < 2 || begin[0] != '"' || begin[length - 1] != '"') {
        return false;
    }

    for (size_t i = 1; i < length - 1; ++i) {
        if (begin[i] == '\\') {
            if (i + 1 >= length - 1) {
                return false;
            }
            ++i;
            continue;
        }

        if (begin[i] == '"') {
            return false;
        }
    }

    return true;
}

bool isDateString(const char* s) {
    if (s == nullptr) {
        return false;
    }

    const char* begin = trimLeadingWhitespace(s);
    if (*begin == '\0') {
        return false;
    }

    const char* end = begin + strlen(begin);
    while (end > begin && isWhitespaceChar(end[-1])) {
        --end;
    }

    size_t length = (size_t)(end - begin);
    if (length == 0) {
        return false;
    }

    char buffer[64];
    if (length >= sizeof(buffer)) {
        return false;
    }

    memcpy(buffer, begin, length);
    buffer[length] = '\0';

    size_t dashCount = 0;
    for (size_t i = 0; i < length; ++i) {
        if (buffer[i] == '-') {
            ++dashCount;
        }
    }

    if (dashCount != 2) {
        return false;
    }

    return isValidDateString(buffer, true) || isValidDateString(buffer, false);
}

bool isFormula(const char* s) {
    if (s == nullptr) {
        return false;
    }

    const char* begin = trimLeadingWhitespace(s);
    if (*begin == '\0' || begin[0] != '=') {
        return false;
    }

    const char* end = begin + strlen(begin);
    while (end > begin && isWhitespaceChar(end[-1])) {
        --end;
    }

    size_t length = (size_t)(end - begin);
    if (length <= 1) {
        return false;
    }

    for (size_t i = 1; i < length; ++i) {
        char c = begin[i];
        if (isDigit(c) || c == '+' || c == '-' || c == '*' || c == '/' || c == '=' || c == '!' || c == '<' ||
            c == '>' || c == '.' || c == 'R' || c == 'r' || c == 'C' || c == 'c' || c == '(' || c == ')' || c == ' ' ||
            c == '\t' || c == '\n' || c == '\r') {
            continue;
        }

        return false;
    }

    return true;
}

Type detectCellType(const char* s) {
    if (s == nullptr) {
        return Type::Unknown;
    }

    const char* begin = trimLeadingWhitespace(s);
    if (*begin == '\0') {
        return Type::Empty;
    }

    const char* end = begin + strlen(begin);
    while (end > begin && isWhitespaceChar(end[-1])) {
        --end;
    }

    if (end == begin) {
        return Type::Empty;
    }

    if (isFormula(begin)) {
        return Type::Formula;
    }

    if (isQuotedString(begin)) {
        return Type::String;
    }

    if (isDateString(begin)) {
        return Type::Date;
    }

    if (isDouble(begin)) {
        return Type::Double;
    }

    if (isInteger(begin)) {
        return Type::Integer;
    }

    return Type::Unknown;
}
