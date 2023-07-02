#include "s21_array.h"

namespace s21 {

template <class T, size_t V>
bool Array<T, V>::ArrayIterator::operator==(
    const ArrayIterator& other) const noexcept {
  if (this == &other) {
    return true;
  }
  return _array == other._array;
};

template <class T, size_t V>
Array<T, V>::Array(std::initializer_list<value_type> const& items) {
  if (items.size() > capacity_) {
    throw std::out_of_range("s21::Array::initializer limit is exceeded");
  }
  int j = 0;
  for (auto i = items.begin(); i != items.end(); i++) {
    array_[j] = *i;
    ++j;
  }
};

template <class T, size_t V>
Array<T, V>::Array(const Array& a) noexcept {
  CopyArray(a);
};

template <class T, size_t V>
T& Array<T, V>::At(size_type pos) {
  if (pos < 0 || pos >= capacity_) {
    throw std::out_of_range("s21::Array::At incorrect value");
  }
  return array_[pos];
};

template <class T, size_t V>
T& Array<T, V>::operator[](size_type pos) {
  if (pos < 0 || pos >= capacity_) {
    throw std::out_of_range("s21::Array::At incorrect value");
  }
  return array_[pos];
};

template <class T, size_t V>
const T& Array<T, V>::Front() const noexcept {
  return array_[0];
};

template <class T, size_t V>
const T& Array<T, V>::Back() const noexcept {
  if (capacity_) {
    return array_[(int)(capacity_ - 1)];
  } else {
    return array_[0];
  }
};

template <class T, size_t V>
T* Array<T, V>::Data() noexcept {
  return array_;
};

template <class T, size_t V>
T* Array<T, V>::Begin() noexcept {
  return array_;
};

template <class T, size_t V>
T* Array<T, V>::End() noexcept {
  return &array_[capacity_];
};

template <class T, size_t V>
bool Array<T, V>::Empty() const noexcept {
  if (capacity_) {
    return false;
  } else {
    return true;
  }
};

template <class T, size_t V>
size_t Array<T, V>::Size() const noexcept {
  return capacity_;
};

template <class T, size_t V>
size_t Array<T, V>::MaxSize() const noexcept {
  return capacity_;
};

template <class T, size_t V>
void Array<T, V>::Swap(Array& other) noexcept {
  std::swap(this->array_, other.array_);
};

template <class T, size_t V>
void Array<T, V>::Fill(const_reference value) noexcept {
  for (size_type i = 0; i < capacity_; ++i) array_[i] = value;
};

template <class T, size_t V>
void s21::Array<T, V>::MoveArray(Array& a) noexcept {
  Fill(value_type(0));
  if (this != &a) {
    std::move(a.array_, a.array_ + V, this->array_);
  }
};

template <class T, size_t V>
void Array<T, V>::CopyArray(const Array& other) noexcept {
  capacity_ = other.capacity_;
  Fill(value_type(0));
  for (size_type i = 0; i < capacity_; ++i) {
    array_[i] = other.array_[i];
  }
};
}  // namespace s21
