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

    static Cell* createFromText(const char* rawText);
};

class EmptyCell : public Cell {
  public:
    EmptyCell();

    Type getType() const override;
    void print(std::ostream& out) const override;
    const char* text() const override;
};

class IntCell : public Cell {
  public:
    explicit IntCell(long long value);

    Type getType() const override;
    void print(std::ostream& out) const override;
    const char* text() const override;
    long long asInteger() const override;

    long long getValue() const;

  private:
    long long value;
};

class DoubleCell : public Cell {
  public:
    explicit DoubleCell(double value);

    Type getType() const override;
    void print(std::ostream& out) const override;
    const char* text() const override;
    double asDouble() const override;

    double getValue() const;

  private:
    double value;
};

class StringCell : public Cell {
  public:
    explicit StringCell(const char* value);
    ~StringCell() override;

    Type getType() const override;
    void print(std::ostream& out) const override;
    const char* text() const override;

  private:
    char* value;
};

class DateCell : public Cell {
  public:
    explicit DateCell(const char* value);
    ~DateCell() override;

    Type getType() const override;
    void print(std::ostream& out) const override;
    const char* text() const override;

  private:
    char* value;
};

class FormulaCell : public Cell {
  public:
    explicit FormulaCell(const char* expression);
    ~FormulaCell() override;

    Type getType() const override;
    bool isFormulaCell() const override;
    void print(std::ostream& out) const override;
    const char* text() const override;

  private:
    char* expression;
};
