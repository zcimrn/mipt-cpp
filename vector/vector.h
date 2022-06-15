#ifndef VECTOR_H_
#define VECTOR_H_

#define VECTOR_MEMORY_IMPLEMENTED

#include <iterator>
#include <memory>

template <class T>
class Vector {
 public:
  using ValueType = T;
  using Pointer = T*;
  using ConstPointer = const T*;
  using Reference = T&;
  using ConstReference = const T&;
  using SizeType = size_t;
  using Iterator = Pointer;
  using ConstIterator = ConstPointer;
  using ReverseIterator = std::reverse_iterator<Vector<ValueType>::Iterator>;
  using ConstReverseIterator = std::reverse_iterator<Vector<ValueType>::ConstIterator>;

  Vector() = default;

  Vector(const Vector<ValueType>& other) {
    if (other.buffer_ == nullptr) {
      return;
    }
    auto new_buffer = static_cast<Pointer>(operator new[](other.capacity_ * sizeof(ValueType)));
    try {
      std::uninitialized_copy_n(other.buffer_, other.size_, new_buffer);
    } catch (...) {
      operator delete[](new_buffer);
      throw;
    }
    buffer_ = new_buffer;
    size_ = other.size_;
    capacity_ = other.capacity_;
  }

  Vector(Vector<ValueType>&& other) : buffer_{other.buffer_}, size_{other.size_}, capacity_{other.capacity_} {
    other.buffer_ = nullptr;
    other.size_ = 0;
    other.capacity_ = 0;
  }

  Vector& operator=(const Vector<ValueType>& other) {
    if (this == &other) {
      return *this;
    }
    if (other.buffer_ == nullptr) {
      if (buffer_ != nullptr) {
        std::destroy_n(buffer_, size_);
        operator delete[](buffer_);
        buffer_ = nullptr;
        size_ = 0;
        capacity_ = 0;
      }
      return *this;
    }
    auto new_buffer = static_cast<Pointer>(operator new[](other.capacity_ * sizeof(ValueType)));
    try {
      std::uninitialized_copy_n(other.buffer_, other.size_, new_buffer);
    } catch (...) {
      operator delete[](new_buffer);
      throw;
    }
    if (buffer_ != nullptr) {
      std::destroy_n(buffer_, size_);
      operator delete[](buffer_);
    }
    buffer_ = new_buffer;
    size_ = other.size_;
    capacity_ = other.capacity_;
    return *this;
  }

  Vector& operator=(Vector<ValueType>&& other) noexcept {
    if (this == &other) {
      return *this;
    }
    if (buffer_ != nullptr) {
      std::destroy_n(buffer_, size_);
      operator delete[](buffer_);
    }
    buffer_ = other.buffer_;
    size_ = other.size_;
    capacity_ = other.capacity_;
    other.buffer_ = nullptr;
    other.size_ = 0;
    other.capacity_ = 0;
    return *this;
  }

  ~Vector() {
    if (buffer_ != nullptr) {
      std::destroy_n(buffer_, size_);
      operator delete[](buffer_);
    }
    buffer_ = nullptr;
    size_ = 0;
    capacity_ = 0;
  }

  explicit Vector(SizeType new_size) {
    if (new_size > 0) {
      auto new_buffer = static_cast<Pointer>(operator new[](new_size * sizeof(ValueType)));
      try {
        std::uninitialized_default_construct_n(new_buffer, new_size);
      } catch (...) {
        operator delete[](new_buffer);
        throw;
      }
      buffer_ = new_buffer;
      size_ = new_size;
      capacity_ = new_size;
    }
  }

  explicit Vector(SizeType new_size, const ValueType& value) {
    if (new_size > 0) {
      auto new_buffer = static_cast<Pointer>(operator new[](new_size * sizeof(ValueType)));
      try {
        std::uninitialized_fill_n(new_buffer, new_size, value);
      } catch (...) {
        operator delete[](new_buffer);
        throw;
      }
      buffer_ = new_buffer;
      size_ = new_size;
      capacity_ = new_size;
    }
  }

  template <class Iterator, class = std::enable_if_t<std::is_base_of_v<
                                std::forward_iterator_tag, typename std::iterator_traits<Iterator>::iterator_category>>>
  explicit Vector(Iterator begin, Iterator end) {
    SizeType size = end - begin;
    if (size == 0) {
      return;
    }
    auto new_buffer = static_cast<Pointer>(operator new[](size * sizeof(ValueType)));
    try {
      std::uninitialized_move_n(begin, size, new_buffer);
    } catch (...) {
      operator delete[](new_buffer);
      throw;
    }
    buffer_ = new_buffer;
    size_ = size;
    capacity_ = size;
  }

  Vector(const std::initializer_list<ValueType>& list) : Vector(list.begin(), list.end()) {
  }

