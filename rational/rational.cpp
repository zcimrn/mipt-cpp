#include "rational.h"
#include <iostream>
#include <numeric>

void Rational::Reduce() {
  int64_t factor = std::gcd(numerator_, denominator_);
  numerator_ /= factor;
  denominator_ /= factor;
}

Rational::Rational() {
  numerator_ = 0;
  denominator_ = 1;
}

Rational::Rational(int64_t new_numerator) {
  numerator_ = new_numerator;
  denominator_ = 1;
}

Rational::Rational(int64_t new_numerator, int64_t new_denominator) {
  if (new_denominator == 0) {
    throw RationalDivisionByZero{};
  }
  if (new_denominator > 0) {
    numerator_ = new_numerator;
    denominator_ = new_denominator;
  } else {
    numerator_ = -new_numerator;
    denominator_ = -new_denominator;
  }
  Reduce();
}

void Rational::SetNumerator(int64_t new_numerator) {
  numerator_ = new_numerator;
  Reduce();
}

int64_t Rational::GetNumerator() const {
  return numerator_;
}

void Rational::SetDenominator(int64_t new_denominator) {
  if (new_denominator == 0) {
    throw RationalDivisionByZero{};
  }
  denominator_ = new_denominator;
  if (denominator_ < 0) {
    numerator_ = -numerator_;
    denominator_ = -denominator_;
  }
  Reduce();
}

int64_t Rational::GetDenominator() const {
  return denominator_;
}

Rational Rational::operator+() const {
  return *this;
}

Rational Rational::operator-() const {
  return Rational(-numerator_, denominator_);
}

Rational operator+(const Rational a, const Rational b) {
  return Rational(a.numerator_ * b.denominator_ + b.numerator_ * a.denominator_, a.denominator_ * b.denominator_);
}

Rational operator-(const Rational a, const Rational b) {
  return Rational(a.numerator_ * b.denominator_ - b.numerator_ * a.denominator_, a.denominator_ * b.denominator_);
}

Rational operator*(const Rational a, const Rational b) {
  return Rational(a.numerator_ * b.numerator_, a.denominator_ * b.denominator_);
}

Rational operator/(const Rational a, const Rational b) {
  return Rational(a.numerator_ * b.denominator_, a.denominator_ * b.numerator_);
}

Rational& Rational::operator+=(const Rational other) {
  return *this = *this + other;
}

Rational& Rational::operator-=(const Rational other) {
  return *this = *this - other;
}

Rational& Rational::operator*=(const Rational other) {
  return *this = *this * other;
}

Rational& Rational::operator/=(const Rational other) {
  return *this = *this / other;
}

Rational& Rational::operator++() {
  return *this += 1;
}

Rational Rational::operator++(int) {
  auto result = *this;
  this->operator++();
  return result;
}

Rational& Rational::operator--() {
  return *this -= 1;
}

Rational Rational::operator--(int) {
  auto result = *this;
  this->operator--();
  return result;
}

bool operator<(const Rational a, const Rational b) {
  return a.numerator_ * b.denominator_ < b.numerator_ * a.denominator_;
}

bool operator>(const Rational a, const Rational b) {
  return a.numerator_ * b.denominator_ > b.numerator_ * a.denominator_;
}

bool operator==(const Rational a, const Rational b) {
  return a.numerator_ * b.denominator_ == b.numerator_ * a.denominator_;
}

bool operator!=(const Rational a, const Rational b) {
  return a.numerator_ * b.denominator_ != b.numerator_ * a.denominator_;
}

bool operator<=(const Rational a, const Rational b) {
  return a.numerator_ * b.denominator_ <= b.numerator_ * a.denominator_;
}

bool operator>=(const Rational a, const Rational b) {
  return a.numerator_ * b.denominator_ >= b.numerator_ * a.denominator_;
}

std::istream& operator>>(std::istream& in, Rational& rational) {
  std::string input;
  in >> input;
  size_t cursor = 0;
  for (; cursor < input.size() && input[cursor] != '/'; cursor++) {
  }
  if (input[cursor] == '/') {
    rational = Rational(stoll(input.substr(0, cursor)), stoll(input.substr(cursor + 1)));
  } else {
    rational = Rational(stoll(input));
  }
  return in;
}

std::ostream& operator<<(std::ostream& out, const Rational rational) {
  out << rational.numerator_;
  if (rational.denominator_ > 1) {
    out << '/' << rational.denominator_;
  }
  return out;
}
