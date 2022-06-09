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
  T buffer[N];

  T& operator[](size_t index) {
    return buffer[index];
  }

  const T& operator[](size_t index) const {
    return buffer[index];
  }

  T& At(size_t index) {
    if (index < 0 || index >= N) {
      throw ArrayOutOfRange{};
    }
    return buffer[index];
  }

  const T& At(size_t index) const {
    if (index < 0 || index >= N) {
      throw ArrayOutOfRange{};
    }
    return buffer[index];
  }

  T& Front() {
    if (N == 0) {
      throw ArrayOutOfRange{};
    }
    return buffer[0];
  }

  const T& Front() const {
    if (N == 0) {
      throw ArrayOutOfRange{};
    }
    return buffer[0];
  }

  T& Back() {
    if (N == 0) {
      throw ArrayOutOfRange{};
    }
    return buffer[N - 1];
  }

  const T& Back() const {
    if (N == 0) {
      throw ArrayOutOfRange{};
    }
    return buffer[N - 1];
  }

  const T* Data() const {
    return buffer;
  }

  size_t Size() const {
    return N;
  }

  bool Empty() const {
    return N == 0;
  }

  void Fill(const T& value) {
    for (size_t i = 0; i < N; i++) {
      buffer[i] = value;
    }
  }

  void Swap(Array<T, N>& other) {
    std::swap(buffer, other.buffer);
  }
};

#define ARRAY_TRAITS_IMPLEMENTED

template <class T>
size_t GetSize(const T&) {
  return 0;
}

template <class T, size_t N>
size_t GetSize(const T (&)[N]) {
  return N;
}

template <class T>
size_t GetRank(const T&) {
  return 0;
}

template <class T, size_t N>
size_t GetRank(const T (&array)[N]) {
  return GetRank(array[0]) + 1;
}

template <class T>
size_t GetNumElements(const T&) {
  return 1;
}

template <class T, size_t N>
size_t GetNumElements(const T (&array)[N]) {
  return GetNumElements(array[0]) * N;
}

#endif
