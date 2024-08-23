#pragma once
#include <iostream>
#include <deque>

namespace lx {
template <typename T, typename Container = std::deque<T>>
class queue {
  Container con_;

 public:
  void push(const T& x) { con_.push_back(x); }

  void pop() { con_.pop_front(); }

  T& back() { return con_.back(); }

  const T& back() const { return con_.back(); }

  T& front() { return con_.front(); }

  const T& front() const { return con_.front(); }

  size_t size() const { return con_.size(); }

  bool empty() const { return con_.empty(); }
};

void test() {
  queue<int> q;
  q.push(1);
  q.push(2);
  q.push(3);
  q.push(4);
  q.push(5);

  while (!q.empty()) {
    std::cout << q.front() << ' ';
    q.pop();
  }
  std::cout << '\n';
}
}  // namespace lx