#pragma once

template <class Iterator, class Ref, class Ptr>
class ReverseIterator {
 public:
  typedef ReverseIterator<Iterator, Ref, Ptr> self;

  Iterator it_;

  ReverseIterator(Iterator it) : it_(it) {}
  ReverseIterator(const ReverseIterator& rit) : it_(rit.it_) {}

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

  bool operator!=(const self& s) { return it_ != s.it_; }

  bool operator==(const self& s) { return !(it_ != s.it_); }

  Ref operator*() { return *it_; }

  Ptr operator->() { return it_.operator->(); }
};