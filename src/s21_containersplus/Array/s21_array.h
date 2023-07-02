// created by pizpotli
#ifndef CPP2_s21_containers_ARRAY_1_S21_ARRAY_H_
#define CPP2_s21_containers_ARRAY_1_S21_ARRAY_H_

#include <initializer_list>
#include <iostream>
#include <limits>
#include <utility>

namespace s21 {
template <class T, const size_t V>
class Array {
 public:
  class ArrayIterator {
   private:
    T *_array;

   public:
    ArrayIterator() : _array(nullptr){};
    ArrayIterator(T *other) : _array(other){};
    bool operator==(const ArrayIterator &other) const noexcept;
    bool operator!=(const ArrayIterator &other) const noexcept {
      return !operator==(other);
    };
  };
  using value_type = T;
  using reference = T &;
  using const_reference = const T &;
  using Iterator = T *;
  using const_Iterator = const T *;
  using size_type = size_t;

  Array() noexcept {};
  Array(std::initializer_list<value_type> const &items);
  Array(const Array &a) noexcept;
  Array(Array &&a) noexcept { MoveArray(a); };
  ~Array() noexcept {};
  void operator=(Array &&a) noexcept { MoveArray(a); };
  void operator=(Array &a) noexcept { CopyArray(a); };
  reference At(size_type pos);
  reference operator[](size_type pos);
  const_reference Front() const noexcept;
  const_reference Back() const noexcept;
  Iterator Data() noexcept;
  Iterator Begin() noexcept;
  Iterator End() noexcept;
  bool Empty() const noexcept;
  size_type Size() const noexcept;
  size_type MaxSize() const noexcept;
  void Swap(Array &other) noexcept;
  void Fill(const_reference value) noexcept;

 private:
  void MoveArray(Array &a) noexcept;
  void CopyArray(const Array &a) noexcept;
  value_type array_[V + 1];
  size_type capacity_ = V;
};
}  // namespace s21

#include "s21_array.tpp"
#endif  // CPP2_s21_containers_ARRAY_1_S21_ARRAY_H_
