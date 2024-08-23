#pragma once
#include <cassert>
#include <cstring>
#include <iostream>

namespace lx {
class string {
  char* str_ = nullptr;
  size_t size_ = 0;
  size_t capacity_ = 0;

 public:
  typedef char* iterator;
  typedef const char* const_iterator;

  string(const char* str = "") {
    if (str != nullptr) {
      size_ = strlen(str);
      capacity_ = size_;
      str_ = new char[capacity_ + 1]{};
      strcpy(str_, str);
    }
  }

  void swap(string& s) {
    std::swap(size_, s.size_);
    std::swap(capacity_, s.capacity_);
    std::swap(str_, s.str_);
  }

  string(const string& s) {
    string tmp(s.str_);
    swap(tmp);
  }

  string(string&& s) { swap(s); }

  string& operator=(string tmp) {
    swap(tmp);
    return *this;
  }

  ~string() {
    delete[] str_;
    str_ = nullptr;
    size_ = capacity_ = 0;
  }

  void push_back(char ch) {
    if (size_ == capacity_) reserve(capacity_ == 0 ? 4 : capacity_ * 2);

    str_[size_++] = ch;
  }

  void append(const char* str) {
    size_t len = strlen(str);
    if (size_ + len > capacity_) reserve(size_ + len);

    strcpy(str_ + size_, str);
    size_ += len;
  }

  string& operator+=(char ch) {
    push_back(ch);
    return *this;
  }

  string& operator+=(const char* str) {
    append(str);
    return *this;
  }

  string& operator+=(string s) {
    append(s.str_);
    size_ += s.size_;
    return *this;
  }

  void clear() {
    size_ = 0;
    str_[size_] = '\0';
  }

  const char* c_str() const { return str_; }

  size_t capacity() const { return capacity_; }

  size_t size() const { return size_; }

  bool empty() const { return size_ == 0; }

  void reserve(size_t n) {
    if (n > capacity_) {
      char* tmp = new char[n + 1]{};
      strcpy(tmp, str_);
      delete[] str_;
      str_ = tmp;
      capacity_ = n;
    }
  }

  void resize(size_t n, char ch = '\0') {
    if (n <= size_)
      size_ = n;
    else {
      reserve(n);
      while (size_ < n) str_[size_++] = ch;
    }
    str_[n] = '\0';
  }

  char& operator[](size_t pos) {
    assert(pos < size_);

    return str_[pos];
  }

  const char& operator[](size_t pos) const {
    assert(pos < size_);

    return str_[pos];
  }

  bool operator<(const string& s) const { return strcmp(str_, s.str_) < 0; }

  bool operator==(const string& s) const { return strcmp(str_, s.str_) == 0; }

  bool operator>(const string& s) const { return strcmp(str_, s.str_) > 0; }

  bool operator<=(const string& s) const { return !(*this > s); }

  bool operator>=(const string& s) const { return !(*this < s); }

  bool operator!=(const string& s) const { return !(*this == s); }

  size_t find(char ch, size_t pos = 0) const {
    for (size_t i = pos; i < size_; i++)
      if (str_[i] == ch) return i;
    return -1;
  }

  size_t find(const char* str, size_t pos = 0) const {
    const char* p = strstr(str_ + pos, str);

    if (p != nullptr) return p - str_;
    return -1;
  }

  string substr(size_t pos, size_t len = -1) const {
    string res;
    if (len == -1 || pos + len >= size_) {
      len = size_ - pos;
      res.reserve(len);
      for (size_t i = pos; i < size_; i++) res += str_[i];
    } else {
      res.reserve(len);
      for (size_t i = pos; i < pos + len; i++) res += str_[i];
    }
    return res;
  }

  void insert(size_t pos, char ch) {
    assert(pos >= 0 && pos <= size_);
    if (size_ == capacity_) reserve(capacity_ == 0 ? 4 : capacity_ * 2);

    size_t end = size_;
    while (end != pos) {
      str_[end] = str_[end - 1];
      --end;
    }
    str_[pos] = ch;
    size_++;
  }

  void insert(size_t pos, const char* s) {
    assert(pos >= 0 && pos <= size_);

    size_t len = strlen(s);
    if (size_ + len > capacity_) reserve(size_ + len);

    size_t end = size_;
    while (end != pos) {
      str_[end + len - 1] = str_[end - 1];
      --end;
    }
    for (size_t i = 0; i < len; i++) str_[pos + i] = s[i];
    size_ += len;
  }

  void erase(size_t pos, size_t len = -1) {
    assert(pos >= 0 && pos < size_);
    if (len == -1 || pos + len >= size_)
      size_ = pos;
    else {
      for (size_t i = 0; i < size_ - len; i++)
        if (pos + len + i < size_)
          str_[pos + i] = str_[pos + len + i];
        else
          break;

      size_ -= len;
    }

    str_[size_] = '\0';
  }

  iterator begin() { return str_; }

  iterator end() { return str_ + size_; }

  const_iterator begin() const { return str_; }

  const_iterator end() const { return str_ + size_; }
};

std::ostream& operator<<(std::ostream& out, const string& s) {
  for (const auto ch : s) out << ch;
  return out;
}

std::istream& operator>>(std::istream& in, string& s) {
  s.clear();

  char ch = in.get();
  while (ch != ' ' && ch != '\n') {
    s += ch;
    ch = in.get();
  }

  return in;
}

void test() {
  string s;
  s += '1';
  s += "2345";
  s += string(s);
  std::cout << s << '\n';
}
}  // namespace lx