  SizeType Size() const {
    return size_;
  }

  SizeType Capacity() const {
    return capacity_;
  }

  bool Empty() const {
    return size_ == 0;
  }

  Reference operator[](SizeType index) {
    return buffer_[index];
  }

  ConstReference operator[](SizeType index) const {
    return buffer_[index];
  }

  Reference At(SizeType index) {
    if (index < size_) {
      return buffer_[index];
    }
    throw std::out_of_range("Reference At(SizeType)");
  }

  ConstReference At(SizeType index) const {
    if (index < size_) {
      return buffer_[index];
    }
    throw std::out_of_range("ConstReference At(SizeType)");
  }

  Reference Front() {
    if (size_ > 0) {
      return buffer_[0];
    }
    throw std::out_of_range("Reference Front()");
  }

  ConstReference Front() const {
    if (size_ > 0) {
      return buffer_[0];
    }
    throw std::out_of_range("ConstReference Front()");
  }

  Reference Back() {
    if (size_ > 0) {
      return buffer_[size_ - 1];
    }
    throw std::out_of_range("Reference Back()");
  }

  ConstReference Back() const {
    if (size_ > 0) {
      return buffer_[size_ - 1];
    }
    throw std::out_of_range("ConstReference Back()");
  }

  Pointer Data() {
    return buffer_;
  }

  ConstPointer Data() const {
    return buffer_;
  }

  void Swap(Vector<ValueType>& other) {
    std::swap(buffer_, other.buffer_);
    std::swap(size_, other.size_);
    std::swap(capacity_, other.capacity_);
  }

  void Resize(SizeType new_size) {
    if (new_size <= size_) {
      std::destroy_n(buffer_ + new_size, size_ - new_size);
      size_ = new_size;
      return;
    }
    if (new_size <= capacity_) {
      try {
        std::uninitialized_default_construct_n(buffer_ + size_, new_size - size_);
      } catch (...) {
        throw;
      }
      size_ = new_size;
      return;
    }
    auto new_buffer = static_cast<Pointer>(operator new[](new_size * sizeof(ValueType)));
    try {
      std::uninitialized_default_construct_n(new_buffer + size_, new_size - size_);
      std::uninitialized_move_n(buffer_, size_, new_buffer);
    } catch (...) {
      operator delete[](new_buffer);
      throw;
    }
    if (buffer_ != nullptr) {
      std::destroy_n(buffer_, size_);
      operator delete[](buffer_);
    }
    buffer_ = new_buffer;
    size_ = new_size;
    capacity_ = new_size;
  }

  void Resize(SizeType new_size, const ValueType& value) {
    if (new_size <= size_) {
      std::destroy_n(buffer_ + new_size, size_ - new_size);
      size_ = new_size;
      return;
    }
    if (new_size <= capacity_) {
      try {
        std::uninitialized_fill_n(buffer_ + size_, new_size - size_, value);
      } catch (...) {
        throw;
      }
      size_ = new_size;
      return;
    }
    auto new_buffer = static_cast<Pointer>(operator new[](new_size * sizeof(ValueType)));
    try {
      std::uninitialized_fill_n(new_buffer + size_, new_size - size_, value);
      std::uninitialized_move_n(buffer_, size_, new_buffer);
    } catch (...) {
      operator delete[](new_buffer);
      throw;
    }
    if (buffer_ != nullptr) {
      std::destroy_n(buffer_, size_);
      operator delete[](buffer_);
    }
    buffer_ = new_buffer;
    size_ = new_size;
    capacity_ = new_size;
  }

  void Reserve(SizeType new_capacity) {
    if (new_capacity <= capacity_) {
      return;
    }
    auto new_buffer = static_cast<Pointer>(operator new[](new_capacity * sizeof(ValueType)));
    if (buffer_ == nullptr) {
      buffer_ = new_buffer;
      capacity_ = new_capacity;
      return;
    }
    try {
      std::uninitialized_move_n(buffer_, size_, new_buffer);
    } catch (...) {
      operator delete[](new_buffer);
      throw;
    }
    std::destroy_n(buffer_, size_);
    operator delete[](buffer_);
    buffer_ = new_buffer;
    capacity_ = new_capacity;
  }

  void ShrinkToFit() {
    if (size_ == capacity_ || buffer_ == nullptr) {
      return;
    }
    if (size_ == 0) {
      operator delete[](buffer_);
      buffer_ = nullptr;
      capacity_ = size_;
      return;
    }
    auto new_buffer = static_cast<Pointer>(operator new[](size_ * sizeof(ValueType)));
    try {
      std::uninitialized_move_n(buffer_, size_, new_buffer);
    } catch (...) {
      operator delete[](new_buffer);
      throw;
    }
    std::destroy_n(buffer_, size_);
    operator delete[](buffer_);
    buffer_ = new_buffer;
    capacity_ = size_;
  }

