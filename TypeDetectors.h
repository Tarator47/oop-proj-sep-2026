#pragma once

#include "Type.h"

bool isDigit(char c);
bool isInteger(const char* s);
bool isDouble(const char* s);
bool isQuotedString(const char* s);
bool isDateString(const char* s);
bool isFormula(const char* s);
Type detectCellType(const char* s);
