#pragma once

#include "Type.h"

#include <iosfwd>

class Cell {
  public:
    virtual ~Cell() {}

    virtual Type getType() const = 0;
    virtual bool isEmpty() const;
    virtual bool isFormulaCell() const;

    virtual void print(std::ostream& out) const = 0;
    virtual const char* text() const = 0;
    virtual long long asInteger() const;
    virtual double asDouble() const;
    virtual Cell* clone() const = 0;

    static Cell* createFromText(const char* rawText);
};

class EmptyCell : public Cell {
  public:
    EmptyCell();
    EmptyCell(const EmptyCell& other);

    Type getType() const override;
    void print(std::ostream& out) const override;
    const char* text() const override;
    Cell* clone() const override;
};

class IntCell : public Cell {
  public:
    explicit IntCell(long long value);
    IntCell(const IntCell& other);

    Type getType() const override;
    void print(std::ostream& out) const override;
    const char* text() const override;
    long long asInteger() const override;
    Cell* clone() const override;

    long long getValue() const;

  private:
    long long value;
};

class DoubleCell : public Cell {
  public:
    explicit DoubleCell(double value);
    DoubleCell(const DoubleCell& other);

    Type getType() const override;
    void print(std::ostream& out) const override;
    const char* text() const override;
    double asDouble() const override;
    Cell* clone() const override;

    double getValue() const;

  private:
    double value;
};

class StringCell : public Cell {
  public:
    explicit StringCell(const char* value);
    StringCell(const StringCell& other);
    ~StringCell() override;

    Type getType() const override;
    void print(std::ostream& out) const override;
    const char* text() const override;
    Cell* clone() const override;

  private:
    char* value;
};

class DateCell : public Cell {
  public:
    explicit DateCell(const char* value);
    DateCell(const DateCell& other);
    ~DateCell() override;

    Type getType() const override;
    void print(std::ostream& out) const override;
    const char* text() const override;
    Cell* clone() const override;

  private:
    char* value;
};

class FormulaCell : public Cell {
  public:
    explicit FormulaCell(const char* expression);
    FormulaCell(const FormulaCell& other);
    ~FormulaCell() override;

    Type getType() const override;
    bool isFormulaCell() const override;
    void print(std::ostream& out) const override;
    const char* text() const override;
    Cell* clone() const override;

  private:
    char* expression;
};
