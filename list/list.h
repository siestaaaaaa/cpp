#pragma once
#include <algorithm>
#include <iostream>
#include <list>

#include "reverse_iterator.h"

namespace lx {
template <class T>
struct node {
  T data_;
  node<T>* prev_;
  node<T>* next_;

  node(const T& x = T()) : data_(x), prev_(nullptr), next_(nullptr) {}
};

template <class T, class Ref, class Ptr>
struct Iterator {
  typedef node<T> node_t;
  typedef Iterator<T, Ref, Ptr> self;

  node_t* node_;

  Iterator(node_t* node = nullptr) : node_(node) {}
  Iterator(const self& it) : node_(it.node_) {}

  self& operator++() {
    node_ = node_->next_;
    return *this;
  }

  self operator++(int) {
    auto tmp(*this);
    node_ = node_->next_;
    return tmp;
  }

  self& operator--() {
    node_ = node_->prev_;
    return *this;
  }

  self operator--(int) {
    auto tmp(*this);
    node_ = node_->prev_;
    return tmp;
  }

  Ref operator*() { return node_->data_; }

  Ptr operator->() { return &node_->data_; }

  bool operator!=(const self& it) { return node_ != it.node_; }

  bool operator==(const self& it) { return !(node_ != it.node_); }
};
//迭代器是指针的抽象
//给不同的数据结构提供了统一的访问方式

template <class T>
class list {
  typedef node<T> node_t;
  typedef Iterator<T, T&, T*> iterator;
  typedef Iterator<T, const T&, const T*> const_iterator;
  typedef ReverseIterator<iterator, T&, T*> reverse_iterator;
  typedef ReverseIterator<const_iterator, const T&, const T*>
      const_reverse_iterator;

  node_t* dummy_;
  size_t size_;

 public:
  reverse_iterator rbegin() { return reverse_iterator(--end()); }

  reverse_iterator rend() { return reverse_iterator(end()); }

  const_reverse_iterator rbegin() const {
    return const_reverse_iterator(--end());
  }

  const_reverse_iterator rend() const { return const_reverse_iterator(end()); }

  iterator begin() { return iterator(dummy_->next_); }

  iterator end() { return iterator(dummy_); }

  const_iterator begin() const { return const_iterator(dummy_->next_); }

  const_iterator end() const { return const_iterator(dummy_); }

  void init() {
    dummy_ = new node_t;
    dummy_->next_ = dummy_;
    dummy_->prev_ = dummy_;
    size_ = 0;
  }

  list() { init(); }

  list(const list<T>& l) {
    init();
    for (auto x : l) {
      push_back(x);
    }
  }

  void swap(list<T>& l) {
    std::swap(dummy_, l.dummy_);
    std::swap(size_, l.size_);
  }

  list<T>& operator=(list<T> l) {
    swap(l);

    return *this;
  }

  void clear() {
    for (auto it = begin(); it != end(); it = erase(it))
      ;
  }

  ~list() {
    clear();

    delete dummy_;
    dummy_ = nullptr;
  }

  iterator insert(iterator pos, const T& x)  // insert before pos
  {
    auto cur = pos.node_, prev = cur->prev_;

    auto newnode = new node_t(x);
    newnode->prev_ = prev;
    newnode->next_ = cur;
    cur->prev_ = newnode;
    prev->next_ = newnode;

    ++size_;

    return iterator(newnode);
  }

  void push_back(const T& x) { insert(end(), x); }

  void push_front(const T& x) { insert(begin(), x); }

  iterator erase(iterator pos)  // erase pos and return next
  {
    auto cur = pos.node_, prev = cur->prev_, next = cur->next_;

    prev->next_ = next;
    next->prev_ = prev;
    delete cur;

    --size_;

    return iterator(next);
  }

  void pop_back() { erase(--end()); }

  void pop_front() { erase(begin()); }

  size_t size() const { return size_; }

  bool empty() const { return size_ == 0; }

  T& front() { return dummy_->next_->data_; }

  const T& front() const { return dummy_->next_->data_; }

  T& back() { return dummy_->prev_->data_; }

  const T& back() const { return dummy_->prev_->data_; }
};

void test1() {
  list<int> l1;
  l1.push_back(1);
  l1.push_back(2);
  l1.push_back(3);
  l1.push_back(4);
  l1.push_back(5);
  for (auto x : l1) std::cout << x << ' ';
  std::cout << '\n';
}

void test2() {
  list<int> l1;
  l1.push_back(1);
  l1.push_back(2);
  l1.push_back(3);
  l1.push_back(4);
  l1.push_back(5);

  for (auto rit = l1.rbegin(); rit != l1.rend(); ++rit)
    std::cout << *rit << ' ';
  std::cout << '\n';
}
}  // namespace lx