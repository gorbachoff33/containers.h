#ifndef STACK_H_
#define STACK_H_
#include <initializer_list>
#include <iostream>
#include <limits>
#include <utility>
#include <vector>
namespace s21 {
template <typename T>
class Stack {
 public:
  using value_type = T;
  using reference = T &;
  using const_reference = const T &;
  using size_type = size_t;
  const_reference top();
  bool empty() noexcept;
  size_type size() const noexcept;
  void push(const_reference value) noexcept;
  void pop() noexcept;
  void swap(Stack &other) noexcept;
  template <typename... Args>
  void emplace_front(Args &&...args);

  Stack() noexcept;
  Stack(std::initializer_list<T> const &items);
  Stack(const Stack &s) noexcept;
  Stack(Stack &&s) noexcept;
  ~Stack() noexcept;
  void delete_all() noexcept {
    size_all_ = 0;
    while (root_) {
      pop();
    }
  }
  void operator=(Stack &&s) noexcept;

 private:
  void MoveFnc(Stack &s) noexcept;
  size_type size_all_ = 0;
  struct node {
    value_type value_;
    node *next_;
  };
  node *root_;
};
}  // namespace s21
#endif  // STACK_H_
#include "stack.tpp"