#ifndef MATRIX_H_
#define MATRIX_H_

#include <stdexcept>
#include <iostream>

class MatrixIsDegenerateError : public std::runtime_error {
 public:
  MatrixIsDegenerateError() : std::runtime_error("MatrixIsDegenerateError") {
  }
};

class MatrixOutOfRange : public std::out_of_range {
 public:
  MatrixOutOfRange() : std::out_of_range("MatrixOutOfRange") {
  }
};

template <class T, size_t N, size_t M>
class Matrix {
 public:
  T matrix[N][M];

  size_t RowsNumber() const {
    return N;
  }

  size_t ColumnsNumber() const {
    return M;
  }

  T& operator()(size_t i, size_t j) {
    return matrix[i][j];
  }

  const T& operator()(size_t i, size_t j) const {
    return matrix[i][j];
  }

  T& At(size_t i, size_t j) {
    if (i >= N || j >= M) {
      throw MatrixOutOfRange{};
    }
    return matrix[i][j];
  }

  const T& At(size_t i, size_t j) const {
    if (i >= N || j >= M) {
      throw MatrixOutOfRange{};
    }
    return matrix[i][j];
  }

  Matrix<T, N, M>& operator+=(const Matrix<T, N, M>& other) {
    for (size_t i = 0; i < N; i++) {
      for (size_t j = 0; j < M; j++) {
        matrix[i][j] += other.matrix[i][j];
      }
    }
    return *this;
  }

  Matrix<T, N, M> operator+(const Matrix<T, N, M>& other) const {
    auto result = *this;
    return result += other;
  }

  Matrix<T, N, M>& operator-=(const Matrix<T, N, M>& other) {
    for (size_t i = 0; i < N; i++) {
      for (size_t j = 0; j < M; j++) {
        matrix[i][j] -= other.matrix[i][j];
      }
    }
    return *this;
  }

  Matrix<T, N, M> operator-(const Matrix<T, N, M>& other) const {
    auto result = *this;
    return result -= other;
  }

  template <size_t L>
  Matrix<T, N, L> operator*(const Matrix<T, M, L>& other) const {
    Matrix<T, N, L> result{};
    for (size_t i = 0; i < N; i++) {
      for (size_t j = 0; j < L; j++) {
        for (size_t k = 0; k < M; k++) {
          result.matrix[i][j] += matrix[i][k] * other.matrix[k][j];
        }
      }
    }
    return std::move(result);
  }

  template <size_t L>
  Matrix<T, N, L>& operator*=(const Matrix<T, M, L>& other) {
    return *this = *this * other;
  }

  Matrix<T, N, M>& operator*=(const T& k) {
    for (size_t i = 0; i < N; i++) {
      for (size_t j = 0; j < M; j++) {
        matrix[i][j] *= k;
      }
    }
    return *this;
  }

  Matrix<T, N, M> operator*(const T& k) const {
    auto result = *this;
    return result *= k;
  }

  Matrix<T, N, M>& operator/=(const T& k) {
    for (size_t i = 0; i < N; i++) {
      for (size_t j = 0; j < M; j++) {
        matrix[i][j] /= k;
      }
    }
    return *this;
  }

  Matrix<T, N, M> operator/(const T& k) const {
    auto result = *this;
    return result /= k;
  }
};

template <class T, size_t N, size_t M>
Matrix<T, M, N> GetTransposed(const Matrix<T, N, M>& matrix) {
  Matrix<T, M, N> result;
  for (size_t i = 0; i < N; i++) {
    for (size_t j = 0; j < M; j++) {
      result(j, i) = matrix(i, j);
    }
  }
  return std::move(result);
}

template <class K, class T, size_t N, size_t M>
Matrix<T, N, M> operator*(const K& k, const Matrix<T, N, M>& matrix) {
  return matrix * k;
}

template <class T, size_t N, size_t M>
bool operator==(const Matrix<T, N, M>& a, const Matrix<T, N, M>& b) {
  for (size_t i = 0; i < N; i++) {
    for (size_t j = 0; j < M; j++) {
      if (a(i, j) != b(i, j)) {
        return false;
      }
    }
  }
  return true;
}

template <class T, size_t N, size_t M>
bool operator!=(const Matrix<T, N, M>& a, const Matrix<T, N, M>& b) {
  return !(a == b);
}

template <class T, size_t N, size_t M>
std::istream& operator>>(std::istream& in, Matrix<T, N, M>& matrix) {
  for (size_t i = 0; i < N; i++) {
    for (size_t j = 0; j < M; j++) {
      in >> matrix(i, j);
    }
  }
  return in;
}

template <class T, size_t N, size_t M>
std::ostream& operator<<(std::ostream& out, const Matrix<T, N, M>& matrix) {
  for (size_t i = 0; i < N; i++) {
    out << matrix(i, 0);
    for (size_t j = 1; j < M; j++) {
      out << ' ' << matrix(i, j);
    }
    out << std::endl;
  }
  return out;
}

#define MATRIX_SQUARE_MATRIX_IMPLEMENTED

template <class T, size_t N>
void Transpose(Matrix<T, N, N>& matrix) {
  for (size_t i = 0; i < N; i++) {
    for (size_t j = i + 1; j < N; j++) {
      std::swap(matrix(i, j), matrix(j, i));
    }
  }
}

template <class T, size_t N>
T Trace(const Matrix<T, N, N>& matrix) {
  T result{};
  for (size_t i = 0; i < N; i++) {
    result += matrix(i, i);
  }
  return std::move(result);
}

template <class T, size_t N>
Matrix<T, N - 1, N - 1> Minor(const Matrix<T, N, N>& matrix, size_t row_number, size_t column_number) {
  Matrix<T, N - 1, N - 1> result;
  for (size_t i = 0; i < N - 1; i++) {
    for (size_t j = 0; j < N - 1; j++) {
      result(i, j) = matrix(i < row_number ? i : i + 1, j < column_number ? j : j + 1);
    }
  }
  return std::move(result);
}

template <class T>
T Determinant(const Matrix<T, 1, 1>& matrix) {
  return std::move(matrix(0, 0));
}

template <class T, size_t N>
T Determinant(const Matrix<T, N, N>& matrix) {
  T result{};
  for (size_t k = 0; k < N; k++) {
    result += Determinant(Minor(matrix, k, 0)) * matrix(k, 0) * (k % 2 == 0 ? 1 : -1);
  }
  return std::move(result);
}

template <class T>
Matrix<T, 1, 1> GetInversed(const Matrix<T, 1, 1>& matrix) {
  T determinant = Determinant(matrix);
  if (determinant == 0) {
    throw MatrixIsDegenerateError{};
  }
  return {1 / determinant};
}

template <class T, size_t N>
Matrix<T, N, N> GetInversed(const Matrix<T, N, N>& matrix) {
  T determinant = Determinant(matrix);
  if (determinant == 0) {
    throw MatrixIsDegenerateError{};
  }
  Matrix<T, N, N> result;
  for (size_t i = 0; i < N; i++) {
    for (size_t j = 0; j < N; j++) {
      result(i, j) = Determinant(Minor(matrix, j, i)) / determinant * ((i + j) % 2 == 0 ? 1 : -1);
    }
  }
  return std::move(result);
}

template <class T, size_t N>
void Inverse(Matrix<T, N, N>& matrix) {
  matrix = GetInversed(matrix);
}

#endif
