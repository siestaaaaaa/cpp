#pragma once
#include <iostream>
#include <deque>

namespace lx {
template <typename T, typename Container = std::deque<T>>
class stack {
  Container con_;

 public:
  void push(const T& x) { con_.push_back(x); }

  void pop() { con_.pop_back(); }

  T& top() { return con_.back(); }

  const T& top() const { return con_.back(); }

  bool empty() const { return con_.empty(); }

  size_t size() const { return con_.size(); }
};

void test() {
  stack<int> stk;
  stk.push(1);
  stk.push(2);
  stk.push(3);
  stk.push(4);
  stk.push(5);

  while (!stk.empty()) {
    std::cout << stk.top() << ' ';
    stk.pop();
  }
  std::cout << '\n';
}
}  // namespace lx