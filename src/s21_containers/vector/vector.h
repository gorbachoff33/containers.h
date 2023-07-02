#ifndef VECTOR_H_INCLUDED
#define VECTOR_H_INCLUDED
#include <initializer_list>
#include <iostream>
#include <limits>
#include <utility>
namespace s21 {
template <typename T>
class Vector {
 public:
  class iter;
  class ConstIterator;
  friend ConstIterator;
  friend class iter;
  using value_type = T;
  using reference = T&;
  using const_reference = const T&;
  using iterator = iter;
  using const_iterator = ConstIterator;
  using size_type = size_t;
  value_type nil_ = 0;
  reference nil_ref_ = nil_;

 private:
  size_type m_size_;
  size_type m_capacity_;
  T* arr_ = nullptr;

 public:
  Vector() noexcept : m_size_(0), m_capacity_(0), arr_(nullptr){};
  explicit Vector(size_type n) noexcept
      : m_size_(n), m_capacity_(n), arr_(n ? new value_type[n] : nullptr){};
  Vector(std::initializer_list<value_type> const& items);
  Vector(const Vector& v) noexcept
      : m_size_(v.m_size_), m_capacity_(v.m_capacity_) {
    arr_ = new value_type[m_capacity_];
    iter it = begin();
    size_type i = 0;
    while (it != end()) {
      arr_[i] = v.arr_[i];
      i++;
      ++it;
    }
  };
  Vector(Vector&& v) noexcept { MoveFnc(v); }

  void operator=(Vector<value_type>&& v) noexcept {
    DeleteAll();
    MoveFnc(v);
  }

  iter begin() noexcept;
  iter end() noexcept;

  template <typename... Args>
  void emplace_back(Args&&... args);

  template <typename... Args>
  iterator emplace(iterator pos, Args&&... args);

  reference at(size_type pos);

  reference operator[](size_type pos);
  const_reference front();
  const_reference back();
  value_type* data() noexcept;
  bool empty() const noexcept;
  size_type size() const noexcept;
  size_type max_size() const noexcept;
  void reserve(size_type size);
  size_type capacity() noexcept;
  void shrink_to_fit() noexcept;
  void clear() noexcept;
  iter insert(iter pos, const_reference value) noexcept;
  void erase(iter pos) noexcept;
  void push_back(const_reference value);
  void pop_back() noexcept;
  void swap(Vector<value_type>& other) noexcept;
  // destructor

  ~Vector() { DeleteAll(); }

  class iter {
   private:
   public:
    T* elem;
    iter() noexcept { elem = nullptr; }
    iter(T* o) noexcept { elem = o; }
    iter(const iter& o) noexcept { elem = o.elem; }
    bool operator==(const iter& o) const noexcept {
      return this->elem == o.elem;
    }
    void operator++() noexcept { elem++; }
    void operator--() noexcept { elem--; }
    iter operator=(T* o) noexcept {
      elem = o;
      return *this;
    }
    bool operator!=(const iter& o) const noexcept {
      return this->elem != o.elem;
    }
    T& operator*() noexcept { return *elem; }
  };
  class ConstIterator : public iter {
   public:
    ConstIterator() noexcept : iter() {}
    ConstIterator(T* o) noexcept : iter(o) {}
    ConstIterator(const iter& o) noexcept : iter(o) {}
    ConstIterator operator=(T* o) noexcept {
      this->elem = o;
      return *this;
    }
    const_reference operator*() noexcept { return *(this->elem); }
  };

 private:
  void MoveFnc(Vector<value_type>& v) noexcept;
  void MoveRight(iterator pos) noexcept;
  void DeleteAll() noexcept {
    m_size_ = 0;
    m_capacity_ = 0;
    if (arr_) {
      delete arr_;
    }
    arr_ = nullptr;
  };
};
}  // namespace s21
#endif  // VECTOR_H_INCLUDED
#include "vector.tpp"