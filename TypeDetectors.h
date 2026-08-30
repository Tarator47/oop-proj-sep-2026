#pragma once

#include "Type.h"

// Помощни функции
bool isDigit(char c);

// Детектори на типове — приемат ИЗЧИСТЕН низ (без whitespace около него)
bool isInteger(const char* s);
bool isDouble(const char* s);
bool isQuotedString(const char* s);
bool isDateString(const char* s);
bool isFormula(const char* s);

// Обща функция за детекция на тип на клетка.
// Ако s е nullptr или състои се само от whitespace, връща Type::Empty.
Type detectCellType(const char* s);
