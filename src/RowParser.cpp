#include "RowParser.h"

#include "Cell.h"

#include <cstring>

namespace {
    void bufferAppend(char*& buf, size_t& len, size_t& cap, char c) {
        if (len + 1 >= cap) {
            size_t newCap = (cap == 0) ? 16 : cap * 2;
            char* newBuf = new char[newCap];
            if (buf != nullptr) {
                memcpy(newBuf, buf, len);
                delete[] buf;
            }
            buf = newBuf;
            cap = newCap;
        }

        buf[len++] = c;
    }
}

Row parseRow(const char* line, char delimiter) {
    Row row;
    if (line == nullptr) {
        return row;
    }

    char* buf = nullptr;
    size_t bufLen = 0;
    size_t bufCap = 0;
    enum State {
        NORMAL,
        IN_STRING,
        AFTER_BACKSLASH
    };

    State state = NORMAL;
    size_t i = 0;
    while (line[i] != '\0') {
        char c = line[i];

        switch (state) {
        case NORMAL:
            if (c == delimiter) {
                bufferAppend(buf, bufLen, bufCap, '\0');
                row.addCell(Cell::createFromText(buf));
                bufLen = 0;
            } else if (c == '"') {
                bufferAppend(buf, bufLen, bufCap, c);
                state = IN_STRING;
            } else {
                bufferAppend(buf, bufLen, bufCap, c);
            }
            break;

        case IN_STRING:
            if (c == '\\') {
                bufferAppend(buf, bufLen, bufCap, c);
                state = AFTER_BACKSLASH;
            } else if (c == '"') {
                bufferAppend(buf, bufLen, bufCap, c);
                state = NORMAL;
            } else {
                bufferAppend(buf, bufLen, bufCap, c);
            }
            break;

        case AFTER_BACKSLASH:
            bufferAppend(buf, bufLen, bufCap, c);
            state = IN_STRING;
            break;
        }

        ++i;
    }

    bufferAppend(buf, bufLen, bufCap, '\0');
    row.addCell(Cell::createFromText(buf));
    delete[] buf;
    return row;
}
