#ifndef RANGE_H_
#define RANGE_H_

#define REVERSE_RANGE_IMPLEMENTED

class Range {
 public:
  class Iterator {
   public:
    explicit Iterator(int64_t start, int64_t stop, int64_t step, int64_t value)
        : start_{start}, stop_{stop}, step_{step}, value_{value} {
    }

    bool operator==(const Iterator& other) const {
      return start_ == other.start_ && stop_ == other.stop_ && step_ == other.step_ && value_ == other.value_;
    }

    bool operator!=(const Iterator& other) const {
      return !operator==(other);
    }

    Iterator& operator++() {
      if (value_ != stop_) {
        value_ += step_;
      }
      return *this;
    }

    Iterator operator++(int) {
      auto result = *this;
      operator++();
      return std::move(result);
    }

    int64_t operator*() const {
      return value_;
    }

   private:
    int64_t start_, stop_, step_, value_;
  };

  explicit Range(int64_t stop) {
    if (stop > 0) {
      stop_ = stop;
      step_ = 1;
    }
  }

  explicit Range(int64_t start, int64_t stop, int64_t step = 1) {
    if (step > 0 && start < stop || step < 0 && start > stop) {
      start_ = start;
      stop_ = start + (std::abs(stop - start) + std::abs(step) - 1) / std::abs(step) * step;
      step_ = step;
    }
  }

  Iterator begin() const {  // NOLINT
    return std::move(Iterator(start_, stop_, step_, start_));
  }

  Iterator end() const {  // NOLINT
    return std::move(Iterator(start_, stop_, step_, stop_));
  }

  Iterator rbegin() const {  // NOLINT
    return std::move(Iterator(stop_ - step_, start_ - step_, -step_, stop_ - step_));
  }

  Iterator rend() const {  // NOLINT
    return std::move(Iterator(stop_ - step_, start_ - step_, -step_, start_ - step_));
  }

 private:
  int64_t start_ = 0, stop_ = 0, step_ = 0;
};

#endif
