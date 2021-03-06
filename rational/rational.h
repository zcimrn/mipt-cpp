#ifndef RATIONAL_H_
#define RATIONAL_H_

#include <iostream>
#include <stdexcept>

class RationalDivisionByZero : public std::runtime_error {
 public:
  RationalDivisionByZero() : std::runtime_error("RationalDivisionByZero") {
  }
};

class Rational {
 public:
  Rational();
  Rational(int64_t new_numerator);  // NOLINT
  Rational(int64_t new_numerator, int64_t new_denominator);

  void SetNumerator(int64_t new_numerator);
  int64_t GetNumerator() const;

  void SetDenominator(int64_t new_denominator);
  int64_t GetDenominator() const;

  Rational operator+() const;
  Rational operator-() const;

  friend Rational operator+(const Rational& left, const Rational& right);
  friend Rational operator-(const Rational& left, const Rational& right);
  friend Rational operator*(const Rational& left, const Rational& right);
  friend Rational operator/(const Rational& left, const Rational& right);

  Rational& operator+=(const Rational& other);
  Rational& operator-=(const Rational& other);
  Rational& operator*=(const Rational& other);
  Rational& operator/=(const Rational& other);

  Rational& operator++();
  Rational operator++(int);
  Rational& operator--();
  Rational operator--(int);

  friend bool operator<(const Rational& left, const Rational& right);
  friend bool operator>(const Rational& left, const Rational& right);
  friend bool operator==(const Rational& left, const Rational& right);
  friend bool operator!=(const Rational& left, const Rational& right);
  friend bool operator<=(const Rational& left, const Rational& right);
  friend bool operator>=(const Rational& left, const Rational& right);

  friend std::istream& operator>>(std::istream& in, Rational& rational);
  friend std::ostream& operator<<(std::ostream& out, const Rational rational);

 private:
  int64_t numerator_, denominator_;

  void Reduce();
};

#endif
