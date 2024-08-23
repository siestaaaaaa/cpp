#pragma once
#include <algorithm>
#include <iostream>

namespace lx {
template <typename T>
struct Node {
  T data_{};
  Node<T>* prev_{};
  Node<T>* next_{};

  Node() = default;
  Node(const T& x) : data_(x) {}
  Node(T&& x) : data_(std::move(x)) {}
};

//迭代器是指针的抽象
//给不同的数据结构提供了统一的访问方式
template <typename T, typename Ref, typename Ptr>
struct Iterator {
  using node = Node<T>;
  using self = Iterator<T, Ref, Ptr>;

  node* ptr_{};

  Iterator() = default;
  Iterator(node* ptr) : ptr_(ptr) {}

  self& operator++() {
    ptr_ = ptr_->next_;
    return *this;
  }

  self operator++(int) {
    auto tmp(*this);
    ptr_ = ptr_->next_;
    return tmp;
  }

  self& operator--() {
    ptr_ = ptr_->prev_;
    return *this;
  }

  self operator--(int) {
    auto tmp(*this);
    ptr_ = ptr_->prev_;
    return tmp;
  }

  Ref operator*() const { return ptr_->data_; }

  Ptr operator->() const { return &ptr_->data_; }

  friend bool operator!=(const self& lhs, const self& rhs) {
    return lhs.ptr_ != rhs.ptr_;
  }
};

template <typename Iterator, typename Ref, typename Ptr>
class ReverseIterator {
  using self = ReverseIterator<Iterator, Ref, Ptr>;

  Iterator it_;

 public:
  ReverseIterator() = default;
  ReverseIterator(Iterator it) : it_(it) {}

  self& operator++() {
    --it_;
    return *this;
  }

  self operator++(int) {
    auto tmp(*this);
    --it_;
    return tmp;
  }

  self& operator--() {
    ++it_;
    return *this;
  }

  self operator--(int) {
    auto tmp(*this);
    ++it_;
    return tmp;
  }

  Ref operator*() const { return *it_; }

  Ptr operator->() const { return it_.operator->(); }

  friend bool operator!=(const self& lhs, const self& rhs) {
    return lhs.it_ != rhs.it_;
  }
};

template <typename T>
class List {
  using node = Node<T>;

  node* dummy_;
  size_t size_{};

 public:
  using iterator = Iterator<T, T&, T*>;
  using const_iterator = Iterator<T, const T&, const T*>;

  using reverse_iterator = ReverseIterator<iterator, T&, T*>;
  using const_reverse_iterator =
      ReverseIterator<const_iterator, const T&, const T*>;

  iterator begin() { return iterator(dummy_->next_); }
  iterator end() { return iterator(dummy_); }
  const_iterator begin() const { return const_iterator(dummy_->next_); }
  const_iterator end() const { return const_iterator(dummy_); }

  reverse_iterator rbegin() { return reverse_iterator(--end()); }
  reverse_iterator rend() { return reverse_iterator(end()); }
  const_reverse_iterator rbegin() const {
    return const_reverse_iterator(--end());
  }
  const_reverse_iterator rend() const { return const_reverse_iterator(end()); }

  void init() {
    dummy_ = new node;
    dummy_->next_ = dummy_;
    dummy_->prev_ = dummy_;
  }

  List() { init(); }

  List(const List& other) {
    init();
    for (auto x : other) {
      push_back(x);
    }
  }

  void swap(List& other) {
    std::swap(dummy_, other.dummy_);
    std::swap(size_, other.size_);
  }

  List<T>& operator=(const List& other) {
    if (this != &other) {
      List<T>{other}.swap(*this);
    }
    return *this;
  }

  void clear() {
    for (auto it = begin(); it != end(); it = erase(it))
      ;
  }

  ~List() {
    clear();

    delete dummy_;
    dummy_ = nullptr;
  }

  // insert before pos
  iterator insert(iterator pos, const T& x) {
    auto newnode = new node(x);
    auto cur = pos.ptr_, prev = cur->prev_;

    newnode->prev_ = prev;
    newnode->next_ = cur;
    cur->prev_ = newnode;
    prev->next_ = newnode;
    ++size_;

    return iterator(newnode);
  }

  void push_back(const T& x) { insert(end(), x); }

  void push_front(const T& x) { insert(begin(), x); }

  // erase at pos and return next
  iterator erase(iterator pos) {
    auto cur = pos.ptr_, prev = cur->prev_, next = cur->next_;

    prev->next_ = next;
    next->prev_ = prev;
    --size_;

    delete cur;
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
  List<int> l1;
  l1.push_back(1);
  l1.push_back(2);
  l1.push_back(3);
  l1.push_back(4);
  l1.push_back(5);
  for (auto x : l1) {
      std::cout << x << ' ';
  }
  std::cout << '\n';
}

void test2() {
  List<int> l1;
  l1.push_back(1);
  l1.push_back(2);
  l1.push_back(3);
  l1.push_back(4);
  l1.push_back(5);

  for (auto rit = l1.rbegin(); rit != l1.rend(); ++rit) {
    std::cout << *rit << ' ';
  }
  std::cout << '\n';
}
}  // namespace lx
