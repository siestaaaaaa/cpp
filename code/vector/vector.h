#pragma once
#include <cassert>
#include <cstring>
#include <iostream>

namespace lx {
template <typename T>
class vector {
  T* start_{};
  T* finish_{};
  T* end_of_storage_{};

 public:
  using iterator = T*;
  using const_iterator = const T*;

  vector() = default;

  vector(size_t n, const T& val = T{}) {
    reserve(n);
    while (n--) {
      push_back(val);
    }
  }

  ~vector() {
    delete[] start_;
    start_ = finish_ = end_of_storage_ = nullptr;
  }

  vector(const vector& v) {
    reserve(v.capacity());

    for (auto& x : v) {
      push_back(x);
    }
  }

  void swap(vector& v) {
    std::swap(start_, v.start_);
    std::swap(finish_, v.finish_);
    std::swap(end_of_storage_, v.end_of_storage_);
  }

  vector<T>& operator=(const vector& other) {
    if (this != &other) {
      vector<T>{other}.swap(*this);
    }
    return *this;
  }

  void reserve(size_t n) {
    if (n > capacity()) {
      T* tmp = new T[n]{};
      if (start_) {
        memcpy(tmp, start_, sizeof(T) * size());
        delete[] start_;
      }
      finish_ = tmp + size();
      start_ = tmp;
      end_of_storage_ = start_ + n;
    }
  }

  void resize(size_t n, const T& val = T{}) {
    if (n <= size()) {
      finish_ = start_ + n;
    } else {
      reserve(n);
      while (finish_ < start_ + n) {
        *finish_++ = val;
      }
    }
  }

  void push_back(const T& x) { insert(end(), x); }

  void pop_back() { erase(end() - 1); }

  iterator insert(iterator pos, const T& x) {
    assert(pos >= start_ && pos <= finish_);

    if (finish_ == end_of_storage_) {
      size_t cp = capacity() == 0 ? 4 : capacity() * 2;
      size_t dist = pos - start_;
      reserve(cp);
      pos = start_ + dist;
    }

    iterator end = finish_;
    while (end > pos) {
      *end = *(end - 1);
      --end;
    }
    *pos = x;
    ++finish_;

    return pos;
  }

  iterator erase(iterator pos) {
    assert(pos >= start_ && pos < finish_);

    iterator it = pos + 1;
    while (it < finish_) {
      *(it - 1) = *it;
      ++it;
    }
    --finish_;

    return pos;
  }

  T& operator[](size_t pos) {
    assert(pos < size());

    return start_[pos];
  }

  const T& operator[](size_t pos) const {
    assert(pos < size());

    return start_[pos];
  }

  size_t capacity() const { return end_of_storage_ - start_; }

  size_t size() const { return finish_ - start_; }

  iterator begin() { return start_; }

  iterator end() { return finish_; }

  const_iterator begin() const { return start_; }

  const_iterator end() const { return finish_; }
};

void test() {
  vector<int> v;
  v.push_back(1);
  v.push_back(2);
  v.push_back(3);
  v.push_back(4);
  v.push_back(5);

  for (auto x : v) {
    std::cout << x << '\n';
  }
}
}  // namespace lx