  void Clear() {
    if (size_ > 0) {
      std::destroy_n(buffer_, size_);
      size_ = 0;
    }
  }

  void PushBack(const ValueType& value) {
    if (size_ < capacity_) {
      new (buffer_ + size_) ValueType(value);
      size_++;
      return;
    }
    SizeType new_capacity = (capacity_ == 0 ? 1 : capacity_ * 2);
    auto new_buffer = static_cast<Pointer>(operator new[](new_capacity * sizeof(ValueType)));
    try {
      new (new_buffer + size_) ValueType(value);
      std::uninitialized_move_n(buffer_, size_, new_buffer);
    } catch (...) {
      operator delete[](new_buffer);
      throw;
    }
    if (buffer_ != nullptr) {
      std::destroy_n(buffer_, size_);
      operator delete[](buffer_);
    }
    buffer_ = new_buffer;
    size_++;
    capacity_ = new_capacity;
  }

  void PushBack(ValueType&& value) {
    if (size_ < capacity_) {
      new (buffer_ + size_) ValueType(std::move(value));
      size_++;
      return;
    }
    SizeType new_capacity = (capacity_ == 0 ? 1 : capacity_ * 2);
    auto new_buffer = static_cast<Pointer>(operator new[](new_capacity * sizeof(ValueType)));
    try {
      new (new_buffer + size_) ValueType(std::move(value));
      std::uninitialized_move_n(buffer_, size_, new_buffer);
    } catch (...) {
      operator delete[](new_buffer);
      throw;
    }
    if (buffer_ != nullptr) {
      std::destroy_n(buffer_, size_);
      operator delete[](buffer_);
    }
    buffer_ = new_buffer;
    size_++;
    capacity_ = new_capacity;
  }

  void PopBack() {
    if (size_ == 0) {
      throw std::out_of_range("PopBack()");
    }
    size_--;
    std::destroy_at(buffer_ + size_);
  }

  template <class... Args>
  void EmplaceBack(Args&&... args) {
    PushBack(ValueType(std::forward<Args>(args)...));
  }

  Iterator begin() {  // NOLINT
    return buffer_;
  }

  ConstIterator begin() const {  // NOLINT
    return buffer_;
  }

  ConstIterator cbegin() const {  // NOLINT
    return buffer_;
  }

  Iterator end() {  // NOLINT
    return buffer_ + size_;
  }

  ConstIterator end() const {  // NOLINT
    return buffer_ + size_;
  }

  ConstIterator cend() const {  // NOLINT
    return buffer_ + size_;
  }

  ReverseIterator rbegin() {  // NOLINT
    return ReverseIterator(buffer_ + size_);
  }

  ConstReverseIterator rbegin() const {  // NOLINT
    return ConstReverseIterator(buffer_ + size_);
  }

  ConstReverseIterator crbegin() const {  // NOLINT
    return ConstReverseIterator(buffer_ + size_);
  }

  ReverseIterator rend() {  // NOLINT
    return ReverseIterator(buffer_);
  }

  ConstReverseIterator rend() const {  // NOLINT
    return ConstReverseIterator(buffer_);
  }

  ConstReverseIterator crend() const {  // NOLINT
    return ConstReverseIterator(buffer_);
  }

 private:
  Pointer buffer_{nullptr};
  SizeType size_{0}, capacity_{0};
};

template <class ValueType>
int8_t Compare(const Vector<ValueType>& left, const Vector<ValueType>& right) {
  for (size_t i = 0, size = std::min(left.Size(), right.Size()); i < size; i++) {
    if (left[i] < right[i]) {
      return -1;
    }
    if (left[i] > right[i]) {
      return 1;
    }
  }
  if (left.Size() < right.Size()) {
    return -1;
  }
  if (left.Size() > right.Size()) {
    return 1;
  }
  return 0;
}

template <class ValueType>
bool operator<(const Vector<ValueType>& left, const Vector<ValueType>& right) {
  return Compare(left, right) < 0;
}

template <class ValueType>
bool operator>(const Vector<ValueType>& left, const Vector<ValueType>& right) {
  return Compare(left, right) > 0;
}

template <class ValueType>
bool operator<=(const Vector<ValueType>& left, const Vector<ValueType>& right) {
  return Compare(left, right) <= 0;
}

template <class ValueType>
bool operator>=(const Vector<ValueType>& left, const Vector<ValueType>& right) {
  return Compare(left, right) >= 0;
}

template <class ValueType>
bool operator==(const Vector<ValueType>& left, const Vector<ValueType>& right) {
  return Compare(left, right) == 0;
}

template <class ValueType>
bool operator!=(const Vector<ValueType>& left, const Vector<ValueType>& right) {
  return Compare(left, right) != 0;
}

#endif
