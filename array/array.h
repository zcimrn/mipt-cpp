#ifndef ARRAY_H_
#define ARRAY_H_

#include <stdexcept>

class ArrayOutOfRange : public std::out_of_range {
 public:
  ArrayOutOfRange() : std::out_of_range("ArrayOutOfRange") {
  }
};

template <class T, size_t N>
class Array {
 public:
  T array[N];

  T& operator[](size_t index) {
    return array[index];
  }

  const T& operator[](size_t index) const {
    return array[index];
  }

  T& At(size_t index) {
    if (index < 0 || index >= N) {
      throw ArrayOutOfRange{};
    }
    return array[index];
  }

  const T& At(size_t index) const {
    if (index < 0 || index >= N) {
      throw ArrayOutOfRange{};
    }
    return array[index];
  }

  T& Front() {
    if (N == 0) {
      throw ArrayOutOfRange{};
    }
    return array[0];
  }

  const T& Front() const {
    if (N == 0) {
      throw ArrayOutOfRange{};
    }
    return array[0];
  }

  T& Back() {
    if (N == 0) {
      throw ArrayOutOfRange{};
    }
    return array[N - 1];
  }

  const T& Back() const {
    if (N == 0) {
      throw ArrayOutOfRange{};
    }
    return array[N - 1];
  }

  const T* Data() const {
    return array;
  }

  size_t Size() const {
    return N;
  }

  bool Empty() const {
    return N == 0;
  }

  void Fill(const T& value) {
    for (size_t i = 0; i < N; i++) {
      array[i] = value;
    }
  }

  void Swap(Array<T, N>& other) {
    std::swap(array, other.array);
  }
};

#endif
