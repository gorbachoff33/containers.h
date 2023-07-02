// created by pizpotli
#ifndef CPP2_s21_containers_LIST_1_S21_LIST_H_
#define CPP2_s21_containers_LIST_1_S21_LIST_H_

#include <initializer_list>
#include <iostream>
#include <limits>

namespace s21 {
template <class T>
class List {
 public:
  using const_reference = const T&;
  using size_type = size_t;
  using reference = T&;
  using value_type = T;

 private:
  struct Node;

 public:
  class ConstListIterator {
   public:
    ConstListIterator() noexcept { _node = nullptr; }
    ConstListIterator(Node* node) noexcept : _node(node) {}
    ConstListIterator(const ConstListIterator& other) noexcept
        : _node(other._node) {}
    ConstListIterator(ConstListIterator&& other) noexcept {
      _node = other._node;
      other._node = nullptr;
    }
    ~ConstListIterator() noexcept {}
    bool operator==(const ConstListIterator other) const noexcept;
    bool operator!=(const ConstListIterator other) const noexcept {
      return !operator==(other);
    }
    ConstListIterator operator++() noexcept;
    ConstListIterator operator++(int) noexcept;
    ConstListIterator operator--() noexcept;
    ConstListIterator operator--(int) noexcept;
    const_reference operator*() noexcept;

   public:
    Node* _node;
    value_type _zero = 0;
    reference error_ = _zero;
  };
  using const_Iterator = List<T>::ConstListIterator;

  class ListIterator : public ConstListIterator {
   public:
    ListIterator() noexcept : ConstListIterator() {}
    ListIterator(Node* node) noexcept : ConstListIterator(node) {}
    ListIterator(const ListIterator& node) noexcept : ConstListIterator(node) {}
    ListIterator(ListIterator&& other) noexcept : ConstListIterator(other) {}
    ~ListIterator() noexcept {}
    reference operator*() noexcept;
    reference operator=(T value) noexcept {
      return this->_node->value_ = value;
    }
    void operator=(const ListIterator& node) noexcept {
      this->_node = node._node;
    }
    void operator=(const_Iterator node) noexcept { this->_node = node._node; }
    void operator=(Node* node) noexcept { this->_node = node; }
    void operator=(Node& node) noexcept { this->_node = &node; }
  };
  using Iterator = List<T>::ListIterator;

  List() noexcept;
  List(size_type n);
  List(std::initializer_list<T> const& items);
  List(const List& l) noexcept;
  List(List&& l) noexcept;
  ~List() noexcept;
  void operator=(List&& l) noexcept;
  void operator=(List& l) noexcept;
  const_reference Front() noexcept;
  const_reference Back() noexcept;
  Iterator Begin() const noexcept;
  Iterator End() const noexcept;
  bool Empty() const noexcept;
  size_type Size() const noexcept;
  size_type MaxSize() const noexcept;
  Iterator Insert(Iterator pos, const_reference value);
  void Clear() noexcept;
  void Erase(Iterator pos);
  void PushBack(const_reference value);
  void PushFront(const_reference value);
  void PopBack() noexcept;
  void PopFront() noexcept;
  void Swap(List& other) noexcept;
  void Merge(List& other);
  void Splice(const_Iterator pos, List& other);
  void Reverse() noexcept;
  void Unique();
  void Sort() noexcept;
  template <typename... Args>
  void EmplaceFront(Args&&... args);
  template <typename... Args>
  void EmplaceBack(Args&&... args);
  template <typename... Args>
  Iterator Emplace(Iterator pos, Args&&... args);

 private:
  struct Node {
    Node() noexcept : next_(nullptr), prev_(nullptr), value_(0) {}
    Node(const_reference t) noexcept
        : value_(t), next_(nullptr), prev_(nullptr) {}
    T value_;
    Node* next_;
    Node* prev_;
  };
  Node* _head;
  Node* _tail;
  Node* _end;
  size_type _size;
  value_type error_ = 0;
  reference zero_ = error_;

  void CopyList(const List& l) noexcept;
  void MoveList(List& l) noexcept;
  void EndAddress() noexcept;
};
}  // namespace s21
#include "s21_list.tpp"
#endif  // CPP2_s21_containers_LIST_1_S21_LIST_H_