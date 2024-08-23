#pragma once
#include <iostream>
#include <vector>

namespace lx {
template <class T>
struct Less {
  bool operator()(const T& x, const T& y) { return x < y; }
};

template <class T>
struct Greater {
  bool operator()(const T& x, const T& y) { return x > y; }
};

template <class T, class Container = std::vector<T>, class Compare = Less<T>>
class priority_queue {
  Container con_;

  void up(int child) {
    Compare cmp;

    int parent = (child - 1) / 2;
    while (child >= 0) {
      if (cmp(con_[parent], con_[child])) {
        swap(con_[child], con_[parent]);
        child = parent;
        parent = (child - 1) / 2;
      } else {
        break;
      }
    }
  }

  void down(int parent) {
    Compare cmp;

    int child = parent * 2 + 1;
    int n = con_.size();
    while (child < n) {
      if (child + 1 < n && cmp(con_[child], con_[child + 1])) {
        ++child;
      }

      if (cmp(con_[parent], con_[child])) {
        swap(con_[child], con_[parent]);
        parent = child;
        child = parent * 2 + 1;
      } else {
        break;
      }
    }
  }

 public:
  void push(const T& x) {
    con_.push_back(x);

    up(con_.size() - 1);
  }

  void pop() {
    std::swap(con_.front(), con_.back());
    con_.pop_back();

    down(0);
  }

  const T& top() const { return con_[0]; }

  bool empty() const { return con_.empty(); }

  size_t size() const { return con_.size(); }
};

void test1() {
  priority_queue<int> pq;
  pq.push(1);
  pq.push(5);
  pq.push(4);
  pq.push(3);
  pq.push(8);

  while (!pq.empty()) {
    std::cout << pq.top() << ' ';
    pq.pop();
  }
  std::cout << '\n';
}

void test2() {
  priority_queue<int, std::vector<int>, Greater<int>> pq;
  pq.push(1);
  pq.push(5);
  pq.push(4);
  pq.push(3);
  pq.push(8);

  while (!pq.empty()) {
    std::cout << pq.top() << ' ';
    pq.pop();
  }
  std::cout << '\n';
}
}  // namespace lx