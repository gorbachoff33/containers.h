// created by pizpotli
#ifndef CPP2_s21_containers_QUEUE_1_S21_QUEUE_H_
#define CPP2_s21_containers_QUEUE_1_S21_QUEUE_H_

#include <initializer_list>
#include <iostream>

namespace s21 {
template <class T>
class Queue {
 public:
  using value_type = T;
  using reference = T&;
  using const_reference = const T&;
  using size_type = size_t;

  Queue() noexcept;
  Queue(const std::initializer_list<T>& items);
  Queue(const Queue& q) noexcept;
  Queue(Queue&& q) noexcept;
  ~Queue() noexcept;
  void operator=(Queue&& q) noexcept;
  void operator=(Queue& q) noexcept;
  const_reference Front() const noexcept;
  const_reference Back() const noexcept;
  bool Empty() const noexcept;
  size_t Size() const noexcept;
  void Push(const T& value) noexcept;
  void Pop() noexcept;
  void Swap(Queue& other) noexcept;
  template <typename... Args>
  void EmplaceBack(Args&&... args);

 private:
  struct Node {
    Node() noexcept : next_(nullptr) {}
    Node(const T& value) noexcept : value_(value), next_(nullptr) {}
    T value_;
    Node* next_;
  };
  int _size = 0;
  Node* _head;
  Node* _prev;

  void Clear() noexcept;
  void MoveQueue(Queue& other) noexcept;
  void NewElement(const T& value) noexcept;
  void DeleteElement() noexcept;
  void CopyQueue(const Queue& other) noexcept;
};
}  // namespace s21

#include "s21_queue.tpp"
#endif  // CPP2_s21_containers_QUEUE_1_S21_QUEUE_H_