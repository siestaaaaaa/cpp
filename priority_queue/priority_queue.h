#pragma once
#include <iostream>
#include <vector>

namespace lx {
template <typename T>
struct Less {
  bool operator()(const T& x, const T& y) const { return x < y; }
};

template <typename T>
struct Greater {
  bool operator()(const T& x, const T& y) const { return x > y; }
};

template <typename T, typename Container = std::vector<T>, typename Compare = Less<T>>
class PriorityQueue {
  Container con_;

  void up(int index) {
    Compare cmp;

    int parent = (index - 1) / 2;
    while (index >= 0) {
      if (cmp(con_[parent], con_[index])) {
        std::swap(con_[index], con_[parent]);
        index = parent;
        parent = (index - 1) / 2;
      } else {
        break;
      }
    }
  }

  void down(int index) {
    Compare cmp;

    int child = index * 2 + 1;
    int n = con_.size();
    while (child < n) {
      if (child + 1 < n && cmp(con_[child], con_[child + 1])) {
        ++child;
      }

      if (cmp(con_[index], con_[child])) {
        std::swap(con_[child], con_[index]);
        index = child;
        child = index * 2 + 1;
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
  PriorityQueue<int> pq;
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
  PriorityQueue<int, std::vector<int>, Greater<int>> pq;
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